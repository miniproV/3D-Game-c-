#include "Chasseur.h"
#include "Gardien.h"


/*======================GARDIEN========================*/




bool Gardien::move_g(double dx, double dy)
{

    if (EMPTY == _l -> data ((int)((_x + dx) / Environnement::scale),
                             (int)((_y + dy) / Environnement::scale)))
    {
        /*_l->set_data((_x)/ Environnement::scale,(_y)/ Environnement::scale,0);*/
        _x=(float)_x+dx;
        _y=(float)_y+dy;

        return true;
        /* _l->set_data((_x)/ Environnement::scale,(_y)/ Environnement::scale,1);*/
    }
    return false;

}




bool Gardien::process_fireball (float dx, float dy)
{
    if (EMPTY == _l -> data ((int)((_fb -> get_x () + dx) / Environnement::scale),
                             (int)((_fb -> get_y () + dy) / Environnement::scale)))
    {

        return true;
    }

    return false;
}




void Gardien::fire(int angle_vertical)
{


    _fb -> init (/* position initiale de la boule */ _x, _y, 10,
            /* angles de visée */ 180, _angle);


}

    int f=0;/*pour recuperer la force*/

void Gardien::attack()
{

    for (int i =1; i < _l->_nguards; ++i)
    {
        float   x = (_l->_guards[i]->_x - _l->_guards[0]->_x) / Environnement::scale;
        float   y = (_l->_guards[i]->_y - _l->_guards[0]->_y) / Environnement::scale;
        float dist = x*x + y*y;                                     /*distance entre le gardien et le chasseur */

        if( dist<25 && dist>20 && _l->_guards[i]->mort==false)
        {
            _l->_guards[i]->_angle=_l->_guards[0]->_angle+180;
            _l->_guards[i]->fire(_l->_guards[i]->_angle);
            f=_l->_guards[i]->force;

        }
    }
    float   xf = ( _fb -> get_x ()-_l -> _guards [0]-> _x)/ Environnement::scale;
    float   yf = ( _fb -> get_y ()-_l -> _guards [0]-> _y)/ Environnement::scale;
    float   disty = xf*xf + yf*yf;                                  /*distance entre la fb et le chasseur */
    if(disty<1.0 && _fb->get()!=0 && _l->_guards[0]->vie>0 )
    {
        _l -> _guards [0]-> vie-=f;

        message ("========(Chasseur,%d)===",_l -> _guards [0]-> vie);

        if(_l->_guards[0]->vie==0)
        {
            partie_terminee(false);
        }
    }


}



void Gardien::deplacement()
{
    int angles [] = {0,90,180,270};
    int x = rand()%4;

    for (int i = 1; i < _l->_nguards; ++i)
    {


        if(_l->_guards[i]->_angle>=0 && _l->_guards[i]->_angle<90 && _l->_guards[i]->mort==false)
        {
            if( _l->_guards[i]->move(0,0.05) )
            { }
            else  if(_l->_guards[i]->move(0,0.05) ==false)
            {
                _l->_guards[i]->_angle=angles[x];
            }

        }
        if(_l->_guards[i]->_angle>=90 && _l->_guards[i]->_angle<180 && _l->_guards[i]->mort==false)
        {
            if(_l->_guards[i]->move(-0.05,0.0)  )
            {




            }
            else if(_l->_guards[i]->move(-0.05,0.0) ==false)

                _l->_guards[i]->_angle=angles[x];


        }

        if(_l->_guards[i]->_angle>=270 && _l->_guards[i]->mort==false)
        {
            if(_l->_guards[i]->move(0.05,0))
            {}
            else if(_l->_guards[i]->move(0.05,0.0) ==false)
            {
                _l->_guards[i]->_angle=angles[x];
            }

        }

        if(_l->_guards[i]->_angle>=180 && _l->_guards[i]->_angle<270 && _l->_guards[i]->mort==false)
        {
            if(_l->_guards[i]->move(0,-0.05) )
            {



            }
            else if(_l->_guards[i]->move(0,-0.05) ==false)
            {
                _l->_guards[i]->_angle=angles[x];
            }

        }

    }
}
/*======================FIN ARDIEN========================*/





























/*======================CHASSEUR========================*/
Chasseur::Chasseur (Labyrinthe* l) : Mover (100, 80, l, 0)
{

    // initialise les sons.
    _hunter_fire = new Sound ("sons/hunter_fire.wav");
    _hunter_hit = new Sound ("sons/hunter_hit.wav");
    if (_wall_hit == 0)
        _wall_hit = new Sound ("sons/hit_wall.wav");
}
bool Chasseur::move_aux (double dx, double dy)
{
    if (EMPTY == _l -> data ((int)((_x + dx) / Environnement::scale),
                             (int)((_y + dy) / Environnement::scale)))
    {
        _x += dx;
        _y += dy;
        return true;

    }
    return false;
}

/*
 *  Fait bouger la boule de feu (ceci est une exemple, à vous de traiter les collisions spécifiques...)
 */
int cT=0; /* compteur pour tresor  */

bool Chasseur::process_fireball (float dx, float dy)
{
    // on bouge que dans le vide!
    if (EMPTY == _l -> data ((int)((_fb -> get_x () + dx) / Environnement::scale),
                             (int)((_fb -> get_y () + dy) / Environnement::scale)))
    {

        return true;
    }
    float   dmax2 = (_l -> width ())*(_l -> width ()) + (_l -> height ())*(_l -> height ());
    _wall_hit -> play (1. - 1/dmax2);
    for (int i =1 ; i<=_l ->_nguards; i++)
    {
        float   x1 = ( _fb -> get_x ()-_l -> _guards [i]-> _x) / Environnement::scale;
        float   y1 = (_fb -> get_y ()-_l -> _guards [i]-> _y)/ Environnement::scale ;
        float   dista= x1*x1 + y1*y1;                            /*distance entre le gardien et la fb */


        // on bouge que dans le vide!
        if (EMPTY == _l -> data ((int)((_fb -> get_x () + dx) / Environnement::scale),
                                 (int)((_fb -> get_y () + dy) / Environnement::scale)))
        {


        }

        if (  _l ->_guards [i]->mort==false && dista<(float) 1)
        {
            if(_l ->_guards [i]-> vie>=0 )
            {
                _l ->_guards [i]-> vie-=_l->_guards[0]->force;
                message ("======(gard %d ,vie %d)====",i,_l -> _guards [i]-> vie);

            }

        }
        if(_l ->_guards [i]-> vie==0)   
        {
            _l ->_guards [i]-> rester_au_sol ();
            _l ->_guards [i]->mort=true;

        }



    }
    float   x = (_fb-> get_x()-_l->_treasor._x*10) / Environnement::scale;
    float   y = (_fb-> get_y()- _l->_treasor._y*10)/ Environnement::scale ;
    float   dT= x*x + y*y; /*distance entre la fb  et le tresor */
    if(dT<3.0)
    {
        cT++;
        message ("====== Encore  %d ====",(10-cT));
        if(cT==10)
        {
            message ("====== Tresor ouvert=====");
            partie_terminee(true);
        }
    }
    return false;

}

/*
 *  Tire sur un ennemi.
 */

void Chasseur::fire (int angle_vertical)
{
    _hunter_fire -> play ();
    _fb -> init (/* position initiale de la boule */ _x, _y, 10.,
            /* angles de visée */ angle_vertical, _angle);
}






/*
 *  Clic droit: par défaut fait tomber le premier gardien.
 *
 *  Inutile dans le vrai jeu, mais c'est juste pour montrer
 *  une utilisation des fonctions « tomber » et « rester_au_sol »
 */



void Chasseur::right_click (bool shift, bool control)
{


    if(shift)
    {


    }               /* teleportation */
    float   x = (_l->_guards[0]->_x-_l->_marks[0]._x*10) / Environnement::scale;
    float   y = (_l->_guards[0]->_y-_l->_marks[0]._y*10)/ Environnement::scale ;
    float   dm= x*x + y*y; /*distance entrec chasseur et marque de teleportation */
    if(dm<1)
    {
        _l->_guards[0]->_x=_l->_marks[1]._x*10;
        _l->_guards[0]->_y=_l->_marks[1]._y*10;

    }
}
/*======================FIN CHASSEUR========================*/
