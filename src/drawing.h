#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

#include "OBJ.h"
#include "imageloader.h"
#include "gamestructs.h"
#include "map.h"
#include "planet.h"
#include <vector>

void drawTile(float x, float y, float z, float scale, int tile, float mx, float my, float mz, Image *image);
void drawSquare(float x1, float y1, float x2, float y2);
void drawCube(float x, float y, float z, float scale);
void drawOcto(float x, float y, float z, float scale);
void drawPoint(float x, float y, float z, float scale, float mx, float my, float mz, Image *image);
void drawPyramid(float x, float y, float z);
void drawMesh(float x, float y, float z, float angle, ObjMesh* oMesh);
void drawBox(float x, float y, Image* image);
void drawGlyph(float x, float y, char c);
void drawSelector(gamePointi Selection);
void drawSelector(gamePointf Selection);

void pushCube(float x, float y, float z, float scale, float r, float g, float b, float a, std::vector<float> *vertices, std::vector<unsigned int> *indices, std::vector<float> *textures, std::vector<float> *normals, std::vector<float> *colors);
void pushEdge(int edge, float x, float y, float z, float scale, float r, float g, float b, float a, std::vector<float> *vertices, std::vector<unsigned int> *indices, std::vector<float> *textures, std::vector<float> *normals, std::vector<float> *colors);
void pushEdge(int edge, vertex_t vertex, std::vector<vertex_t> *vertices);

void drawMap(Map* map);
void drawMap(Map* map, float x, float y, float z, float scale);

void generatePlanetMesh(Planet *planet);
void generateMapMesh(Map *map);
void generateMapMesh2(Map *map);
#endif