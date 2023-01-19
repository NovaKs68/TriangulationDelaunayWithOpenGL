#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

// Function to print the
// index of an element
int Parser::getIndex(vector<Point> v, Point K)
{
    auto it = find(v.begin(), v.end(), K);

    return it - v.begin();
}

std::vector<Point> Parser::texteEnPoints(const std::string cheminAccesTexte) const
{
    vector<Point> nuagePoints;

    // Lis le fichier ligne par ligne
    std::ifstream file(cheminAccesTexte);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // Parse la ligne en trois coordonnées x y z
            std::stringstream ss(line.c_str());
            std::string s;
            int coordonnes[3] = {};
            int index = 0;
            while (std::getline(ss, s, ' ')) {
                coordonnes[index] = std::stoi(s);
                index++;
            }

            // Créer le point
            nuagePoints.push_back(Point(coordonnes[0], coordonnes[1], coordonnes[2]));
        }
        file.close();
    }

	return nuagePoints;
}

void Parser::carteEnOBJ(const Carte C, const std::string cheminAccesOBJ) {
    ofstream myfile(cheminAccesOBJ);

    vector<Point> sommets;
    for (size_t i = 0; i < C.nbSommets(); i++)
    {
        auto sommet = C.sommet(i);
        sommets.push_back(sommet->coordonnees());

        std::string s;
        s.append("v ");
        s.append(std::to_string(sommet->coordonnees().x()));
        s.append(" ");
        s.append(std::to_string(sommet->coordonnees().y()));
        s.append(" ");
        s.append(std::to_string(sommet->coordonnees().z()));
        myfile << s << endl;

    }

    // Marque l'enveloppe convexe
    auto currentDemiCote = C.demiCoteParticulier();
    do {
        currentDemiCote->changeMarque(2);
        currentDemiCote->oppose()->changeMarque(2);
        currentDemiCote = currentDemiCote->oppose()->suivant();
    } while (C.demiCoteParticulier() != currentDemiCote);


    for (int i = 0; i < C.nbSommets(); i++) {
        auto demiCoteDuSommet = C.sommet(i)->demiCote();
        // Ignore l'enveloppe
        do {
            if (demiCoteDuSommet->marque() < 2) {

                // Créer le triangle
                auto a = demiCoteDuSommet->coordonnees();
                auto b = demiCoteDuSommet->oppose()->coordonnees();
                auto c = demiCoteDuSommet->suivant()->oppose()->coordonnees();

                std::string s;
                s.append("f ");
                s.append(std::to_string(getIndex(sommets, a) + 1));
                s.append(" ");
                s.append(std::to_string(getIndex(sommets, b) + 1));
                s.append(" ");
                s.append(std::to_string(getIndex(sommets, c) + 1));
                myfile << s << endl;
            }
            demiCoteDuSommet = demiCoteDuSommet->suivant();

        } while (demiCoteDuSommet != C.sommet(i)->demiCote());
    }

    myfile.close();
}

std::tuple<vector<GLfloat>, vector<GLuint>> Parser::carteEnVerticesIndices(const Carte C) {
    vector<GLfloat> vertices;
    vector<GLuint> indices;
    vector<Point> sommets;

    int maxXYValue = 0;
    int maxZValue = 0;

    // Calcul max value for each dimension
    for (int i = 0; i < C.nbSommets(); i++) {
        auto sommet = C.sommet(i);
        auto max = std::max(sommet->coordonnees().x(), sommet->coordonnees().y());
        
        if (maxXYValue < max) {
            maxXYValue = max;
        }

        if (maxZValue < sommet->coordonnees().z()) {
            maxZValue = sommet->coordonnees().z();
        }
    }

    for (size_t i = 0; i < C.nbSommets(); i++)
    {
        auto sommet = C.sommet(i);
        sommets.push_back(sommet->coordonnees());

        // Light blue for the top of the ocean and grey with white on the top of the montaine
        float colorR = 255 / (float)(maxZValue - sommet->coordonnees().z());
        float colorG = 255 / (float)(maxZValue - sommet->coordonnees().z());
        float colorB = 255 / (float)(maxZValue - sommet->coordonnees().z());
        if (sommet->coordonnees().z() < 50) { // Dark blue in the bottom of the ocean
            colorB = 255;
        }
        else if (sommet->coordonnees().z() >= 50 && sommet->coordonnees().z() < 250) { // Display green in the middle of ilands
            colorG = (255 * (1 - (float)(sommet->coordonnees().z() / maxZValue)));
            float colorR = 255;
            float colorB = 255;
        }
        //else if (sommet->coordonnees().z() == maxZValue) { // Display white for the last triangle
        //    float colorR = 255;
        //    float colorG = 255;
        //    float colorB = 255;
        //}
        

        vertices.insert(vertices.end(), { (float)sommet->coordonnees().x() / maxXYValue, (float)sommet->coordonnees().y() / maxXYValue, (float)sommet->coordonnees().z() / maxXYValue, colorR, colorG, colorB });
    }

    // Marque l'enveloppe convexe
    auto currentDemiCote = C.demiCoteParticulier();
    do {
        currentDemiCote->changeMarque(3);
        currentDemiCote->oppose()->changeMarque(3);
        currentDemiCote = currentDemiCote->oppose()->suivant();
    } while (C.demiCoteParticulier() != currentDemiCote);

    for (int i = 0; i < C.nbSommets(); i++) {
        auto demiCoteDuSommet = C.sommet(i)->demiCote();
        // Ignore l'enveloppe
        do {
            if (demiCoteDuSommet->marque() < 3) {

                // Créer le triangle
                auto a = demiCoteDuSommet->coordonnees();
                auto b = demiCoteDuSommet->oppose()->coordonnees();
                auto c = demiCoteDuSommet->suivant()->oppose()->coordonnees();

                indices.insert(indices.end(), { (unsigned int)getIndex(sommets, a), (unsigned int)getIndex(sommets, b), (unsigned int)getIndex(sommets, c) });
            }
            demiCoteDuSommet = demiCoteDuSommet->suivant();

        } while (demiCoteDuSommet != C.sommet(i)->demiCote());
    }
        
    return std::tuple<vector<GLfloat>, vector<GLuint>>(vertices, indices);
}