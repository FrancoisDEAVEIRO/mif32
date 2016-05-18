#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <vector>

#include "Noeud.hpp"

class Grille{
	public : 
		int N;
		std::vector<int> tab;
		std::vector<Noeud> noeuds;
		Grille();
		Grille(int);
		void insertion(Noeud&);
		void affiche();
	
};




#endif // GRILLE_HPP