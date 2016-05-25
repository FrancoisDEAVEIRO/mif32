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
    }else{
		int idResp = resp(n.X, n.Y).id;
		for(std::vector<Noeud>::iterator i = noeuds.begin(); i != noeuds.end();i++){
			if((*i).id == idResp){
				// On divise en 2 l'espace
				if((*i).hauteur() >= (*i).largeur()){
					n.espace.xMin = (*i).espace.xMin;
					n.espace.xMax = (*i).espace.xMax;
					if((*i).Y >= (*i).espace.yMax/2){
						n.espace.yMin = (*i).espace.yMin;
						(*i).espace.yMin = (*i).espace.yMax/2;
						n.espace.yMax = (*i).espace.yMin;
					}else{
						n.espace.yMax = (*i).espace.yMax;
						(*i).espace.yMax = (*i).espace.yMax/2;
						n.espace.yMin = (*i).espace.yMax;
					}
				}else{
					n.espace.yMin = (*i).espace.yMin;
					n.espace.yMax = (*i).espace.yMax;
					if((*i).X >= (*i).espace.xMax/2){
						n.espace.xMin = (*i).espace.xMin;
						(*i).espace.xMin = (*i).espace.xMax/2;
						n.espace.xMax = (*i).espace.xMin;
					}else{
						n.espace.xMax = (*i).espace.xMax;
						(*i).espace.xMax = (*i).espace.xMax/2;
						n.espace.xMin = (*i).espace.xMax;
					}
				}
				// On vérifie que le noeud insérer à ses coordonnées qui sont dans le nouvel espace, sinon on lui en donne de nouvelle
				if(n.espace.xMin>n.X || n.espace.xMax < n.X || n.espace.yMin > n.Y || n.espace.yMax < n.Y ){
					n.X = (rand()% (n.espace.yMax - n.espace.yMin)) + n.espace.yMin;
					n.Y = (rand()% (n.espace.yMax - n.espace.yMin)) + n.espace.yMin;
				}
				// Mise à jour du voisin
				(*i).idVoisin = n.id;
				n.id = (*i).id;
			}
		}
    }
	std::cout << "noeud inseré" << std::endl;
	noeuds.push_back(n);
	affiche();
}


void Grille::affiche(){
    for(std::vector<Noeud>::iterator i = noeuds.begin(); i != noeuds.end();i++){
        (*i).affiche();
    }
}

Noeud& Grille::resp(int x, int y){
    for(std::vector<Noeud>::iterator i = noeuds.begin(); i != noeuds.end();i++){
        if((*i).espace.xMin < x && (*i).espace.xMax >= x && (*i).espace.yMin < y && (*i).espace.yMax >= y)
            return (*i);
    }
    return noeuds[0];
}






