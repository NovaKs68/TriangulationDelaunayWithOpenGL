#include "affichage.h"
#include "point.h"
#include "carte.h"
#include "graphics.h"

// trace un segment entre deux points a et b
void segment(const Point &a, const Point &b)
{
	line(a.x(), tailleDeFenetre - a.y(), b.x(), tailleDeFenetre - b.y());
}

// trace la carte C dans une fenêtre graphique
void trace(const Carte &C)
{
    for (size_t i = 0; i < C.nbDemiCotes(); i++)
    {
        auto demiCoteATrace = C.demiCote(i);

        if (demiCoteATrace->index() < demiCoteATrace->oppose()->index()) {
            segment(demiCoteATrace->coordonnees(), demiCoteATrace->oppose()->coordonnees());
        }
    }
    
}


