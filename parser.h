#pragma once
#ifndef PARSER_H
#define PARSER_H

#include "point.h"
#include "carte.h"
#include <string>
#include <vector>
#include<glad/glad.h>
#include <tuple>

static class Parser
{

public:
	// Converti un fichier texte en points x y z
	std::vector<Point> texteEnPoints(const std::string cheminAccesTexte) const;
	// Converti une carte en fichier OBJ
	void carteEnOBJ(const Carte C, const std::string cheminAccesOBJ);

	std::tuple<vector<GLfloat>, vector<GLuint>> carteEnVerticesIndices(const Carte C);

private:
	int getIndex(vector<Point> v, Point K);
};

#endif
