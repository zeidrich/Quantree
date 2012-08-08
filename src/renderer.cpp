#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "OBJ.h"
#include "drawing.h"
#include "imageloader.h"
#include "window.h"
#include "gamestructs.h"
#include "map.h"
#include "renderer.h"
#include "strings.h"
#include "errors.h"
#include <cmath>
#include <vector>
#include <ctime>

Renderer* Renderer::currentRenderer;

Renderer::Renderer(int argc, char** argv)
{
	this->initialize(argc, argv);
	this->setEnables();
	if (Renderer::currentRenderer == 0)
		currentRenderer = this;
}
Renderer::~Renderer()
{
}
int Renderer::initialize(int argc, char** argv)
///////////
// Glut Initialization, Create Window, set size
{
	glutInit(&argc, argv);  //Initialization - Called once.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //Displaymode Init - Called Once
	glutInitWindowSize(800, 600);  //Initial Window Size -- Could be modified to use a define or something.
	Window *hWindow = Window::getCurrentWindow();
	if (hWindow == 0)
	{
		ErrorManager::displayError(1, "Renderer::initialize - No Current Window!");
		return 0;
	}
	else
	{
		hWindow->setHeight(600);  //Initialize Window Object Size - Should use variable
		hWindow->setWidth(800);  // Above
	}
	glutCreateWindow(STR_PROGRAM_TITLE); //Create window and name.
	glewInit();  // Initializing
	return 1;
}
int Renderer::setEnables()
{
	int result = 1;
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	
	return result;
}
int Renderer::loadTexture(char* filename)
{
	GLuint textureId;
	int result = 1;
	Image* image = loadBMP(filename);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image->width, image->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image->pixels);
	delete image;
	this->textures.push_back(textureId);
	return result;
}

GLuint Renderer::loadTextureFromMap(Map* map)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	if (map->topography == 0)
		map->generateTopography();
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 map->width, map->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 map->topography);
	this->textures.push_back(textureId);
	return textureId;
}

GLuint Renderer::getTexture(int index)
{
	return this->textures[index];
}

GLuint Renderer::getLastTexture()
{
	return this->textures.back();
}

Renderer* Renderer::getCurrentRenderer()
{
	return currentRenderer;
}