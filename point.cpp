#include "point.h"
#include <math.h>
#include <cmath>

// renvoie 1, -1 ou 0 selon que le point auquel la méthode est appliquée est
// à gauche de, à droite de, ou sur la droite (ab) orientée de a vers b.
int Point::aGauche(const Point &a, const Point &b) const
{
	return ((b.d_x - a.d_x) * (this->d_y - a.d_y)) - ((this->d_x - a.d_x) * (b.d_y - a.d_y));
}

// |a d g|
// |b e h|
// |c f i|
// 
//       |e h|     |d g|     |d g|
// = a * |f i| - b |f i| + c |e h|
// = e * i - h * f
// = d * i - g * f
// = d * h - g * e
// 
// = a * ((e * i - h * f) - b * (d * i - g * f) + c * (d * h - g * e))
//
// a = a.x() - this->x()
// d = a.y() - this->y()
// g = sqrt(a.x() - this->x()) + sqrt(a.y() - this->y())
// b = b.x() - this->x()
// e = b.y() - this->y()
// h = sqrt(b.x() - this->x()) + sqrt(b.y() - this->y())
// c = c.x() - this->x()
// f = c.y() - this->y()
// i = sqrt(c.x() - this->x()) + sqrt(c.y() - this->y())
//
// Retour 1 si dans le cercle, 0 sur le cercle et -1 hors du cercle
int Point::dansCercle(const Point& a, const Point& b, const Point& c) const
{
	float ap = a.x() - this->x();
	float d = a.y() - this->y();
	float g = pow(ap, 2) + pow(d, 2);
	float bp = b.x() - this->x();
	float e = b.y() - this->y();
	float h = pow(bp, 2) + pow(e, 2);
	float cp = c.x() - this->x();
	float f = c.y() - this->y();
	float i = pow(cp, 2) + pow(f, 2);

	// float res = ap * (e * i - h * f) - bp * (d * i - g * f) + cp * (d * h - g * e);
	float res = ap * (e * i - h * f) - d * (bp * i - h * cp) + g * (bp * f - e * cp);


	/*float premier = ((b.d_y - this->d_y) * (sqrt(abs(c.d_x - this->d_x)) + sqrt(abs(c.d_y - this->d_y))))
		- ((sqrt(abs(b.d_x - this->d_x)) + sqrt(abs(b.d_y - this->d_y))) * (c.d_y - this->d_y));

	float deuxieme = ((a.d_y - this->d_y) * (sqrt(abs(c.d_x - this->d_x)) + sqrt(abs(c.d_y - this->d_y))))
		- ((sqrt(abs(a.d_x - this->d_x)) + sqrt(abs(a.d_y - this->d_y))) * (c.d_y - this->d_y));

	float troisieme = ((a.d_y - this->d_y) * (sqrt(abs(b.d_x - this->d_x)) + sqrt(abs(b.d_y - this->d_y))))
		- ((sqrt(abs(a.d_x - this->d_x)) + sqrt(abs(a.d_y - this->d_y))) * (b.d_y - this->d_y));
	float petitUn = (a.d_x - this->d_x);

	float un = petitUn * premier;

	float deux = ((b.d_x - this->d_x) * deuxieme);

	float trois = ((c.d_x - this->d_x) * troisieme);

	float test = un - deux + trois;*/

	if (res < 0) {
		return -1;
	}
	else if (res > 0) {
		return 1;
	}
	else {
		return 0;
	}
}

// Compare les coordonnées de deux points
bool Point::operator==(const Point& comparaisonPoint)
{
	return (*this).d_x == comparaisonPoint.x() && (*this).d_y == comparaisonPoint.y();
}