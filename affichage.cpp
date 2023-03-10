#include "affichage.h"
#include "point.h"
#include "carte.h"
#include "graphics.h"

// trace un segment entre deux points a et b
void segment(const Point &a, const Point &b)
{
	line(a.x() / 35, tailleDeFenetre - a.y() / 35, b.x() / 35, tailleDeFenetre - b.y() / 35);
}

// trace la carte C dans une fen?tre graphique
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


