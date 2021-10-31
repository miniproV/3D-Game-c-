
#ifndef GARDIEN_H
#define GARDIEN_H

#include "Mover.h"
#include <unistd.h>
#include <iostream>
#include <cstdlib>
class Labyrinthe;

class Gardien : public Mover
{
private:
    bool move_g (double dx, double dy);
    void deplacement ();
    void dep_g(int i,int x, int x1);
    void attack();
public:
    const char* modele;
    Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele)
    {}
    void update (void)
    {
        deplacement();
        attack();
    }
    void upv (int) {};
    bool move(double dx, double dy)
    {
        return move_g (dx, dy);
    }
    void fire (int angle_vertical);
    bool process_fireball (float dx, float dy);
};

#endif
