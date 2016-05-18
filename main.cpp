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
	for(int i=0; i<taille; i++){
		a[i]=-1;
	}
	// Coordinateur
	if(my_rank==0){
		position = 0;
		a[0] = 5;
		a[1] = 7;
		for(int i=1; i<nb_process; i++){
			char buff[N*N];
			MPI_Pack(a, taille, MPI_INT, buff, N*N, &position, MPI_COMM_WORLD);	
			std::cout <<std::endl << "pack 0 : " ;
			for(int j=0; j<taille ; j++){
				std::cout<<a[j]<<" ";
			}
			MPI_Send( a, taille, MPI_PACKED, i, 0, MPI_COMM_WORLD);
			std::cout << std::endl << "send 0 : " ;
			for(int j=0; j<taille ; j++){
				std::cout<<a[j]<<" ";
			}
			MPI_Recv(a, taille, MPI_PACKED, i, 0, MPI_COMM_WORLD, &status);
			std::cout << std::endl << "reçu 0 : " ;
			for(int j=0; j<taille ; j++){
				std::cout<<a[j]<<" ";
			}
		}
	}
	//Autres noeuds
	else{
		position = 0;
		/*std::cout << std::endl << "test "<< my_rank << " : " ;
		for(int j=0; j<taille ; j++){
			std::cout<<a[j]<<" ";
		}*/
		char buff[N*N];
		x=0;
		MPI_Recv( a, taille, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		//MPI_Unpack(buff, N*N, &position, a, taille, MPI_INT, MPI_COMM_WORLD);
		std::cout << std::endl << "reçu "<< my_rank << " : " ;
		for(int j=0; j<taille ; j++){
			std::cout<<a[j]<<" ";
		}
		Noeud n;
		n.create(N,100);
		grille.insertion(n);		
		for(int i=0; i<taille; i++){
			if(a[i]==-1){
				a[i] = my_rank;
				break;
			}
		}
		MPI_Pack(a, taille, MPI_INT, buff, N*N, &position, MPI_COMM_WORLD);
		std::cout << std::endl << "pack "<< my_rank<<" : ";
		for(int j=0; j<taille ; j++){
				std::cout<<a[j]<<" ";
		}
		MPI_Send(a, taille, MPI_PACKED, 0,  0, MPI_COMM_WORLD);
		std::cout << std::endl << "send "<< my_rank<<" : ";
		for(int j=0; j<taille ; j++){
			std::cout<<a[j]<<" ";
		}
	}
	
	
	MPI_Finalize();
	return 0;
}
