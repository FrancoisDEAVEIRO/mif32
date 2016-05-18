#ifndef NOEUD_HPP
#define NOEUD_HPP

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Espace{
	int xMin;
	int xMax;
	int yMin;
	int yMax;
};

class Noeud{
	public : 
		int id;
		int X;
		int Y;
		int value;
		Espace espace;	
		Noeud();
		void affiche();
		void create(int,int);
		int hauteur();
		int largeur();
};




#endif // NOEUD_HPP
