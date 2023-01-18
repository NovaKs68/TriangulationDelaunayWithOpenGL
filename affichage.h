#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "point.h"
#include "carte.h"

const int tailleDeFenetre = 800;

// trace un segment entre deux points a et b
void segment(const Point &a, const Point &b);

// trace la carte C dans une fenêtre graphique
void trace(const Carte &C);

#endif
