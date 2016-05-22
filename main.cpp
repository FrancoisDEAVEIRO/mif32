#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Noeud.hpp"
#include "Grille.hpp"
#define N 1000

/**** TAGS ****/
#define INSERTION 1
#define RECHERCHE 2
#define SUPPRESSION 3
#define INFO 4
#define FIN 5
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
    int taille = (nb_process-1)*nbInfoNode;
    int buff[taille], tag[1];
    for(int j=0; j<taille; j++){
        buff[j]=-1;
    }
    // Coordinateur
    if(my_rank==0){
		
		// Création du réseau
        for(int i=1; i<nb_process; i++){ 
			tag[0] = INSERTION;  
			MPI_Send(tag, 1, MPI_INT, i, 2, MPI_COMM_WORLD);        
            MPI_Send(buff, taille, MPI_INT, i, 0, MPI_COMM_WORLD);
            fichier << "Envoie de 0 vers : " << i <<std::endl ;
            MPI_Recv(buff, taille, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            fichier << "Reception en 0, expediteur : " << i <<std::endl ;
            std::cout<<"reception par 0 : ";
            for(int j=0; j<taille ; j++){
                std::cout<<buff[j]<<" ";
            }
            std::cout << std::endl;
            Noeud n(i, buff[(i-1)*nbInfoNode], buff[(i-1)*nbInfoNode+1]);
            n.affiche();
            int resp = grille.resp(n.X, n.Y);
            // cas de la grille vide
			grille.insertion(n);
        } 
        
        // Insertion de données dans le réseau
        // On ajoute N*10 données
        for(int i=1; i<nb_process*10; i++){ 
			
		}
    }
    //Autres noeuds
    else{
		// Reception du type de message à traiter ensuite
		MPI_Recv( tag, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
		std::cout << "info reçu : source = " << status.MPI_SOURCE << " tag : " << tag[0] << std::endl;

		switch(tag[0]){
			case INSERTION : {
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
				std::cout << std::endl << "send "<< my_rank<<" : ";
				for(int j=0; j<taille ; j++){
					std::cout<<buff[j]<<" ";
				}
				std::cout << std::endl;
				break;
			}{
			case RECHERCHE :
				break;
			}{
			default:
				std::cout << "error tag type" << std::endl;
				break;
			}
		}
        
    }
    
    
    MPI_Finalize();
    fichier.close();
    return 0;
}




