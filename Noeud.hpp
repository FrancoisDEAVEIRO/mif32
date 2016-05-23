#ifndef NOEUD_HPP
#define NOEUD_HPP

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
	int xMin;
	int xMax;
	int yMin;
	int yMax;
}Espace;

typedef struct{
	int x;
	int y;
	int value;
}Data;

class Noeud{
	public : 
		int id;
		int X;
		int Y;
		std::vector<Data> tab;
		Espace espace;	
		Noeud();
		Noeud(int,int,int);
		void affiche();
		void create(int);
		int hauteur();
		int largeur();
};


#endif // NOEUD_HPP
