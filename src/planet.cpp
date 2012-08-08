#include "planet.h"
#include "drawing.h"
#include "renderer.h"

Planet::Planet()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

GLuint Planet::getBuffer() //Get buffer ID for planet model.
{
	if (glIsBuffer(this->buffer)){
		return this->buffer;
	}
	else {
		return this->createBuffer();
	}
		
}
GLuint Planet::createBuffer() //Create and return new Buffer for planet model.
{
	glGenBuffers(1, &(this->buffer));
	return this->buffer;
}
char Planet::generateBuffer() //Generate vertex data in planet buffer.
{
	generatePlanetMesh(this);
	return 0;
}


char Planet::assignMapgen(Mapgen* mapgen, unsigned char mask) //Assign maps to planet for population. Mask indicates sides to apply map to.
{
	mapgen;

	mask = mask & ~mappedmask;
	mappedmask = mappedmask | mask;
	
	// Currently assigns a single map 16x16 to world, ignoring masks.
	//TODO: Assign map to sides according to mask
	//TODO: Identify connecting sides and make large map for them.  Assign submaps to sides.

	mapgen = new Mapgen(MAP_WIDTH*1, MAP_HEIGHT*1,2,1);
	mapgenlist.push_back(mapgen);

	/*for (char c = 0; c < 8; c++)
	{
		unsigned char tempMask = 1 << c;
		if (tempMask & mappedmask)
		{
			mapgen = new Mapgen(MAP_WIDTH*1, MAP_HEIGHT*1,2,0);
			maplist.push_back(mapgen);
		}
	}*/
	
	/*	
	for each unconnected part of mask
		mapgen = new Mapgen(width*run, height*rise)
		if the mapgen is larger than 1 side
			split into singles 
			add each mapgen to list
		else
			add mapgen to list of mapgens
		end if
	end for
	return succes/fail.

	*/

	return 0;
}
char Planet::generateMaps(unsigned char mask) //Generate sumbaps based on current Mapgen assignements.  Mask indicates sides to apply.
{

	mask;
	Renderer* currentRenderer = Renderer::getCurrentRenderer();
	Map* newMap = new Map(MAP_HEIGHT,MAP_WIDTH,MAP_DEPTH);

	Mapgen* currentMapgen = this->mapgenlist.back();
	currentMapgen->generateMap();
	newMap->Clear();
	mapgenTile* currentTileArray = 0;
	currentTileArray = currentMapgen->getSubmap(MAP_HEIGHT,MAP_WIDTH,1,0,0,0);
	
	newMap->loadMap(0,currentTileArray,0,0,0);
	newMap->CreateBuffer();
	generateMapMesh(newMap);
	newMap->UpdateBuffer();
	
	newMap->generateTopography();
	
	GLuint texture = currentRenderer->loadTextureFromMap(newMap);
	texture;
	
	this->maplist.push_back(newMap);

	/*
	for each side in mask
		generate map for side based on mapgen of side		
	*/
	return 0;
}

Planet* Planet::getPlanetFromMap(Map *map)
{
	Planet *returnPlanet = 0;
	map;
	/*
	check map for parent;
	if parent is a planet return it.  If parent is a map, check it for a parent. 
	do some some sort of break if it does too many checks.
	*/
	return returnPlanet;
}

Map *Planet::getMapFromPlanet(Planet *planet, char side)
{
	//Return the map on the specified side.
	//TODO: Return map on the specified side.
	//TODO: Input Validation
	side;
	planet;
	Map *returnMap = 0;
	
	return returnMap;
}


void* Planet::getPlanetAttrib(char* attrib) //Get attribute data about planet (mass, orbit, neighbours etc).
{
	void* returnAttrib = 0;
	attrib;
	/*
	Return value associated with the attrib. 
	*/
	return returnAttrib;
}

char Planet::setPlanetAttrib(char* attrib, void* value) //Set attribute, return error code
{
	attrib;
	value;
	/*
	Set value associated with attrib to value.
	Some sanity checking needed to make sure contents of value makes sense.
	*/
	return 0;
}


char Planet::setPosition(float x, float y, float z)
{
	char returnCode = 1;
	//TODO: Any boundary checking.
	this->x = x;
	this->y = y;
	this->z = z;
	return returnCode;
}