#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <stack>

#include "point.h"
#include "graphics.h"
#include "affichage.h"
#include "parser.h"

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
GLfloat vertices[];
//{ //     COORDINATES     /        COLORS      /   TexCoord  //
//	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
//	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
//	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
//	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
//	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
//};

// Indices for vertices order
GLuint indices[];
//{
//	0, 1, 2,
//	0, 2, 3,
//	0, 1, 4,
//	1, 2, 4,
//	2, 3, 4,
//	3, 0, 4
//};

// Range les points de gauche à droite et de haut en bas
bool sortPoints(Point a, Point b) {
    return (a.x() < b.x() ? true : (a.x() == b.x()) ? a.y() < b.y() : false);
}

Carte triangulation(const vector<Point>& T, Carte& C) {
    DemiCote* premierDemiCote;

    // Créer le premier coté
    if (T.at(1).y() < T.at(0).y()) {
        premierDemiCote = C.ajouteCote(T.at(0), T.at(1));
    }
    else {
        premierDemiCote = C.ajouteCote(T.at(1), T.at(0));
    }

    // Définir le demi cote particulier qui a l'exterieur sur sa gauche
    C.changeDemiCoteParticulier(premierDemiCote);

    // Parcours tous les points
    for (auto it = T.begin() + 2; it != T.end(); ++it) {

        DemiCote* nextDemiCote = premierDemiCote;

        // Premier demi cote du nouveau point
        DemiCote* demiCoteNouveauPoint = C.ajouteCote(*it, nextDemiCote->precedent());

        while (0 > (*it).aGauche(nextDemiCote->coordonnees(), nextDemiCote->oppose()->coordonnees())) {
            nextDemiCote = nextDemiCote->oppose()->suivant();
            C.ajouteCote(demiCoteNouveauPoint, nextDemiCote->precedent());

        }

        // Repartir du milieu dans l'autre sens
        nextDemiCote = premierDemiCote->precedent()->precedent();

        while (0 < (*it).aGauche(nextDemiCote->coordonnees(), nextDemiCote->oppose()->coordonnees())) {
            nextDemiCote = nextDemiCote->oppose()->precedent();
            demiCoteNouveauPoint = C.ajouteCote(demiCoteNouveauPoint, nextDemiCote);
        }

        premierDemiCote = demiCoteNouveauPoint->suivant();

        C.changeDemiCoteParticulier(premierDemiCote);
    }

    return C;
}

void delaunay(Carte& C) {
    // Marque l'enveloppe convexe pour ne pas la calculer dans l'algorithme flip
    auto currentDemiCote = C.demiCoteParticulier();
    do {
        currentDemiCote->changeMarque(1);
        currentDemiCote->oppose()->changeMarque(1);
        currentDemiCote = currentDemiCote->oppose()->suivant();
    } while (C.demiCoteParticulier() != currentDemiCote);


    stack<DemiCote*> pile;

    // Rempli la pile des demiCotes internes de la triangulation
    for (int i = 0; i < C.nbSommets(); i++) {
        auto demiCoteDuSommet = C.sommet(i)->demiCote();

        do {
            // Ignore la marque 1
            if (demiCoteDuSommet->marque() == 0) {
                pile.push(demiCoteDuSommet); // Ajoute le demiCote à la pile
                demiCoteDuSommet->changeMarque(1); // Marque 1 pour dire qu'on est déjà passé dessus
                demiCoteDuSommet->oppose()->changeMarque(1);
            }
            demiCoteDuSommet = demiCoteDuSommet->suivant();

        } while (demiCoteDuSommet != C.sommet(i)->demiCote());
    }

    // Parcourir tous les demiCote de la pile pour flip les triangles illégaux
    while (pile.size() > 0) {


        // Prendre le premier DemiCote de la pile
        auto premierDemiCote = pile.top();
        pile.pop();
        premierDemiCote->changeMarque(0);
        premierDemiCote->oppose()->changeMarque(0);

        auto a = premierDemiCote->coordonnees();
        auto b = premierDemiCote->oppose()->coordonnees();
        auto c = premierDemiCote->suivant()->oppose()->coordonnees();
        auto d = premierDemiCote->precedent()->oppose()->coordonnees();

        auto dansCercle = d.dansCercle(a, b, c);

        if (dansCercle > 0) {

            if (premierDemiCote->suivant()->oppose()->marque() == 0) {
                premierDemiCote->suivant()->changeMarque(1);
                premierDemiCote->suivant()->oppose()->changeMarque(1);
                pile.push(premierDemiCote->suivant()->oppose());
            }

            if (premierDemiCote->suivant()->oppose()->suivant()->marque() == 0) {
                premierDemiCote->suivant()->oppose()->suivant()->changeMarque(1);
                premierDemiCote->suivant()->oppose()->suivant()->oppose()->changeMarque(1);
                pile.push(premierDemiCote->suivant()->oppose()->suivant());
            }

            if (premierDemiCote->precedent()->oppose()->marque() == 0) {
                premierDemiCote->precedent()->changeMarque(1);
                premierDemiCote->precedent()->oppose()->changeMarque(1);
                pile.push(premierDemiCote->precedent()->oppose());
            }

            if (premierDemiCote->precedent()->oppose()->precedent()->marque() == 0) {
                premierDemiCote->precedent()->oppose()->precedent()->changeMarque(1);
                premierDemiCote->precedent()->oppose()->precedent()->oppose()->changeMarque(1);
                pile.push(premierDemiCote->precedent()->oppose()->precedent());
            }

            C.flip(premierDemiCote);
        }
    }
}

int main()
{
    const string cheminAccesTexte = ".\\ile Saint Christophe.txt";
    const string cheminAccesOBJ = ".\\resultat.obj";

    Carte C = Carte();
    vector<Point> nuagePoints;

    // Seed le random
    srand(time(0));

    // Parse le fichier texte en points
    Parser parser;
    nuagePoints = parser.texteEnPoints(cheminAccesTexte);

    // Supprime tous les points dupliqués
    std::sort(nuagePoints.begin(), nuagePoints.end(), sortPoints);
    nuagePoints.erase(unique(nuagePoints.begin(), nuagePoints.end()), nuagePoints.end());

	// Triangule
    C = triangulation(nuagePoints, C);
    delaunay(C);

	// Affiche tous les points sur la fenêtre
	// (agrandissement de la fenêtre pour que tous les points soient visibles)
	// opengraphsize(tailleDeFenetre + 50, tailleDeFenetre + 50);
    // trace(C);
    /*getch();
    closegraph();*/

	// Create file obj with triangulation
	parser.carteEnOBJ(C, cheminAccesOBJ);

	// Initialize indices and vertices with the triangulation
	std::tuple<vector<GLfloat>, vector<GLuint>> verticesIndices = parser.carteEnVerticesIndices(C);	 


	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "Ile Saint Christophe", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(&std::get<0>(verticesIndices)[0], std::get<0>(verticesIndices).size());
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(&std::get<1>(verticesIndices)[0], std::get<1>(verticesIndices).size());

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	std::string texturePath = "./brick.png";

	// Texture
	/*Texture brickTex((texturePath).c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);*/

	// Original code from the tutorial
	/*Texture brickTex("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);*/

	// Variables that help the rotation of the pyramid
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Simple timer
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = crntTime;
		}

		// Initializes matrices so they are not the null matrix
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		// Assigns different transformations to each matrix
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

		// Outputs the matrices into the Vertex Shader
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(uniID, 0.5f);
		// Binds texture so that is appears in rendering
		// brickTex.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, std::get<1>(verticesIndices).size() / sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	// brickTex.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}