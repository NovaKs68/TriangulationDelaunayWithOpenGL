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

	// renvoie 1, -1 ou 0 selon que le point auquel la méthode est appliquée est
	// à gauche de, à droite de, ou sur la droite (ab) orientée de a vers b.
	int aGauche(const Point &a, const Point &b) const;

	int dansCercle(const Point& a, const Point& b, const Point& c) const;

	// Compare les coordonnées de deux points
	bool operator==(const Point&);
private:
    // coordonnées
	int d_x, d_y, d_z;
};

#endif
