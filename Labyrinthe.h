#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include "Environnement.h"
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#define	LAB_WIDTH	80
#define	LAB_HEIGHT	160

class Labyrinthe : public Environnement {
private:
	char	_data [LAB_WIDTH][LAB_HEIGHT];

public:
	Labyrinthe (char*);
	int width () { return LAB_WIDTH;}	// retourne la largeur du labyrinthe.
	int height () { return LAB_HEIGHT;}
	// retourne la longueur du labyrinthe.
	char data (int i, int j)
	{
		return _data [i][j];
	}	// retourne la case (i, j).
  /*  void set_data (int i , int j,int k){
    	if(_data[i][j]==0)
    	_data[i][j]=k;
    }*/
};

#endif
