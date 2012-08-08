#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include "gamestructs.h"
#include "gamemath.h"
#include "structures.h"
#include <vector>

// Renders Display
class Renderer {
	public:
		Renderer(int argc, char** argv);
		~Renderer();
		int loadTexture(char* filename); //Load texture from file.
		GLuint loadTextureFromMap(Map* map);
		GLuint Renderer::getTexture(int index);
		GLuint Renderer::getLastTexture();

		static Renderer* getCurrentRenderer();
	private:
		static Renderer* currentRenderer;

		std::vector<GLuint> textures;
		
		int initialize(int argc, char** argv);
		int setEnables();
};

#endif