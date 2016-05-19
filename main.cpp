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
    int taille = 20;
    int position, x, y, a[taille];
    for(int j=0; j<taille; j++){
        a[j]=-1;
    }
    // Coordinateur
    if(my_rank==0){
        position = 0;
        for(int i=1; i<nb_process; i++){
            
            
            MPI_Send(a, taille, MPI_PACKED, i, 0, MPI_COMM_WORLD);
            MPI_Recv(a, taille, MPI_PACKED, i, 0, MPI_COMM_WORLD, &status);
            
            std::cout<<"reception";
            for(int j=0; j<taille ; j++){
                std::cout<<a[j]<<" ";
            }
            std::cout<<std::endl;
        }
        
        
    }
    //Autres noeuds
    else{
        position = 0;
        x=0;
        MPI_Recv( a, taille, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        Noeud n;
        n.create(N,100);
        grille.insertion(n);        
        for(int i=0; i<taille; i++){
            if(a[i]==-1){
                a[i] = my_rank;
                a[i+1] = n.X;
                a[i+2] = n.Y;
                break;
            }
        }
        MPI_Send(a, taille, MPI_PACKED, 0,  0, MPI_COMM_WORLD);
        std::cout << std::endl << "send "<< my_rank<<" : ";
        for(int j=0; j<taille ; j++){
            std::cout<<a[j]<<" ";
        }
        std::cout << std::endl;
    }
    
    
    MPI_Finalize();
    return 0;
}

