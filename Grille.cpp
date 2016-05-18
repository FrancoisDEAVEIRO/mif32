#include "Grille.hpp"

Grille::Grille(){
	N = 1000;
	for(int i=0; i<N*N; i++){
			tab.push_back(0);
	}
}

Grille::Grille(int n){
	N = n;
	for(int i=0; i<N*N; i++){
			tab.push_back(0);
	}
}

void Grille::insertion(Noeud& n){
	// Cas du noeud qui prend en charge toute la grille (id = 1)
	if(noeuds.size() == 0){
		n.espace.xMin = 0;
		n.espace.xMax = N;
		n.espace.yMin = 0;
		n.espace.yMax = N;
		noeuds.push_back(n);
	}else{
		;
	}
}


void Grille::affiche(){
	for(std::vector<Noeud>::iterator i = noeuds.begin(); i != noeuds.end();i++){
		(*i).affiche();
	}
}
