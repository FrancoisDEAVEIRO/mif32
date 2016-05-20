#include "Noeud.hpp"

Noeud::Noeud(){
	id = -1;
	X = -1;
	Y = -1;
	value = -1;
	espace.xMin = -1;
	espace.xMax = -1;
	espace.yMin = -1;
	espace.yMax = -1;
}

Noeud::Noeud(int num, int val, int x, int y){
	value = val;
	X = x;
	Y = y;
	id = num;
	espace.xMin = -1;
	espace.xMax = -1;
	espace.yMin = -1;
	espace.yMax = -1;
}

void Noeud::create(int N, int maxValue){
	value = rand()%maxValue;
	X = rand()%N;
	Y = rand()%N;
}

void Noeud::affiche(){
	std::cout << id << ":" <<"["<<X<<","<<Y<<"] = "<<value<<" esp : {"<< 
			espace.xMin<<","<< espace.xMax<<","<< espace.yMin<<","<< espace.yMax<<"}"<<std::endl;
}

int Noeud::hauteur(){
	return espace.yMax-espace.yMin;
}

int Noeud::largeur(){
	return espace.xMax-espace.xMin;
}

