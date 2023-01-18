#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stack>

// Function to print the
// index of an element
int Parser::getIndex(vector<Point> v, Point K)
{
    auto it = find(v.begin(), v.end(), K);

    return it - v.begin() + 1;
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
                s.append(std::to_string(getIndex(sommets, a)));
                s.append(" ");
                s.append(std::to_string(getIndex(sommets, b)));
                s.append(" ");
                s.append(std::to_string(getIndex(sommets, c)));
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

    for (size_t i = 0; i < C.nbSommets(); i++)
    {
        auto sommet = C.sommet(i);
        sommets.push_back(sommet->coordonnees());

        if (sommet->coordonnees().x() < 1 && sommet->coordonnees().y() < 1) {
            auto test = "";
        }
        
        vertices.insert(vertices.end(), { (float)sommet->coordonnees().x(), (float)sommet->coordonnees().z(), (float)sommet->coordonnees().y(), 0.83f, 0.70f, 0.44f, 0.0f, 0.0f });

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
                
                if (getIndex(sommets, a) < 0 || getIndex(sommets, b) < 0) {
                    auto test = "";
                }

                indices.insert(indices.end(), { (unsigned int)getIndex(sommets, a), (unsigned int)getIndex(sommets, b), (unsigned int)getIndex(sommets, c) });
            }
            demiCoteDuSommet = demiCoteDuSommet->suivant();

        } while (demiCoteDuSommet != C.sommet(i)->demiCote());
    }

    //vertices = { //     COORDINATES     /        COLORS      /   TexCoord  //
    //        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    //        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
    //         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    //         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
    //         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
    //};

    //indices = {
    //    0, 1, 2,
    //    0, 2, 3,
    //    0, 1, 4,
    //    1, 2, 4,
    //    2, 3, 4,
    //    3, 0, 4
    //};
        
    return std::tuple<vector<GLfloat>, vector<GLuint>>(vertices, indices);
    
}