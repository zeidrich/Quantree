#ifndef PLANET_H_INC
#define PLANET_H_INC

#include "globals.h"
#include "gamestructs.h"
#include "mapgen.h"
#include "map.h"
#include "mobile.h"
#include <vector>

#define PLANET_TOP 0x01
#define PLANET_BOTTOM 0x02
#define PLANET_LEFT 0x04
#define PLANET_RIGHT 0x08
#define PLANET_FRONT 0x10
#define PLANET_BACK 0x20

class Planet
{
	public:
		Planet();
		GLuint getBuffer(); //Get buffer ID for planet model.
		GLuint createBuffer(); //Create a new Buffer for planet model.
		char generateBuffer(); //Generate vertex data in planet buffer.
		
		char assignMapgen(Mapgen* mapgen, unsigned char mask); //Assign maps to planet for population. Mask indicates sides to apply map to.
		char generateMaps(unsigned char mask); //Generate sumbaps based on current Mapgen assignements.  Mask indicates sides to apply.
		
		Planet *getPlanetFromMap(Map *map);
		Map *getMapFromPlanet(Planet *planet, char side);

		void* getPlanetAttrib(char* attrib); //Get attribute data about planet (mass, orbit, neighbours etc).
		char setPlanetAttrib(char* attrib, void* value); //Set attribute, return error code

		std::vector<vertex_t> *vertex_data;
		std::vector<vertex_t> *tempVertices;

		char addObject(Mobile *mob);
		char addObject(std::vector<Mobile *> *mobvector);

		char setPosition(float x, float y, float z);

		float x,y,z;

	private:
		GLuint buffer;
		char mappedmask;
		std::vector<Mapgen*> mapgenlist;
		std::vector<Map*> maplist;
};

#endif