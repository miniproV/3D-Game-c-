#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"
char laby [150][150];
Sound*	Chasseur::_hunter_fire;	// bruit de l'arme du chasseur.
Sound*	Chasseur::_hunter_hit;	// cri du chasseur touché.
Sound*	Chasseur::_wall_hit;	// on a tapé un mur.

Environnement* Environnement::init (char* filename)
{
    return new Labyrinthe (filename);
}

bool reliedLigne (int x1, int x2,int y1, int y2)
{

    if(x1==x2)
    {

        for (int i = y1+1; i < y2; ++i)
        {

            if ( laby[x1][i]=='-' or laby[x1][i]=='+' or laby[x1][i]=='a' or laby[x1][i]=='b')
            {
                return true;
                break;
            }
            else
            {
                return false;
                break;
            }

        }
    }



}


bool reliedcol (int x1, int x2,int y1, int y2)
{
    for (int i = x1+1; i < x2; ++i)
    {

        if(y1==y2)
        {
            if ( (laby[i][y1]!='|'  &&  laby[i][y1]!='+' &&  laby[i][y1]!='b' && laby[i][y1]!='a'))
            {
                return false ;
            }


        }
        else
            return false;

    }
    return true;

}

Labyrinthe::Labyrinthe (char* filename)
{
    /*===========================  Off  =====================*/


    /*================= Pour les coordonneés des debuts de murs   ==============*/
    std::vector<int> X;
    std::vector<int> Y;
    /*=================Pour les coordonneés des debuts des box    ==============*/

    std::vector<int> Xb;
    std::vector<int> Yb;
    /*=================    ==============*/
    int nb_g=0; /*counter nombre gardien*/
    int w=0;/*counter nombre walls*/
    /*=================  Pour les coordonneés des debuts des gardiens  ==============*/
    std::vector<int> Xg;
    std::vector<int> Yg;
    /*=================type de gardien    ==============*/
    const char *type[4] = {"Blade","Lezard","Serpent","Samourai"};
    /*=================Pour les coordonneés des debuts des marques    ==============*/
    std::vector<int> Xm;
    std::vector<int> Ym;
    /*================= Pour les coordonneés des debuts d'affiches de types a   ==============*/
    std::vector<int> Xa;
    std::vector<int> Ya;
    /*================= Pour les coordonneés des debuts d'affiches de types b   ==============*/
    std::vector<int> Xav;
    std::vector<int> Yav;
    /*================= Chasseur   ==============*/
    float Xc=0.;
    float Yc=0.;
    /*=================  Treasor  ==============*/
    float Xt=0;
    float Yt=0;

    /*===========================  Off  =====================*/



    /*===========================File TO 2D ARRAY =====================*/

    ifstream fichier(filename);


    //L'ouverture s'est bien passée, on peut donc lire
    int lignes=0;
    int colon = 0;
    int comments =0;


    char c;
    while (fichier.get(c))
    {
        colon++;
        if(c=='#' or iswalpha(c))
        {
            comments++;
        }
        laby[lignes][colon]=c;
        if(c=='\n')
        {
            lignes+=1;
            colon=0;
        }


        if(c==EOF)
        {
            lignes+=1;
            colon=0;
            fichier.close();
        }
    }
    int first=0;


    /*===========================Ignorer les lignes avant le premier + =====================*/
    for (int i = 0; i <= lignes; ++i)
    {
        if(laby[i][1]=='+')
        {
            first=i;
            break;
        }
    }

    for (int i = 0; i <= lignes; ++i)
    {
        for(int j=0; j<100; j++) /* code */
        {

            laby[i][j]=laby[i+first][j];
        }
    }


    /*===========================  (X,Y) Off Components =====================*/
    for (int i = 0; i <lignes; ++i)
    {
        for(int j=0; j<100; j++) /* code */
        {

            switch (laby[i][j])
            {

            case 'G':
                Xg.push_back(i);
                Yg.push_back(j);
                nb_g++;
                break;
            case 'a':
                Xa.push_back(i);
                Ya.push_back(j);

                break;

            case 'b':
                Xav.push_back(i);
                Yav.push_back(j);


                break;
            case 'm':
                Xm.push_back(i);
                Ym.push_back(j);
                break;


            case 'x':
                Xb.push_back(i);
                Yb.push_back(j);
                break;


            case 'C':
                Xc=i;
                Yc=j;
                nb_g++;
                break;
            case 'T':
                Xt=i;
                Yt=j;
                break;
            case '+':
                X.push_back(i);
                Y.push_back(j);
                break;



            }


        }
    }
    /*===========================  Off  =====================*/


    /*===========================  Construction  =====================*/







    int n_b=0;                      /*Nombres de caisses*/
    static Box	caisses [10];
    for(int i=0; i<(int)Xb.size(); i++)
    {

        caisses[n_b]= {Xb[i], Yb[i], 0 };
        n_b++;

    }
    for (int i = n_b; i >=0; --i)
    {
        _data [caisses [i]._x][caisses [i]._y] = 1;
    }

    _nboxes = n_b;
    _boxes = caisses;


    int n_m=0; /*nombre de marques */
    static Box	marques [10] ;
    for(int i=0; i<(int)Xm.size(); i++)
    {

        marques[n_m]= {Xm[i], Ym[i], 2 };
        char	tmp [128];
        sprintf (tmp, "%s/%s", texture_dir, "entr.jpg");
        marques [0]._ntex = wall_texture (tmp);
        sprintf (tmp, "%s/%s", texture_dir, "sortie.jpg");
        marques [1]._ntex = wall_texture (tmp);
        n_m++;

    }
    _nmarks =n_m;
    _marks = marques;

    /*===========================  Construction  Gardiens =====================*/
    _nguards = nb_g;                    /* nombre de gardien  sans le chasseur */
    _guards = new Mover* [_nguards];
    _guards [0] = new Chasseur (this);
    _guards [0] -> _x =  Xc*10;
    _guards [0] -> _y =  Yc*10;
    _guards [0]-> vie=1000;
    _guards[0]-> force=50;
    _data [(int)(_guards [0] -> _x / scale)][(int)(_guards [0] -> _y / scale)] = 1;
    for (int il = nb_g; il >0; --il)
    {

        int v1 = rand() % 4;

        /* code */_guards [il] = new Gardien (this,type[v1]);
        _guards [il] -> _x = Xg[il-1]*10;
        _guards [il] -> _y = Yg[il-1]*10;
        switch(v1)
        {

        case 0:
            _guards [il] -> vie=300;
            _guards[il]-> force=30;
            break;
        case 1:
            _guards [il] -> vie=500;
            _guards[il]-> force=40;
            break;

        case 2:
            _guards [il] -> vie=200;
            _guards[il]-> force=20;

            break;
        case 3:
            _guards [il] -> vie=1000;
            _guards[il]-> force=50;
            break;
        }
        _data [(int)(_guards [il] -> _x / scale)][(int)(_guards [il] -> _y / scale)] = 1;

    }
    /*===========================  Construction  =====================*/


    static Wall affiche [1000];
    int n_a=0; /* Nomber affiches 'a' */

    for(int l=0; l<=(int)Xa.size(); l++)
    {
        if(laby[Xa[l]][Ya[l]+1]=='-')
        {
            affiche[l]= {Xa[l], Ya[l], Xa[l], Ya[l]+2, 0 };
            n_a++;
        }
        else if(laby[Xa[l]][Ya[l]+1]!='-')
        {
            affiche[l]= {Xa[l], Ya[l], Xa[l]+2, Ya[l], 0 };
            n_a++;
        }
    }

    int n_av=0;   /* Nomber affiches 'b' */
    for(int l=0; l<=(int)Xav.size(); l++)
    {

        if(laby[Xav[l]][Yav[l]+1]=='-')
        {
            affiche[l+n_a]= {Xav[l], Yav[l], Xav[l], Yav[l]+2, 0 };
            n_av++;
        }
        else
            affiche[l+n_a]= {Xav[l], Yav[l], Xav[l]+2, Yav[l], 0 };
        n_av++;

    }

    _npicts = n_a+n_av; /* Nombres total d'affiches = nb affiches a + nb affiches b */
    _picts = affiche;

    for (int i = n_a; i <= _npicts; ++i) /*changer la texture des affiches differante de a donc b */
    {
        char	tmp [128];
        sprintf (tmp, "%s/%s", texture_dir, "voiture.jpg");
        _picts [i]._ntex = wall_texture (tmp);
    }

    /*============================================================*/

    static Wall walls [1000];
    /* On verfier si les + sur la meme colonne de laby sont relier on dessine les murs */
    for(int i=0; i<(int)X.size()-1; i++)
    {
        for (int j = i+1; j < (int)X.size(); ++j)
        {
            if(reliedcol(X[i],X[j],Y[i],Y[j]))
            {
                walls[w]= {X[i], Y[i], X[j], Y[i], 0 };


                w++;

            }
        }

    }

    int x=0;
    /* On verfier si les + sur la meme ligne de laby sont relier on dessine les murs  */
    while(x<=lignes)
    {
        for(int l=0; l<=(int)X.size()-1; l++)
        {

            if(x==X[l])
            {
                if (reliedLigne(x,x,Y[l],Y[l+1]))
                {

                    walls[w]= {x, Y[l], x, Y[l+1], 0 };


                    w++;

                }
            }

        }
        x++;

    }
    _nwall = w;
    _walls = walls;
    /* On met tous les points entre  [w->x] et [w->y] data [x][y] == 1 */

    for(int w = _nwall ; w>=0; w--)
    {
        for(int x = walls [w]._x1; x<=walls [w]._x2; x++)
        {
            for(int y=walls [w]._y1; y<=walls [w]._y2; y++)
            {
                _data [x][y] = 1;
            }
        }
    }









    /*===========================  Tresor  =====================*/


    _treasor._x = Xt;
    _treasor._y = Yt;
    _data [_treasor._x][_treasor._y] = 1;


    /*===========================  Off  =====================*/


}
