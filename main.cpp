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
	int echec = 1;
	// Coordinateur
	if(my_rank==0){
		for(int i=1; i<nb_process; i++){
			MPI_Send(&echec, 1, MPI_INT, i,  99, MPI_COMM_WORLD);
			MPI_Recv(&echec, 1, MPI_INT, i, 99, MPI_COMM_WORLD, &status);
			std::cout << i << " reçu" << std::endl;
		}
	}
	//Autres noeuds
	else{
		MPI_Recv(&echec, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, &status); 
		Noeud n;
		n.create(N,100);
		grille.insertion(n);
		std::cout<<grille.noeuds.size()<<std::endl;
		n.affiche();
		if(my_rank!=0)
			MPI_Send(&echec, 1, MPI_INT, 0,  99, MPI_COMM_WORLD);
		std::cout << my_rank << " envoie à 0"<< std::endl;
	}
	
	
	MPI_Finalize();
	return 0;
}
