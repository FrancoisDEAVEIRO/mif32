#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Noeud.hpp"
#include "Grille.hpp"
#define N 1000

/**** TAGS ****/
#define CREATION 1
#define INSERTION 2
#define RECHERCHE 3
#define SUPPRESSION 4
#define INFO 5
#define FIN 6
/**************/

int main(int argc, char** argv) {
    std::ofstream fichier;
    fichier.open("log.txt", std::ios::out); //on ouvre le fichier en écriture
    if (fichier.bad()) //permet de tester si le fichier s'est ouvert sans probleme
        return 0;
    int my_rank;
    Grille grille;
    MPI_Status status;    
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int nb_process;
    MPI_Comm_size(MPI_COMM_WORLD, &nb_process);
    int nbInfoNode = 3;
    int nbInfoStockees = 5;
    int taille = (nb_process-1)*nbInfoNode;
    int buff[taille], tag[1], data[2], ack[1];
    for(int j=0; j<taille; j++){
        buff[j]=-1;
    }
    // Coordinateur
    if(my_rank==0){
		srand(time(NULL));
		
		// CREATION du réseau
        for(int i=1; i<nb_process; i++){ 
			tag[0] = CREATION;  
			MPI_Send(tag, 1, MPI_INT, i, 0, MPI_COMM_WORLD);  
			fichier << "[0] Envoie TAG CREATION vers : " << i <<std::endl ;      
            MPI_Send(buff, taille, MPI_INT, i, 0, MPI_COMM_WORLD);
            fichier << "[0] Envoie vers : " << i <<std::endl ;
            MPI_Recv(buff, taille, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            fichier << "[0] Reception des données de la grille, expediteur : " << i <<std::endl ;
            for(int j=0; j<taille ; j++){
                std::cout<<buff[j]<<" ";
            }
            std::cout << std::endl;
            Noeud n(i, buff[(i-1)*nbInfoNode], buff[(i-1)*nbInfoNode+1]);
            n.affiche();
            // cas de la grille vide
			grille.insertion(n);
        }   
        
        // INSERTION de données dans le réseau
        // On ajoute N*10 données
        std::vector<Data> first5Data;
        std::vector<Data> last5Data;
        for(int i=0; i<nb_process*10; i++){ 
			tag[0] = INSERTION; 
			int idProcess = (rand()%(nb_process-1)) +1;
			MPI_Send(tag, 1, MPI_INT, idProcess, 0, MPI_COMM_WORLD); 
			fichier << "[0] Envoie TAG INSERTION vers : " << idProcess <<std::endl ; 
			Data d;
			d.x = rand()%N;
			d.y = rand()%N;
			data[0] = d.x;
			data[1] = d.y;
			// On stock les 5 premières et les 5 derniers données insérées
			if(i<nbInfoStockees){
				first5Data.push_back(d);
			}
			if(i>nb_process*10-nbInfoStockees){
				last5Data.push_back(d);
			}
			MPI_Send(data, 2, MPI_INT, idProcess, 0, MPI_COMM_WORLD);
			fichier << "[0] Envoie de la donnee " << data[0] << " à insérer pour : " << idProcess << std::endl;
			MPI_Recv(ack, 1, MPI_INT, idProcess, 0, MPI_COMM_WORLD, &status);
			fichier << "[0] ACK reçu de " << i << std::endl;
		}	
		
		// RECHERCHE
		for(int i=0; i<nbInfoStockees; i++){
			tag[1] = RECHERCHE; 
			MPI_Bcast(tag, 1, MPI_INT, 0, MPI_COMM_WORLD); 
			// Méthode du Broadcast
			if(first5Data.size() > i){
				data[0] = first5Data[i].x;
				data[1] = last5Data[i].y;			
			}
			MPI_Bcast(data, 2, MPI_INT, 0, MPI_COMM_WORLD); 
			fichier << "[0] Broadcast : recherche de la valeur en  [" << data[0] << "," << data[1] << "]" << std::endl;
			MPI_Recv(ack, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			fichier << "[0] Resultat de la recherche : " << ack[0] << std::endl;
		}
		
		// FIN des transmission, on informe tous les noeuds
		for(int i=1; i<nb_process; i++){ 
			tag[0] = FIN;  
			MPI_Send(tag, 1, MPI_INT, i, 0, MPI_COMM_WORLD); 
			fichier << "[0] Envoie TAG FIN  vers : "  << i << std::endl;
		}
    }
    //Autres noeuds
    else{
		bool fin = false;
		while(!fin){
		// Reception du type de message à traiter ensuite
			MPI_Recv( tag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
			fichier << "["<< my_rank << "] TAG reçu : ";
			
			switch(tag[0]){
				case CREATION : {
					fichier << "CREATION" << std::endl;
					MPI_Recv( buff, taille, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
					// disperse la RNG
					srand(time(NULL) + my_rank);
					Noeud n;
					n.create(N);        
					for(int i=0; i<taille; i++){
						if(buff[i]==-1){
							buff[i] = my_rank;
							buff[i+1] = n.X;
							buff[i+2] = n.Y;
							break;
						}
					}
					MPI_Send(buff, taille, MPI_INT, 0,  0, MPI_COMM_WORLD);
					fichier << "["<< my_rank << "] Noeud crée, envoie au à 0" << std::endl;
					for(int j=0; j<taille ; j++){
						std::cout<<buff[j]<<" ";
					}
					std::cout << std::endl;
					break;
				}{
				case INSERTION :
					fichier << "INSERTION" << std::endl;
					MPI_Recv( data, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
					for(unsigned int i; i<grille.noeuds.size(); i++){
						if(grille.noeuds[i].id == my_rank){
							Data d;
							d.x = data[0];
							d.y = data[1];
							d.value = data[0] + data[1];
							grille.noeuds[i].tab.push_back(d);
							break;
						}
					}
					fichier << "["<< my_rank << "] Donnée insérée" << std::endl;
					ack[0] = 1;
					MPI_Send(ack, 1, MPI_INT, 0,  0, MPI_COMM_WORLD);
					fichier << "["<< my_rank << "] Envoie ACK à 0" << std::endl;
					break;
				}{ case RECHERCHE :
					fichier << "RECHERCHE" << std::endl;
					MPI_Recv( data, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
					fichier << "["<< my_rank << "] Reception du message de broadcast de recherche" << std::endl;
					for(std::vector<Noeud>::iterator i = grille.noeuds.begin(); i != grille.noeuds.end();i++){
						if((*i).id == my_rank){
							for(unsigned int j; j<(*i).tab.size(); j++){
								if((*i).tab[j].x == data[0] && (*i).tab[j].y == data[1]){
									ack[0] = (*i).tab[j].value;
									MPI_Send(ack, 1, MPI_INT, 0,  0, MPI_COMM_WORLD);
									fichier << "["<< my_rank << "] Envoie de la donnée à 0" << std::endl;
								}
							}
							break;
						}
					}
				}{
				case FIN :
					fichier << "FIN" << std::endl;
					fin = true;
					fichier << "["<< my_rank << "] Fin" << std::endl;
					break;
				}{
				default:
					std::cout << "error tag type" << std::endl;
					break;
				}
			}
        }
    }
    
    
    MPI_Finalize();
    fichier.close();
    return 0;
}




