#include <vector>
using namespace std;

#include "carte.h"
#include "point.h"


// Destructeur
Carte::~Carte()
{
}

// Ajoute dans la carte un nouveau côté composé de deux demi-côtés dc1 et dc2
// dont precedent1 et precedent2 seront les demi-côtés précédents respectifs.
// On suppose precedent1 et precedent2 déjà présents dans la carte.
// Renvoie l'adresse de dc1.
DemiCote* Carte::ajouteCote(DemiCote* precedent1, DemiCote* precedent2)
{
	auto demiCote1 = ajouteDemiCote(precedent1);
	auto demiCote2 = ajouteDemiCote(precedent2, demiCote1);

	return demiCote1;
}

// Ajoute dans la carte un nouveau côté composé de deux demi-côtés dc1 et dc2.
// precedent1 sera le demi-côté précédent de dc1. dc2 sera issu d’un
// nouveau sommet (à créer) dont les coordonnées sont celles du point p2.
// On suppose precedent1 déjà présent dans la carte.
// Renvoie l'adresse de dc1.
DemiCote* Carte::ajouteCote(DemiCote* precedent1, const Point& p2)
{
	auto demiCote1 = ajouteDemiCote(precedent1);
	auto demiCote2 = ajouteDemiCote(p2, demiCote1);

	return demiCote1;
}

// Ajoute dans la carte un nouveau côté composé de deux demi-côtés dc1 et dc2.
// precedent2 sera le demi-côté précédent de dc2. dc1 sera issu d’un
// nouveau sommet (à créer) dont les coordonnées sont celles du point p1.
// On suppose precedent2 déjà présent dans la carte.
// Renvoie l'adresse de dc1.
DemiCote* Carte::ajouteCote(const Point& p1, DemiCote* precedent2)
{
	auto demiCote1 = ajouteDemiCote(p1);
	auto demiCote2 = ajouteDemiCote(precedent2, demiCote1);

	return demiCote1;
}

// Ajoute dans la carte un nouveau côté composé de deux demi-côtés dc1 et dc2.
// dc1 et dc2 seront issus de deux nouveaux sommets (à créer) dont les
// coordonnées sont celles des points p1 et p2 respectivement.
// Renvoie l'adresse de dc1.
DemiCote* Carte::ajouteCote(const Point& p1, const Point& p2)
{
	auto demiCote1 = ajouteDemiCote(p1);
	auto demiCote2 = ajouteDemiCote(p2, demiCote1);

	return demiCote1;
}

// Ajoute un nouveau demi-côté dans la carte dont precedent sera le demi-côté
// 	précédent et oppose sera le demi-côté opposé.
// On suppose que le demi-côté precedent est déjà présent dans la carte.
// Le demi-côté oppose est soit déjà présent dans la carte soit nul (valeur
// par défaut).
// Renvoie l’adresse du nouveau demi-côté.
DemiCote* Carte::ajouteDemiCote(DemiCote* precedent, DemiCote* oppose)
{
	// Creer le nouveau demi cote
	DemiCote* nouveauDemiCote = new DemiCote(
		precedent->suivant(),
		precedent,
		oppose,
		precedent->sommet(),
		nbDemiCotes());

	// Modifier les demi cotes voisins
	precedent->suivant()->d_precedent = nouveauDemiCote;
	precedent->d_suivant = nouveauDemiCote;

	// Modifier le demi cote opposé
	if (oppose != nullptr) {
		oppose->d_oppose = nouveauDemiCote;
	}

	d_tabDemiCotes.push_back(nouveauDemiCote);

	return nouveauDemiCote;
}

// Ajoute un nouveau demi-côté dans la carte qui sera issu d’un nouveau
// sommet (à créer) dont les coordonnées sont celles du point p.
// oppose sera le demi-côté opposé du nouveau demi-côté.
// oppose est soit déjà présent dans la carte soit nul (valeur par défaut).
// Renvoie l’adresse du nouveau demi-côté.
DemiCote* Carte::ajouteDemiCote(const Point &p, DemiCote* oppose)
{
	// Creer le nouveau demi cote
	DemiCote* nouveauDemiCote = new DemiCote(
		NULL,
		NULL,
		oppose,
		NULL,
		nbDemiCotes());
	d_tabDemiCotes.push_back(nouveauDemiCote);

	// Creer le nouveau sommet
	Sommet* nouveauSommet = new Sommet(p, nouveauDemiCote, nbSommets());
	d_tabSommets.push_back(nouveauSommet);

	// Remplir le demi cote
	nouveauDemiCote->d_precedent = nouveauDemiCote;
	nouveauDemiCote->d_suivant = nouveauDemiCote;
	nouveauDemiCote->d_sommet = nouveauSommet;

	// Modifier le demi cote opposé
	if (oppose != nullptr) {
		oppose->d_oppose = nouveauDemiCote;
	}

	return nouveauDemiCote;
}

// Inverser le tracé entre le demi coté d et son opposé avec 
void Carte::flip(DemiCote *d) 
{
	flipDemiCote(d);
	flipDemiCote(d->oppose());
}

void Carte::flipDemiCote(DemiCote* d) {
	// Defini un marqueur de la ou va se trouver le demi cote
	auto demiCoteDepart = d->suivant()->oppose();

	// Changer le demi cote représentant le sommet
	if (d->d_sommet->demiCote() == d) {
		d->d_sommet->d_demiCote = d->suivant();
	}

	// Changer les anciens voisins de d
	d->suivant()->d_precedent = d->precedent();
	d->precedent()->d_suivant = d->suivant();

	// Deplacer d
	d->d_precedent = demiCoteDepart;
	d->d_suivant = demiCoteDepart->suivant();

	// Changer les nouveaux voisins de d
	d->precedent()->d_suivant = d;
	d->suivant()->d_precedent = d;

	// Mettre à jour le sommet de d
	d->d_sommet = demiCoteDepart->sommet();
}
