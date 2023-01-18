#ifndef POINT_H
#define POINT_H

class Point
{

public:
	// constructeurs
	Point() : d_x{ 0 }, d_y{ 0 }, d_z{ 0 } {}
	Point(int x, int y, int z) : d_x{ x }, d_y{ y }, d_z{ z } {}
	// accesseurs
	int x() const {return d_x;}
	int y() const {return d_y;}
	int z() const {return d_z;}

	// renvoie 1, -1 ou 0 selon que le point auquel la m�thode est appliqu�e est
	// � gauche de, � droite de, ou sur la droite (ab) orient�e de a vers b.
	int aGauche(const Point &a, const Point &b) const;

	int dansCercle(const Point& a, const Point& b, const Point& c) const;

	// Compare les coordonn�es de deux points
	bool operator==(const Point&);
private:
    // coordonn�es
	int d_x, d_y, d_z;
};

#endif
