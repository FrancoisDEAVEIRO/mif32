#include <mpi.h>
#include <stdio.h>
#include <iostream>

#include "Noeud.hpp"
#include "Grille.hpp"
#define N 1000

int main(int argc, char** argv) {
    
    int my_rank;
    Grille grille;
    MPI_Status status;    
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int nb_process;
    MPI_Comm_size(MPI_COMM_WORLD, &nb_process);
    int nbInfoNode = 4;
    int taille = (nb_process-1)*nbInfoNode;
    int position, x, y, buff[taille];
    for(int j=0; j<taille; j++){
        buff[j]=-1;
    }
    // Coordinateur
    if(my_rank==0){
        position = 0;
        for(int i=1; i<nb_process; i++){
                   
            MPI_Send(buff, taille, MPI_INT, i, 123, MPI_COMM_WORLD);
            MPI_Recv(buff, taille, MPI_INT, i, 123, MPI_COMM_WORLD, &status);
            
            std::cout<<"reception par 0 : ";
            for(int j=0; j<taille ; j++){
                std::cout<<buff[j]<<" ";
            }
            std::cout << std::endl;
            Noeud n(i, buff[(i-1)*nbInfoNode], buff[(i-1)*nbInfoNode+1], buff[(i-1)*nbInfoNode+2]);
            n.affiche();
            int resp = grille.resp(n.X, n.Y);
            // cas de la grille vide
            if(resp == 0){
				std::cout<<"insertion du premier noeud"<<std::endl;
			}
        }
        
        
    }
    //Autres noeuds
    else{
        position = 0;
        x=0;
        MPI_Recv( buff, taille, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
        // seed RNG
        srand(time(NULL) + my_rank);
        Noeud n;
        n.create(N,100);        
        for(int i=0; i<taille; i++){
            if(buff[i]==-1){
                buff[i] = my_rank;
                buff[i+1] = n.X;
                buff[i+2] = n.Y;
                buff[i+3] = n.value;
                break;
            }
        }
        MPI_Send(buff, taille, MPI_INT, 0,  123, MPI_COMM_WORLD);
        std::cout << std::endl << "send "<< my_rank<<" : ";
        for(int j=0; j<taille ; j++){
            std::cout<<buff[j]<<" ";
        }
        std::cout << std::endl;
    }
    
    
    MPI_Finalize();
    return 0;
}

