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
#include "planet.h"
#include <cmath>
#include <vector>
#include <ctime>



void drawTile(float x, float y, float z, float scale, int tile, float mx, float my, float mz, Image *image)
{
	UNREFERENCED_PARAMETER(image);
	UNREFERENCED_PARAMETER(mx);
	UNREFERENCED_PARAMETER(my);
	UNREFERENCED_PARAMETER(mx);
	UNREFERENCED_PARAMETER(mz);

	switch (tile) {

		case 1:
			drawCube(x,y,z,scale);
			//drawOcto(x,y,z,scale);
			//drawPoint(x,y,z,scale, mx, my, mz, image);
			break;
		case 2:
			drawPyramid(x,y,z);
			break;

	}



}

void drawCubeOld(float x, float y, float z)
{
x = x - 0.5f;
y = y - 0.5f;
z = z - 0.5f;
			glBegin(GL_QUADS);

			//top
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f);
			
			glVertex3f(x, y, z);
			glTexCoord2f(1.0f, 0.0f);
			
			glVertex3f(x+1, y, z);
			glTexCoord2f(1.0f, 1.0f);
			
			glVertex3f(x+1, y+1, z);
			glTexCoord2f(0.0f, 1.0f);
			
			glVertex3f(x, y+1, z);

			//bottom
			glNormal3f(0.0f, 0.0f, -1.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x, y, z-1);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x+1, y, z-1);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x+1, y+1, z-1);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x, y+1, z-1);

			//front
			glNormal3f(0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f(x, y, z);
			glTexCoord2f(-1.0f, 0.0f);
			
			glVertex3f(x+1, y, z);
			glTexCoord2f(-1.0f, -1.0f);
			
			glVertex3f(x+1, y, z-1);
			glTexCoord2f(0.0f, -1.0f);
			
			glVertex3f(x, y, z-1);

			//back
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f(x, y+1, z);
			glTexCoord2f(-1.0f, 0.0f);
			
			glVertex3f(x+1, y+1, z);
			glTexCoord2f(-1.0f, -1.0f);
			
			glVertex3f(x+1, y+1, z-1);
			glTexCoord2f(0.0f, -1.0f);
			
			glVertex3f(x, y+1, z-1);

			//left
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f(x, y, z );
			glTexCoord2f(-1.0f, 0.0f);
			
			glVertex3f(x, y+1, z);
			glTexCoord2f(-1.0f, -1.0f);
			
			glVertex3f(x, y+1, z-1);
			glTexCoord2f(0.0f, -1.0f);
			
			glVertex3f(x, y, z-1);

			//right
			glNormal3f(+1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f(x+1, y, z);
			glTexCoord2f(-1.0f, 0.0f);
			
			glVertex3f(x+1, y+1, z);
			glTexCoord2f(-1.0f, -1.0f);
			
			glVertex3f(x+1, y+1, z-1);
			glTexCoord2f(0.0f, -1.0f);
			
			glVertex3f(x+1, y, z-1);

			glEnd();
}

inline void pushEdgeTextureMap(vertex_t &temp, vertex_t &bias, int x, int y)
{
		bias.textures[0] = bias.position[x] - temp.position[x] + 0.5f;
		//bias.textures[0] -= -1;
		//if (bias.textures[0] < -0.999f) bias.textures[0] = -0.999f;
		//if (bias.textures[0] > -0.001f) bias.textures[0] = -0.001f;
		//bias.textures[0] = bias.textures[0] < 1.0f ? 1.0f : bias.textures[0] > 0.0f ? 0.0f : bias.textures[0];
		bias.textures[1] = bias.position[y] - temp.position[y] + 0.5f;
		//bias.textures[1] -= -1;
		//if (bias.textures[1] < -0.999f) bias.textures[1] = -0.999f;
		//if (bias.textures[1] > -0.001f) bias.textures[1] = -0.001f;
		//bias.textures[1] = bias.textures[1] < 0 ? 0 : bias.textures[1];// > 1.0f ? 1.0f : bias.textures[1];
}

void pushEdge(int edge, vertex_t vertex, std::vector<vertex_t> *vertices)
{

	float radius = 0.5f;
	float sbulge = radius*0.38f;//072f;
	float lbulge = sbulge*1.32f;//0.237f;
	
	float rise = 0.31f;//0.118f;
	float run = radius-rise;
	float height = 0.237f;
	float scale = 0.5f;
	vertex_t temp = vertex;
	vertex_t bias = vertex;

		temp.textures[0] = 0;
		temp.textures[1] = 0;
		temp.textures[2] = 0;
		temp.textures[3] = 0;

	int x = 0;
		int y = 1;
		int z = 2;
		float xrun = radius;
		float yrun = radius;
		float zheight = lbulge;
		float srise = rise;//rise/2;
		float xrise = srise;
		float yrise = srise;

	switch (edge)
	{
		case 1:
			scale = 0 - scale;
			zheight = 0 - height;
		case 0:
			x = 0;
			y = 1;
			z = 2;
			break;
		case 5:
			scale = 0 - scale;
		case 4:
			y = 2;
			x = 0;
			z = 1;
			break;
		case 3:
			scale = 0 - scale;
		case 2:
			x = 1;
			y = 2;
			z = 0;
			break;
		default:
			return;
	}
	
		temp.position[z] += scale;//+= scale > 0 ? scale + lbulge + sbulge: scale - (lbulge + sbulge); // 0.5f + 0.237f + 0.072f = 0.809f == ~ 1/2(sqrt(5)/2+0.5)
		
		float zlbulge;
		float zsbulge;
		float ysbulge;
		float xsbulge;
		float xsign;
		float ysign;
		float zsign; 

		for (int c = 0; c <= 4; c++)
		{
		xsign = c & 1 ? -1 : 1;
		ysign = c & 2 ? -1 : 1;
		//zsign = scale > 0 ? 1 : -1;
		zsign = 1;

		if (scale < 0)
		{
			//xsign *= -1;
			//ysign *= -1;
			zsign *= -1;
		}
			

		xrun = c & 1 ? run * -1 : run;
		yrun = c & 2 ? run * -1 : run;
		xrise = c & 1 ? srise * -1 : srise;
		yrise = c & 2 ? srise * -1 : srise;

		zlbulge = scale > 0 ? lbulge : -lbulge;
		zsbulge = scale > 0 ? sbulge : -sbulge;
		ysbulge = yrun > 0 ? sbulge : -sbulge;
		xsbulge = xrun > 0 ? sbulge : -sbulge;

		bias.position[x] = temp.position[x] + 0;
		bias.position[y] = temp.position[y] + 0;
		bias.position[z] = temp.position[z] + zlbulge;
		bias.normal[x] = 0;
		bias.normal[y] = 0;
		bias.normal[z] = zsign * 1.0f;
		bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);
		
		bias.position[x] = temp.position[x] + 0;
		bias.position[y] = temp.position[y] + yrise;
		bias.position[z] = temp.position[z] + zsbulge;
		bias.normal[x] = 0;
		bias.normal[y] = ysign * 0.42;
		bias.normal[z] = zsign * 0.9f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);
		
		bias.position[x] = temp.position[x] + xrise;
		bias.position[y] = temp.position[y] + 0;
		bias.position[z] = temp.position[z] + zsbulge;
		bias.normal[x] = xsign * 0.42;
		bias.normal[y] = 0;
		bias.normal[z] = zsign * 0.9f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);



		bias.position[x] = temp.position[x] + 0;
		bias.position[y] = temp.position[y] + yrise;
		bias.position[z] = temp.position[z] + zsbulge;
		bias.normal[x] = 0;
		bias.normal[y] = ysign * 0.42;
		bias.normal[z] = zsign * 0.9f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		bias.textures[0] = bias.position[x] - temp.position[x] - 0.5f;
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);
		
		bias.position[x] = temp.position[x] + xrise;
		bias.position[y] = temp.position[y] + 0;
		bias.position[z] = temp.position[z] + zsbulge;
		bias.normal[x] = xsign * 0.42f;
		bias.normal[y] = 0;
		bias.normal[z] = zsign * 0.9f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);

		bias.position[x] = temp.position[x] + xrise+xrun;
		bias.position[y] = temp.position[y] + yrise+yrun;
		bias.position[z] = temp.position[z];
		bias.normal[x] = xsign * 0.578f;
		bias.normal[y] = ysign * 0.578f;
		bias.normal[z] = zsign * 0.578f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);


		bias.position[x] = temp.position[x] + xrise+xrun;
		bias.position[y] = temp.position[y] + 0;
		bias.position[z] = temp.position[z];
		bias.normal[x] = xsign * 0.707f;
		bias.normal[y] = 0;
		bias.normal[z] = zsign * 0.707f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);

		bias.position[x] = temp.position[x] + xrise+xrun;
		bias.position[y] = temp.position[y] + yrise+yrun;
		bias.position[z] = temp.position[z];
		bias.normal[x] = xsign * 0.578f;
		bias.normal[y] = ysign * 0.578f;
		bias.normal[z] = zsign * 0.578f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);
		
		bias.position[x] = temp.position[x] + xrise;
		bias.position[y] = temp.position[y] + 0;
		bias.position[z] = temp.position[z] + zsbulge;
		bias.normal[x] = xsign * 0.42f;
		bias.normal[y] = 0;
		bias.normal[z] = zsign * 0.9f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);

		


		bias.position[x] = temp.position[x] + 0;
		bias.position[y] = temp.position[y] + yrise;
		bias.position[z] = temp.position[z] + zsbulge;
		bias.normal[x] = 0;
		bias.normal[y] = ysign * 0.42f;
		bias.normal[z] = zsign * 0.9f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);
		
		bias.position[x] = temp.position[x] + 0;
		bias.position[y] = temp.position[y] + yrise+yrun;
		bias.position[z] = temp.position[z];
		bias.normal[x] = 0;
		bias.normal[y] = ysign * 0.707f;
		bias.normal[z] = zsign * 0.707f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);

		bias.position[x] = temp.position[x] + xrise+xrun;
		bias.position[y] = temp.position[y] + yrise+yrun;
		bias.position[z] = temp.position[z];
		bias.normal[x] = xsign * 0.578f;
		bias.normal[y] = ysign * 0.578f;
		bias.normal[z] = zsign * 0.578f;
				bias.normal[x] = bias.position[x];
		bias.normal[y] = bias.position[y];
		bias.normal[z] = bias.position[z];
		pushEdgeTextureMap(temp, bias, x, y);
		vertices->push_back(bias);
		/*
		temp.textures[0] = 0.5f;
		temp.textures[1] = 0.5f;
		temp.normal[z] = 1;
		temp.normal[x] = 0.0f;
		temp.normal[y] = 0.0f;
		vertices->push_back(temp);
		// |
		temp.textures[1] -= yrun;
		temp.position[y] -= yrun;
		temp.position[z] -= zlbulge;
		temp.normal[z] = zlbulge;
		temp.normal[y] = -ysbulge;
		
		vertices->push_back(temp);
		// /.
		temp.position[x] -= xrun;
		temp.textures[0] -= xrun;
		temp.position[y] += yrun;
		temp.textures[1] += yrun;
		temp.normal[x] = -xsbulge;
		temp.normal[y] = 0;
		vertices->push_back(temp);
		vertices->push_back(temp);
		
		// \.
		temp.position[y] -= (yrun + yrise);
		temp.textures[1] -= (yrun + yrise);
		temp.position[x] -= xrise;
		temp.textures[0] -= xrise;
		temp.position[z] -= zsbulge;
		temp.normal[x] = -xsbulge*1.4;
		temp.normal[y] = -ysbulge*1.4;
		temp.normal[z] = zsbulge;
		vertices->push_back(temp);
		// |
		temp.position[y] += (yrun + yrise);
		temp.textures[1] += (yrun + yrise);
		temp.normal[x] = -xsbulge*2;
		temp.normal[z] = zsbulge;
		temp.normal[y] = 0;
		vertices->push_back(temp);
		temp.position[x] += xrise;
		temp.textures[0] += xrise;
		temp.position[z] += zsbulge;
		temp.normal[x] = -xsbulge;
		temp.normal[y] = 0;
		temp.normal[z] = zsbulge;
		// \.
		vertices->push_back(temp);
		temp.position[y] -= (yrun + yrise);
		temp.textures[1] -= (yrun + yrise);
		temp.position[x] -= xrise;
		temp.textures[0] -= xrise;
		temp.position[z] -= zsbulge;
		temp.normal[x] = -xsbulge*2;
		temp.normal[y] = -ysbulge*2;
		temp.normal[z] = zsbulge;
		vertices->push_back(temp);
		// -\.
		temp.position[x] += (xrun + xrise);
		temp.textures[0] += (xrun + xrise);
		temp.position[y] += yrise;
		temp.textures[1] += yrise;
		temp.position[z] += zsbulge;
		temp.normal[z] = zlbulge;
		temp.normal[y] = -ysbulge;
		temp.normal[x] = 0;
		vertices->push_back(temp);
		
		vertices->push_back(temp);
		temp.position[z] -= zsbulge;
		temp.position[y] -= yrise;
		temp.textures[1] -= yrise;
		temp.normal[z] = zsbulge;
		temp.normal[y] = -ysbulge*2;
		temp.normal[x] = 0;
		vertices->push_back(temp);
		temp.position[x] -= (xrun + xrise);
		temp.textures[0] -= (xrun + xrise);
		temp.normal[z] = zsbulge;
		temp.normal[y] = -ysbulge*1.4;
		temp.normal[x] = -xsbulge*1.4;
		vertices->push_back(temp);
		temp.position[x] += (xrun + xrise);
		temp.position[y] += (yrun + yrise);
		temp.position[z] += (zsbulge + zlbulge);
		*/
		}

	if (edge == 0) //Top
	{
		//temp.normal[0] = -.577;
		//temp.normal[1] = -.577;
		//temp.normal[2] = -.577;
		/*
		temp.position[0] -= 0.5;
		temp.position[1] -= 0.5;
		temp.position[2] += 0.5;
		vertices->push_back(temp);
		temp.position[0] += 1;
		temp.textures[0] += 1;
		temp.normal[0] *= -1;
		vertices->push_back(temp);
		temp.position[1] += 1;
		temp.textures[1] += 1;
		temp.normal[1] *= -1;
		vertices->push_back(temp);
		temp.position[0] -= 1;
		temp.textures[0] -= 1;
		temp.normal[0] *= -1;
		vertices->push_back(temp);*/
		
		//temp.position[0] -= 0.5;
		//temp.position[1] -= 0.5;
		//temp.position[2] += 0.5;
		//temp.normal[0] *= -1;
		//vertices->push_back(temp);
		//temp.position[0] += 0.5;
		//temp.textures[0] += 0.5;
		//vertices->push_back(temp);
		//temp.position[1] += 0.5;
		//temp.textures[1] += 0.5;
		//temp.position[2] += 0.125;
		//vertices->push_back(temp);
		//temp.position[2] -= 0.125;
		//temp.position[0] -= 0.5;
		//temp.textures[0] -= 0.5;
		//vertices->push_back(temp);

		//temp.position[0] += 0.5;
		//temp.position[1] -= 0.5;
		//temp.textures[0] += 0.5;
		//temp.textures[1] -= 0.5;
		//vertices->push_back(temp);
		//temp.position[0] += 0.5;
		//temp.textures[0] += 0.5;
		//vertices->push_back(temp);
		//temp.position[1] += 0.5;
		//temp.textures[1] += 0.5;
		//vertices->push_back(temp);
		//temp.position[0] -= 0.5;
		//temp.textures[0] -= 0.5;
		//temp.position[2] += 0.125;
		//vertices->push_back(temp);
		//temp.position[2] -= 0.125;

		//temp.position[0] -= 0.5;
		////temp.position[1] -= 0.5;
		//temp.textures[0] -= 0.5;
		////temp.textures[1] -= 0.5;
		//vertices->push_back(temp);
		//temp.position[0] += 0.5;
		//temp.textures[0] += 0.5;
		//temp.position[2] += 0.125;
		//vertices->push_back(temp);
		//temp.position[2] -= 0.125;
		//temp.position[1] += 0.5;
		//temp.textures[1] += 0.5;
		//vertices->push_back(temp);
		//temp.position[0] -= 0.5;
		//temp.textures[0] -= 0.5;
		//vertices->push_back(temp);

		//temp.position[0] += 0.5;
		//temp.position[1] -= 0.5;
		//temp.textures[0] += 0.5;
		//temp.textures[1] -= 0.5;
		//temp.position[2] += 0.125;
		//vertices->push_back(temp);
		//temp.position[0] += 0.5;
		//temp.textures[0] += 0.5;
		//temp.position[2] -= 0.125;
		//vertices->push_back(temp);
		//temp.position[1] += 0.5;
		//temp.textures[1] += 0.5;
		//vertices->push_back(temp);
		//temp.position[0] -= 0.5;
		//temp.textures[0] -= 0.5;
		//vertices->push_back(temp);

		



		//

		////Q1
		//vertices->push_back(temp);
		//temp.position[1] -= run;
		//temp.position[2] -= rise;
		//vertices->push_back(temp);
		//temp.position[0] -= run;
		//temp.position[2] -= rise;
		//vertices->push_back(temp);
		//
		//vertices->push_back(temp);
		//temp.position[2] += rise;
		//temp.position[1] += run;
		//vertices->push_back(temp);
		//temp.position[0] += run;
		//temp.position[2] += rise;
		//vertices->push_back(temp);
		//
		////Q2
		//vertices->push_back(temp);
		//temp.position[1] += run;
		//temp.position[2] -= rise;
		//vertices->push_back(temp);
		//temp.position[0] -= run;
		//temp.position[2] -= rise;
		//vertices->push_back(temp);
		//
		//vertices->push_back(temp);
		//temp.position[2] += rise;
		//temp.position[1] -= run;
		//vertices->push_back(temp);
		//temp.position[0] += run;
		//temp.position[2] += rise;
		//vertices->push_back(temp);

		////Q3
		//vertices->push_back(temp);
		//temp.position[1] += run;
		//temp.position[2] -= rise;
		//vertices->push_back(temp);
		//temp.position[0] += run;
		//temp.position[2] -= rise;
		//vertices->push_back(temp);
		//
		//vertices->push_back(temp);
		//temp.position[2] += rise;
		//temp.position[1] -= run;
		//vertices->push_back(temp);
		//temp.position[0] -= run;
		//temp.position[2] += rise;
		//vertices->push_back(temp);

		////Q4
		//vertices->push_back(temp);
		//temp.position[1] -= run;
		//temp.position[2] -= rise;
		//vertices->push_back(temp);
		//temp.position[0] += run;
		//temp.position[2] -= rise;
		//vertices->push_back(temp);
		//
		//vertices->push_back(temp);
		//temp.position[2] += rise;
		//temp.position[1] += run;
		//vertices->push_back(temp);
		//temp.position[0] -= run;
		//temp.position[2] += rise;
		//vertices->push_back(temp);
	}
	else
		return;
	if (edge == 1) //Bottom
	{
		//temp.normal[0] = -.577;
		//temp.normal[1] = -.577;
		//temp.normal[2] = -.577;
		temp.position[0] -= 0.5;
		temp.position[1] -= 0.5;
		temp.position[2] -= 0.5;
		vertices->push_back(temp);
		temp.position[0] += 1;
		temp.textures[0] += 1;
		temp.normal[0]  *= -1;
		vertices->push_back(temp);
		temp.position[1] += 1;
		temp.textures[1] += 1;
		temp.normal[1]  *= -1;
		vertices->push_back(temp);
		temp.position[0] -= 1;
		temp.textures[0] -= 1;
		temp.normal[0] *= -1;
		vertices->push_back(temp);
	}
	if (edge == 2) //Left -- TODO
	{
		//temp.normal[0] = -0.577;
		//temp.normal[1] = -0.577;
		//temp.normal[2] = -0.577;
		temp.position[0] += 0.5;
		temp.position[1] -= 0.5;
		temp.position[2] -= 0.5;
		vertices->push_back(temp);
		temp.position[2] += 1;
		temp.textures[0] += 1;
		temp.normal[2] *= -1;
		vertices->push_back(temp);
		temp.position[1] += 1;
		temp.textures[1] += 1;
		temp.normal[1] *= -1;
		vertices->push_back(temp);
		temp.position[2] -= 1;
		temp.textures[0] -= 1;
		temp.normal[2] *= -1;
		vertices->push_back(temp);
	}
	if (edge == 3) //Right -- TODO
	{
		//temp.normal[0] = -0.577;
		//temp.normal[1] = -0.577;
		//temp.normal[2] = -0.577;
		temp.position[0] -= 0.5;
		temp.position[1] -= 0.5;
		temp.position[2] -= 0.5;
		vertices->push_back(temp);
		temp.position[2] += 1;
		temp.textures[0] += 1;
		temp.normal[2] *= -1;
		vertices->push_back(temp);
		temp.position[1] += 1;
		temp.textures[1] += 1;
		temp.normal[1] *= -1;
		vertices->push_back(temp);
		temp.position[2] -= 1;
		temp.textures[0] -= 1;
		temp.normal[2] *= -1;
		vertices->push_back(temp);
	}
	if (edge == 4) //Front -- TODO
	{
		//temp.normal[0] = -0.577;
		//temp.normal[1] = -0.577;
		//temp.normal[2] = -0.577;
		temp.position[0] -= 0.5;
		temp.position[1] += 0.5;
		temp.position[2] -= 0.5;
		vertices->push_back(temp);
		temp.position[2] += 1;
		temp.textures[1] += 1;
		temp.normal[2] *= -1;
		vertices->push_back(temp);
		temp.position[0] += 1;
		temp.textures[0] += 1;
		temp.normal[0] *= -1;
		vertices->push_back(temp);
		temp.position[2] -= 1;
		temp.textures[1] -= 1;
		temp.normal[2] *= -1;
		vertices->push_back(temp);
	}
	if (edge == 5) //Back -- TODO
	{
		//temp.normal[0] = -0.577;
		//temp.normal[1] = -0.577;
		//temp.normal[2] = -0.577;
		temp.position[0] -= 0.5;
		temp.position[1] -= 0.5;
		temp.position[2] -= 0.5;
		vertices->push_back(temp);
		temp.position[2] += 1;
		temp.textures[1] += 1;
		temp.normal[2] *= -1;
		vertices->push_back(temp);
		temp.position[0] += 1;
		temp.textures[0] += 1;
		temp.normal[0] *= -1;
		vertices->push_back(temp);
		temp.position[2] -= 1;
		temp.textures[1] -= 1;
		temp.normal[2] *= -1;
		vertices->push_back(temp);
	}

}

//void pushEdge(int edge, float x, float y, float z, float scale, float r, float g, float b, float a, std::vector<float> *vertices, std::vector<unsigned int> *indices, std::vector<float> *textures, std::vector<float> *normals, std::vector<float> *colors)
//{
//	unsigned int tail = vertices->size()/3;
//	//std::vector<float> tempVertices;
//	//std::vector<unsigned int> tempIndices;
//	int top = 0;
//	int bottom = 1;
//	int left = 2;
//	int right = 3;
//	if (edge == top)
//	{
//		float cube_vertices[] = 
//			{x+0, y+0, z+0,
//			x+scale, y+0, z+0,
//			x+scale, y+scale, z+0,
//			x+0, y+scale, z+0
//			};
//	
//		float cube_normals[] = 
//		{
//			0.0f, 0.0f, 1.0f,
//			0.0f, 0.0f, 1.0f,
//			0.0f, 0.0f, 1.0f,
//			0.0f, 0.0f, 1.0f
//		};
//		float cube_textures[] = 
//		{0.0f, 0.0f,
//		1.0f, 0.0f,
//		1.0f, 1.0f,
//		0.0f, 1.0f
//		};
//	float cube_colors[] = 
//	{
//		r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a
//	};
//	unsigned int cube_indices[] = 
//		{tail+0, tail+1, tail+2, tail+3	};
//	vertices->insert(vertices->end(), cube_vertices, cube_vertices+24);
//	indices->insert(indices->end(), cube_indices, cube_indices+8);
//	textures->insert(textures->end(), cube_textures, cube_textures+16);
//	normals->insert(normals->end(), cube_normals, cube_normals+24);
//	colors->insert(colors->end(), cube_colors, cube_colors+32);
//	}
//    if (edge == bottom)
//	{
//		float cube_vertices[] = 
//			{x+0, y+0, z-scale,
//			x+scale, y+0, z-scale,
//			x+scale, y+scale, z-scale,
//			x+0, y+scale, z-scale
//			};
//	
//		float cube_normals[] = 
//		{
//			0.0f, 0.0f, -1.0f,
//			0.0f, 0.0f, -1.0f,
//			0.0f, 0.0f, -1.0f,
//			0.0f, 0.0f, -1.0f
//		};
//		float cube_textures[] = 
//		{0.0f, 0.0f,
//		1.0f, 0.0f,
//		1.0f, 1.0f,
//		0.0f, 1.0f
//		};
//	float cube_colors[] = 
//	{
//		r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a
//	};
//	unsigned int cube_indices[] = 
//		{tail+0, tail+1, tail+2, tail+3	};
//	vertices->insert(vertices->end(), cube_vertices, cube_vertices+24);
//	indices->insert(indices->end(), cube_indices, cube_indices+8);
//	textures->insert(textures->end(), cube_textures, cube_textures+16);
//	normals->insert(normals->end(), cube_normals, cube_normals+24);
//	colors->insert(colors->end(), cube_colors, cube_colors+32);
//	}
//	if (edge == left)
//	{
//		float cube_vertices[] = 
//			{x+0, y+0, z,
//			x+0, y+scale, z,
//			x+0, y+scale, z-scale,
//			x+0, y+0, z-scale
//			};
//	
//		float cube_normals[] = 
//		{
//			-1.0f, 0.0f, 0.0f,
//			-1.0f, 0.0f, 0.0f,
//			-1.0f, 0.0f, 0.0f,
//			-1.0f, 0.0f, 0.0f
//		};
//		float cube_textures[] = 
//		{0.0f, 0.0f,
//		1.0f, 0.0f,
//		1.0f, 1.0f,
//		0.0f, 1.0f
//		};
//	float cube_colors[] = 
//	{
//		r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a
//	};
//	unsigned int cube_indices[] = 
//		{tail+0, tail+1, tail+2, tail+3	};
//	vertices->insert(vertices->end(), cube_vertices, cube_vertices+24);
//	indices->insert(indices->end(), cube_indices, cube_indices+8);
//	textures->insert(textures->end(), cube_textures, cube_textures+16);
//	normals->insert(normals->end(), cube_normals, cube_normals+24);
//	colors->insert(colors->end(), cube_colors, cube_colors+32);
//	}
//	
//	if (edge == right)
//	{
//		float cube_vertices[] = 
//			{x+scale, y+0, z,
//			x+scale, y+scale, z,
//			x+scale, y+scale, z-scale,
//			x+scale, y+0, z-scale
//			};
//	
//		float cube_normals[] = 
//		{
//			1.0f, 0.0f, 0.0f,
//			1.0f, 0.0f, 0.0f,
//			1.0f, 0.0f, 0.0f,
//			1.0f, 0.0f, 0.0f
//		};
//		float cube_textures[] = 
//		{0.0f, 0.0f,
//		1.0f, 0.0f,
//		1.0f, 1.0f,
//		0.0f, 1.0f
//		};
//	float cube_colors[] = 
//	{
//		r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a
//	};
//	unsigned int cube_indices[] = 
//		{tail+0, tail+1, tail+2, tail+3	};
//	vertices->insert(vertices->end(), cube_vertices, cube_vertices+24);
//	indices->insert(indices->end(), cube_indices, cube_indices+8);
//	textures->insert(textures->end(), cube_textures, cube_textures+16);
//	normals->insert(normals->end(), cube_normals, cube_normals+24);
//	colors->insert(colors->end(), cube_colors, cube_colors+32);
//	}
//	
//}

void pushCube(float x, float y, float z, float scale, float r, float g, float b, float a, std::vector<float> *vertices, std::vector<unsigned int> *indices, std::vector<float> *textures, std::vector<float> *normals, std::vector<float> *colors)
{
	unsigned int tail = vertices->size()/3;
	//std::vector<float> tempVertices;
	//std::vector<unsigned int> tempIndices;
	float cube_vertices[] = 
		{x+0, y+0, z+0,
		x+scale, y+0, z+0,
		x+scale, y+scale, z+0,
		x+0, y+scale, z+0,

		x+0, y+0, z-scale,
		x+scale, y+0, z-scale,
		x+scale, y+scale, z-scale,
		x+0, y+scale, z-scale,
	
		x+0, y+0, z+0,
		x+scale, y+0, z+0,
		x+scale, y+0, z-scale,
		x+0, y+0, z-scale,

		x+scale, y+0, z+0,
		x+scale, y+scale, z+0,
		x+scale, y+scale, z-scale,
		x+scale, y+0, z-scale,

		x+scale, y+scale, z+0,
		x+0, y+scale, z+0,
		x+0, y+scale, z-scale,
		x+scale, y+scale, z-scale,

		x+0, y+0, z+0,
		x+0, y+scale, z+0,
		x+0, y+scale, z-scale,
		x+0, y+0, z-scale
		};
	float cube_textures[] = 
		{0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f};
	float cube_normals[] = 
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f
	};
	float cube_colors[] = 
	{
		r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a,
		r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a,
		r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a,
		r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a,
		r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a,
		r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a,
	};
	unsigned int cube_indices[] = 
		{tail+0, tail+1, tail+2, tail+3,
		tail+4, tail+5, tail+6, tail+7,
		tail+8, tail+9, tail+10, tail+11,
		tail+12, tail+13, tail+14, tail+15,
		tail+16, tail+17, tail+18, tail+19,
		tail+20, tail+21, tail+22, tail+23
		};
	//tempVertices.assign(cube_vertices, cube_vertices+13);
	//tempIndices.assign(cube_indices, cube_indices+5);
	vertices->insert(vertices->end(), cube_vertices, cube_vertices+72);
	indices->insert(indices->end(), cube_indices, cube_indices+24);
	textures->insert(textures->end(), cube_textures, cube_textures+48);
	normals->insert(normals->end(), cube_normals, cube_normals+72);
	colors->insert(colors->end(), cube_colors, cube_colors+96);
	//indices->push_back(cube_indices);
}

void drawCube(float x, float y, float z, float scale)
{
/*x = x - 0.5f;
y = y - 0.5f;
z = z - 0.5f;*/
glPushMatrix();
//glLoadIdentity();
glTranslatef(x,y,z);
//glTranslatef(x-0.5,y-0.5,z-0.5);
//glUseProgram(0);
//glRotatef(180-angle,0,0,1);
			

			//top
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f);
			
			glVertex3f(x+0, y+0, z+0);
			glTexCoord2f(1.0f, 0.0f);
			
			glVertex3f(x+scale, y+0, z+0);
			glTexCoord2f(1.0f, 1.0f);
			
			glVertex3f(x+scale, y+scale, z+0);
			glTexCoord2f(0.0f, 1.0f);
			
			glVertex3f(x+0, y+scale, z+0);
			
			//bottom
			glNormal3f(0.0f, 0.0f, -1.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x+0, y+0, z-1*scale);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x+scale, y+0, z-1*scale);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x+scale, y+1*scale, z-1*scale);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x+0, y+1*scale, z-1*scale);

			//front
			glNormal3f(0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f(x+0, y+0, z+0);
			glTexCoord2f(-1.0f, 0.0f);
			
			glVertex3f(x+1*scale, y+0, z+0);
			glTexCoord2f(-1.0f, -1.0f);
			
			glVertex3f(x+1*scale, y+0, z-1*scale);
			glTexCoord2f(0.0f, -1.0f);
			
			glVertex3f(x+0, y+0, z-1*scale);

			//back
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f(x+0, y+1*scale, z+0);
			glTexCoord2f(-1.0f, 0.0f);
			
			glVertex3f(x+1*scale, y+1*scale, z+0);
			glTexCoord2f(-1.0f, -1.0f);
			
			glVertex3f(x+1*scale, y+1*scale, z-1*scale);
			glTexCoord2f(0.0f, -1.0f);
			
			glVertex3f(x+0, y+1*scale, z-1*scale);

			//left
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f(x+0, y+0, z+0 );
			glTexCoord2f(-1.0f, 0.0f);
			
			glVertex3f(x+0, y+1*scale, z+0);
			glTexCoord2f(-1.0f, -1.0f);
			
			glVertex3f(x+0, y+1*scale, z-1*scale);
			glTexCoord2f(0.0f, -1.0f);
			
			glVertex3f(x+0, y+0, z-1*scale);

			//right
			glNormal3f(+1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f(x+1*scale, y+0, z+0);
			glTexCoord2f(-1.0f, 0.0f);
			
			glVertex3f(x+1*scale, y+1*scale, z+0);
			glTexCoord2f(-1.0f, -1.0f);
			
			glVertex3f(x+1*scale, y+1*scale, z-1*scale);
			glTexCoord2f(0.0f, -1.0f);
			
			glVertex3f(x+1*scale, y+0, z-1*scale);
			/* */
			
		glPopMatrix();
}

void drawPoint(float x, float y, float z, float scale, float mx, float my, float mz, Image *image)
{
	UNREFERENCED_PARAMETER(image);
	UNREFERENCED_PARAMETER(scale);


	glPushMatrix();
	float distance;
	//glLoadIdentity();
	//glTranslatef(x,y,z);
	glTranslatef(x-0.5,y-0.5,z-0.5);
	distance = sqrt((mx-x)*(mx-x)+(my-y)*(my-y)+(mz-z)*(mz-z));
	//glPointSize(100000/(distance*distance));
	glPointParameterf(GL_POINT_SIZE_MAX, 1000.0f);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(64);

	//glUseProgram(0);
	//glRotatef(180-angle,0,0,1);
	//glBindTexture(GL_TEXTURE_2D, objectID);
	glEnable(GL_POINT_SPRITE_ARB);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

	float distiterator = 0;
	float range = 0.35f;
	int type = 0;
	glBegin(GL_POINTS);
	glDisable(GL_TEXTURE_2D);
	//glBegin(GL_TRIANGLE_STRIP);
	if (distance < 20.0f)
	{
		for (distiterator = distance/5; distiterator < distance; distiterator = distiterator + distance/5)
		{
			if (type == 0)
			{
			range = 0.5f;
			if (distiterator > 1.0){
			glVertex3f(range/distiterator, range/distiterator, -range/distiterator);
			glVertex3f(-range/distiterator, -range/distiterator, -range/distiterator);
			glVertex3f(-range/distiterator, range/distiterator, -range/distiterator);
			glVertex3f(range/distiterator, -range/distiterator, -range/distiterator);

			glVertex3f(range/distiterator, range/distiterator, range/distiterator);
			glVertex3f(-range/distiterator, -range/distiterator, range/distiterator);
			glVertex3f(-range/distiterator, range/distiterator, range/distiterator);
			glVertex3f(range/distiterator, -range/distiterator, range/distiterator);
			type = 1;}
			}
			else 
			{
			//glVertex3f(0.0f/distiterator, 0.0f/distiterator, 0.0f/distiterator);
			range = 0.707f;
			if (distiterator > 1.0){
			glVertex3f(-range/distiterator, 0/distiterator, 0.0/distiterator);
			glVertex3f(range/distiterator, 0/distiterator, 0.0/distiterator);

			glVertex3f(0/distiterator, range/distiterator, 0.0f/distiterator);
			glVertex3f(0/distiterator, -range/distiterator, 0.0f/distiterator);
		
			glVertex3f(0/distiterator, 0/distiterator, range/distiterator);
			glVertex3f(0/distiterator, 0/distiterator, -range/distiterator);
			type = 0;}
			}
		}
	
	}
	else{
		glVertex3f(0, 0, 0);
	}
		 //glDrawPixels(100, 100, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	glEnd();
	glPopMatrix();
}

void makeOcto(float x, float y, float z, float scale)
{
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(z);

	static float bulge = 0.5*1.618f;//0.5*1.618f;
	static float edge = 0.618f;//0.618f; 

			glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0.0f, 0.0f, 1.0f);						
			glVertex3f(0, 0, scale*bulge);
			glNormal3f(0.577f, 0.577f, 0.577f);			
			glVertex3f(scale*0.5f, scale*0.5f, scale*0.5f);
			glNormal3f(0, 0.707f, 0.707f);			
			glVertex3f(0, scale*edge, scale*edge);
			glNormal3f(-0.577f, 0.577f, 0.577f);			
			glVertex3f(-scale*0.5f, scale*0.5f, scale*0.5f);
			glNormal3f(-0.707f, 0, 0.707f);			
			glVertex3f(-scale*edge, 0, scale*edge);
			glNormal3f(-0.577f, -0.577f, 0.577f);
			glVertex3f(-scale*0.5f, -scale*0.5f, scale*0.5f);
			glNormal3f(0, -0.707f, 0.707f);			
			glVertex3f(0, -scale*edge, scale*edge);
			glNormal3f(0.577f, -0.577f, 0.577f);			
			glVertex3f(scale*0.5f, -scale*0.5f, scale*0.5f);
			glNormal3f(0.707f, 0, 0.707f);			
			glVertex3f(scale*edge, 0, scale*edge);
			glNormal3f(0.577f, 0.577f, 0.577f);
			glVertex3f(scale*0.5f, scale*0.5f, scale*0.5f);						
			glEnd();

			glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0.0f, 0.0f, -1.0f);						
			glVertex3f(0, 0, -scale*bulge);
			glNormal3f(0.577f, 0.577f, -0.577f);			
			glVertex3f(scale*0.5f, scale*0.5f, -scale*0.5f);
			glNormal3f(0, 0.707f, -0.707f);			
			glVertex3f(0, scale*edge, -scale*edge);
			glNormal3f(-0.577f, 0.577f, -0.577f);			
			glVertex3f(-scale*0.5f, scale*0.5f, -scale*0.5f);
			glNormal3f(-0.707f, 0, -0.707f);			
			glVertex3f(-scale*edge, 0, -scale*edge);
			glNormal3f(-0.577f, -0.577f, -0.577f);
			glVertex3f(-scale*0.5f, -scale*0.5f, -scale*0.5f);
			glNormal3f(0, -0.707f, -0.707f);			
			glVertex3f(0, -scale*edge, -scale*edge);
			glNormal3f(0.577f, -0.577f, -0.577f);			
			glVertex3f(scale*0.5f, -scale*0.5f, -scale*0.5f);
			glNormal3f(0.707f, 0, -0.707f);			
			glVertex3f(scale*edge, 0, -scale*edge);
			glNormal3f(0.577f, 0.577f, -0.577f);
			glVertex3f(scale*0.5f, scale*0.5f, -scale*0.5f);						
			glEnd();
			
			glBegin(GL_TRIANGLE_FAN);
			glNormal3f(-1.0f, 0.0f, 0.0f);						
			glVertex3f(-scale*bulge, 0, 0);
			glNormal3f(-0.577f, 0.577f, -0.577f);						
			glVertex3f(-scale*0.5f, scale*0.5f, -scale*0.5f);
			glNormal3f(-0.707f, 0, -0.707f);			
			glVertex3f(-scale*edge, 0, -scale*edge);			
			glNormal3f(-0.577f, -0.577f, -0.577f);
			glVertex3f(-scale*0.5f, -scale*0.5f, -scale*0.5f);
			glNormal3f(-0.707f, -0.707f, 0);			
			glVertex3f(-scale*edge, -scale*edge, 0);
			glNormal3f(-0.577f, -0.577f, 0.577f);
			glVertex3f(-scale*0.5f, -scale*0.5f, scale*0.5f);
			glNormal3f(-0.707f, 0, 0.707f);			
			glVertex3f(-scale*edge, 0, scale*edge);
			glNormal3f(-0.577f, 0.577f, 0.577f);
			glVertex3f(-scale*0.5f, scale*0.5f, scale*0.5f);
			glNormal3f(-0.707f, 0.707f, 0);			
			glVertex3f(-scale*edge, scale*edge, 0);
			glNormal3f(-0.577f, 0.577f, -0.577f);
			glVertex3f(-scale*0.5f, scale*0.5f, -scale*0.5f);						
			glEnd();

			glBegin(GL_TRIANGLE_FAN);
			glNormal3f(1.0f, 0.0f, 0.0f);			
			glVertex3f(scale*bulge, 0, 0);
			glNormal3f(0.577f, 0.577f, -0.577f);						
			glVertex3f(scale*0.5f, scale*0.5f, -scale*0.5f);
			glNormal3f(0.707f, 0, -0.707f);		
			glVertex3f(scale*edge, 0, -scale*edge);			
			glNormal3f(0.577f, -0.577f, -0.577f);
			glVertex3f(scale*0.5f, -scale*0.5f, -scale*0.5f);
			glNormal3f(0.707f, -0.707f, 0);			
			glVertex3f(scale*edge, -scale*edge, 0);
			glNormal3f(0.577f, -0.577f, 0.577f);
			glVertex3f(scale*0.5f, -scale*0.5f, scale*0.5f);
			glNormal3f(0.707f, 0, 0.707f);			
			glVertex3f(scale*edge, 0, scale*edge);
			glNormal3f(0.577f, 0.577f, 0.577f);
			glVertex3f(scale*0.5f, scale*0.5f, scale*0.5f);
			glNormal3f(0.707f, 0.707f, 0);			
			glVertex3f(scale*edge, scale*edge, 0);
			glNormal3f(0.577f, 0.577f, -0.577f);
			glVertex3f(scale*0.5f, scale*0.5f, -scale*0.5f);						
			glEnd();

			glBegin(GL_TRIANGLE_FAN);
			
			glNormal3f(0.0f, -1.0f, 0.0f);
						
			glVertex3f(0, -scale*bulge, 0);
			glNormal3f(0.577f, -0.577f, 0.577f);			
			glVertex3f(scale*0.5f, -scale*0.5f, scale*0.5f);
			glNormal3f(0, -0.707f, 0.707f);		
			glVertex3f(0, -scale*edge, scale*edge);	
			glNormal3f(-0.577f, -0.577f, 0.577f);			
			glVertex3f(-scale*0.5f, -scale*0.5f, scale*0.5f);
			glNormal3f(-0.707f, -0.707f, 0);		
			glVertex3f(-scale*edge, -scale*edge, 0);	
			glNormal3f(-0.577f, -0.577f, -0.577f);
			glVertex3f(-scale*0.5f, -scale*0.5f, -scale*0.5f);
			glNormal3f(0, -0.707f, -0.707f);		
			glVertex3f(0, -scale*edge, -scale*edge);
			glNormal3f(0.577f, -0.577f, -0.577f);
			glVertex3f(scale*0.5f, -scale*0.5f, -scale*0.5f);
			glNormal3f(0.707f, -0.707f, 0);		
			glVertex3f(scale*edge, -scale*edge, 0);
			glNormal3f(0.577f, -0.577f, 0.577f);
			glVertex3f(scale*0.5f, -scale*0.5f, scale*0.5f);
						
			glEnd();

			glBegin(GL_TRIANGLE_FAN);
			
			glNormal3f(0.0f, 1.0f, 0.0f);
						
			glVertex3f(0, scale*bulge, 0);
			glNormal3f(0.577f, 0.577f, 0.577f);			
			glVertex3f(scale*0.5f, scale*0.5f, scale*0.5f);
			glNormal3f(0, 0.707f, 0.707f);		
			glVertex3f(0, scale*edge, scale*edge);	
			glNormal3f(-0.577f, 0.577f, 0.577f);			
			glVertex3f(-scale*0.5f, scale*0.5f, scale*0.5f);
			glNormal3f(-0.707f, 0.707f, 0);		
			glVertex3f(-scale*edge, scale*edge, 0);	
			glNormal3f(-0.577f, 0.577f, -0.577f);
			glVertex3f(-scale*0.5f, scale*0.5f, -scale*0.5f);
			glNormal3f(0, 0.707f, -0.707f);		
			glVertex3f(0, scale*edge, -scale*edge);
			glNormal3f(0.577f, 0.577f, -0.577f);
			glVertex3f(scale*0.5f, scale*0.5f, -scale*0.5f);
			glNormal3f(0.707f, 0.707f, 0);		
			glVertex3f(scale*edge, scale*edge, 0);
			glNormal3f(0.577f, 0.577f, 0.577f);
			glVertex3f(scale*0.5f, scale*0.5f, scale*0.5f);
						
			glEnd();
}

void drawOcto(float x, float y, float z, float scale)
{
/*x = x - 0.5f;
y = y - 0.5f;
z = z - 0.5f;*/
glPushMatrix();
//glLoadIdentity();
glTranslatef(x,y,z);
glTranslatef(0,0,-2);
//glTranslatef(x-0.5,y-0.5,z-0.5);
//glUseProgram(0);
//glRotatef(180-angle,0,0,1);
	glDisable(GL_TEXTURE_2D);
	int count = 1;
	scale = scale / count;
	//glDisable(GL_BLEND);
	//

	for (int j = 0; j < count; j++)
	{
		glTranslatef(0,0,scale);
	for (int i = 0; i < count; i++)
	{
		glTranslatef(0,scale,0);
		for(int t = 0; t < count; t++)
		{
			glTranslatef(scale,0,0);
			makeOcto(x,y,z,scale);
		}
		glTranslatef(-1,0,0);
	}
	glTranslatef(0,-1,0);
	}
	/*
			makeOcto(x,y,z,scale);
			glTranslatef(0,0.5f,0);

			makeOcto(x,y,z,scale);
			glTranslatef(-0.5f,0,0);

			makeOcto(x,y,z,scale);
			glTranslatef(0,-0.5f,-0.5f);

			makeOcto(x,y,z,scale);
			glTranslatef(0.5f,0,0);

			makeOcto(x,y,z,scale);
			glTranslatef(0,0.5f,0);

			makeOcto(x,y,z,scale);
			glTranslatef(-0.5f,0,0);
			makeOcto(x,y,z,scale);*/

		glPopMatrix();
}

void drawPyramid(float x, float y, float z)
{
x = x - 0.5f;
y = y - 0.5f;
z = z - 0.5f;

			//bottom
			glBegin(GL_QUADS);

			glNormal3f(0.0f, 0.0f, -1.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x, y, z-1);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x+1, y, z-1);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x+1, y+1, z-1);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x, y+1, z-1);

			//front
			glNormal3f(0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f((float)x+0.5f, (float)y+0.5f, z);
			glTexCoord2f(1.0f, 0.0f);
			
			glVertex3f((float)x+0.5f, (float)y+0.5f, z);
			glTexCoord2f(1.0f, 1.0f);
			
			glVertex3f(x+1, y, z-1);
			glTexCoord2f(0.0f, 1.0f);
			
			glVertex3f(x, y, z-1);

			//back
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f((float)x+0.5f, (float)y+0.5f, z);
			glTexCoord2f(1.0f, 0.0f);
			
			glVertex3f((float)x+0.5f, (float)y+0.5f, z);
			glTexCoord2f(1.0f, 1.0f);
			
			glVertex3f(x+1, y+1, z-1);
			glTexCoord2f(0.0f, 1.0f);
			
			glVertex3f(x, y+1, z-1);

			//left
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f((float)x+0.5f, (float)y+0.5f, z );
			glTexCoord2f(1.0f, 0.0f);
			
			glVertex3f((float)x+0.5f, (float)y+0.5f, z);
			glTexCoord2f(1.0f, 1.0f);
			
			glVertex3f(x, y+1, z-1);
			glTexCoord2f(0.0f, 1.0f);
			
			glVertex3f(x, y, z-1);

			//right
			glNormal3f(+1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex3f((float)x+0.5f, (float)y+0.5f, z);
			glTexCoord2f(1.0f, 0.0f);
			
			glVertex3f((float)x+0.5f, (float)y+0.5f, z);
			glTexCoord2f(1.0f, 1.0f);
			
			glVertex3f(x+1, y+1, z-1);
			glTexCoord2f(0.0f, 1.0f);
			
			glVertex3f(x+1, y, z-1);
			glEnd();
}

void drawMesh(float x, float y, float z, float angle, ObjMesh* oMesh)
{
	
/*x = x - 0.5f;
y = y - 0.5f;*/
z = z - 1.0f;




ObjMesh* mesh = oMesh;

unsigned int points;

int a;
float red, green, blue;
//glMatrixMode(GL_MODELVIEW);
glPushMatrix();
//glLoadIdentity();
//glTranslatef(x,y,z);
glTranslatef(x,y,z);
glRotatef(180-angle,0,0,1);
glRotatef(-90, 1, 0, 0);
glRotatef(180, 0, 0, 1);
glShadeModel(GL_SMOOTH);

glBegin(GL_TRIANGLES);
for (points = 0; points < mesh->m_iNumberOfFaces; points++)
{
	//glVertex3f(object->Mesh.m_aVertexArray[points].x, object->Mesh.m_aVertexArray[points].y, object->Mesh.m_aVertexArray[points].z);
	/*if (object->Mesh.m_iNumberOfNormals > points)
	{
		glNormal3f(object->Mesh.m_aNormalArray[points].x, object->Mesh.m_aNormalArray[points].y, object->Mesh.m_aNormalArray[points].z);
	}*/
	
	for (a = 0; a < 3; a++)
	{	
		red = fmod(mesh->m_aVertexArray[mesh->m_aFaces[points].m_aVertexIndices[a]].x, 1.0f);
		green = fmod(mesh->m_aVertexArray[mesh->m_aFaces[points].m_aVertexIndices[a]].y, 1.0f);
		blue = fmod(mesh->m_aVertexArray[mesh->m_aFaces[points].m_aVertexIndices[a]].z, 1.0f);
		glColor3f((red*red*red)/(red*green*blue), (green*green*green)/(red*green*blue), (blue*blue*blue)/(red*green*blue));
		if (mesh->m_aFaces[points].m_aNormalIndices[a])
			glNormal3f(mesh->m_aNormalArray[mesh->m_aFaces[points].m_aNormalIndices[a]].x,
					mesh->m_aNormalArray[mesh->m_aFaces[points].m_aNormalIndices[a]].y,
					mesh->m_aNormalArray[mesh->m_aFaces[points].m_aNormalIndices[a]].z);
		glVertex3f(mesh->m_aVertexArray[mesh->m_aFaces[points].m_aVertexIndices[a]].x,
				mesh->m_aVertexArray[mesh->m_aFaces[points].m_aVertexIndices[a]].y,
				mesh->m_aVertexArray[mesh->m_aFaces[points].m_aVertexIndices[a]].z);
	}
	
}
glEnd();

glPopMatrix();



}

void drawSquare(float x1, float y1, float x2, float y2)
{
	UNREFERENCED_PARAMETER(x1);
	UNREFERENCED_PARAMETER(y1);
	UNREFERENCED_PARAMETER(x2);
	UNREFERENCED_PARAMETER(y2);
	/*
	glMatrixMode (GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	
	glLoadIdentity();
	
	//gluOrtho2D (0, Window::getWidth(), 0, Window::getHeight());
	glOrtho (0, Window::getWidth(), Window::getHeight(), 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);

	//glLoadIdentity();
	//glDisable(GL_LIGHTING);


	glBegin(GL_QUADS);
	glVertex3f(x1, y1,0);
	glVertex3f(x1, y2,0);
	glVertex3f(x2, y2,0);
	glVertex3f(x2, y1,0);
	glEnd();
	//glRasterPos2f(0,0);
	
	

	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'c');
	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'a');
	//glEnd();
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);*/
}

void drawGlyph(float x, float y, char c)
{
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(c);
	/*
	glMatrixMode (GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D (0, Window::getWidth(), 0, Window::getHeight());
	

	glRasterPos2f(x+5,y+5);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);*/
}

void drawBox(float x, float y, Image* image) {
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(x);

	glMatrixMode (GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	
	glLoadIdentity();

	
	//gluOrtho2D (0, Window::getWidth(), 0, Window::getHeight());
	//glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	/*glBegin(GL_QUADS);
	glVertex2f(10, 10);
	glVertex2f(100, 10);
	glVertex2f(100, 100);
	glVertex2f(10, 100);*/
	glRasterPos2f(0,0);
	glDrawPixels(image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'c');
	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'a');
	//glEnd();
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
}

void drawMap(Map* map, float x, float y, float z, float scale)
{
	glPushMatrix();
	//glLoadIdentity();
	
	glTranslatef(x,y,z);
	glScalef(scale, scale, scale);
	
	//glRotatef(rot,rand()%100,rand()%100,rand()%100);
	//glRotatef(rot,0,0,1);
	//glTranslatef(-map->width/2, -map->height/2, +map->depth/2);
	drawMap(map);
	glPopMatrix();
}

void drawMap(Map* map)
{
	
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//glEnableClientState(GL_INDEX_ARRAY);
		
		glBindBuffer(GL_ARRAY_BUFFER, map->buffer);
		glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, position));
		//glBindBuffer(GL_ARRAY_BUFFER, map->textures);
		glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, textures));
		//glBindBuffer(GL_ARRAY_BUFFER, _normals);
		glNormalPointer(GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, normal));
		//glBindBuffer(GL_ARRAY_BUFFER, _colors);
		glColorPointer(4, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, color));
		//printf("--====Offsets: %i, %i, %i", offsetof(vertex_t, position), offsetof(vertex_t, normal), offsetof(vertex_t, color));
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indices);
				
		//glIndexPointer(1, GL_UNSIGNED_INT, 0);
					
		glDrawArrays(GL_TRIANGLES, 0, map->Vertices()->size());
		//glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//glVertexPointer(3,GL_FLOAT,0,0);
		//glDrawRangeElements(GL_TRIANGLES, 0, 2, 3, GL_UNSIGNED_INT, (void *)0);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glDisableClientState(GL_INDEX_ARRAY);
		
}

void generatePlanetMesh(Planet *planet)
{
	planet;
	planet->tempVertices->clear();
	vertex_t tempVertex;
	for (int c = 0; c < 6; c++)
	{
		tempVertex.color[0] = 0.0f;
		tempVertex.color[1] = 1.0f;
		tempVertex.color[2] = 0.0f;
		tempVertex.color[3] = 1.0f;
		
		tempVertex.position[0] = 0.0f;
		tempVertex.position[1] = 0.0f;
		tempVertex.position[2] = 0.0f;

		tempVertex.textures[0] = 0;
		tempVertex.textures[1] = 0;
		tempVertex.textures[2] = 0;
		tempVertex.textures[3] = 0;

		switch (c)
		{
		case 0: // top
		case 1: // left
		case 2: // front
		case 3: // right 
		case 4: // back
		case 5: // bottom
		default:
			c;
		}

	}

	/*
	vector modeldata
	PLANET_SIDES = 6
	Map* sides[PLANET_SIDES]
	GLuint textures[PLANET_SIDES]
	for each side
		sides[side] = getMapFromPlanet(this, side)
		textures[side] = sides[side].getTexture(above)
		function addmap(side)
			add texture to atlas in correct position
		end function
		function sphereface(side)
			make distorted face
			rotate to correct orientation
			set texture coordinates to atlas specific face
			push to modeldata
		end function
	end for
	*/
}

void generateMapMesh(Map *map)
{
	static int divisor = 1;
	int x;
	int y;
	int z;
	// = map->depth;
	//int zlevel = map->depth;
	//float fx;
	//float fy;
	//float fz;
	//float normalize;
	//if (z < 0) z = 0;
	//if (z > MAP_LEVELS) z = MAP_LEVELS-1;
	gamePointi location;
	//gamePointi testPoint;
	
	
	clock_t starttime;
	clock_t endtime;

	starttime = clock();

	//tileInfo result;
	tileInfo thisPoint;

	//map->Vertices()->clear();
	map->tempVertices->clear();

	//float r=0;
	//float g=0;
	//bool exist;
	vertex_t vertex;
	for (x = -1; x <= map->width; x++) 
	{
		for (y = -1; y <= map->height; y++)
		{
			for(z = -1; z <= map->depth; z++)
			{

			location.x = x;
			location.y = y;
			location.z = z;
			//eximap->getTileInfo(location).status;
			//if (false)
			thisPoint = map->getTileAdjacencyInfo(location);
			if (thisPoint.status == success) //Tile is Empty or out of scope
			{
				/*
				testPoint = location;
				testPoint.z++;
				result = map->getTileInfo(testPoint);*/
				/*fx = (float)x/map->width;
				fy = (float)y/map->height;
				fz = (float)z/map->depth;
				normalize = sqrt(fx*fx+fy*fy+fz*fz);*/
				vertex.normal[0] = 0.577f;
				vertex.normal[1] = 0.577f;
				vertex.normal[2] = 0.577f;
				vertex.color[0] = thisPoint.r;
				vertex.color[1] = thisPoint.g;
				vertex.color[2] = thisPoint.b;
				vertex.color[3] = 1.0f;
					
					//vertex.normal[0] = 0.0f;
					//vertex.normal[1] = 0.0f;
					//vertex.normal[2] = 1.0f;
					
				vertex.position[0] = location.x;
				vertex.position[1] = location.y;
				vertex.position[2] = location.z;

				if (thisPoint.top == false)
				{/*
					vertex.color[0] = thisPoint.r;
					vertex.color[1] = thisPoint.g;
					vertex.color[2] = thisPoint.b;
					vertex.color[3] = 1.0f;
					
					vertex.normal[0] = 0.0f;
					vertex.normal[1] = 0.0f;
					/vertex.normal[2] = 1.0f;
					
					vertex.position[0] = location.x;
					vertex.position[1] = location.y;
					vertex.position[2] = location.z;*/
					pushEdge(0, vertex, map->tempVertices);
				}
				/*
				testPoint = location;
				testPoint.z--;
				result = map->getTileInfo(testPoint);
				*/
				if (thisPoint.bottom == false)
				{/*
					vertex.color[0] = thisPoint.r;
					vertex.color[1] = thisPoint.g;
					vertex.color[2] = thisPoint.b;
					vertex.color[3] = 1.0f;
					//vertex.normal[0] = 1.0f;
					//vertex.normal[1] = 0.0f;
					//vertex.normal[2] = 0.0f;
					vertex.position[0] = location.x;
					vertex.position[1] = location.y;
					vertex.position[2] = location.z;*/
					pushEdge(1, vertex, map->tempVertices);
				}
				/*
				testPoint = location;
				testPoint.x++;
				result = map->getTileInfo(testPoint);
				*/
				if (thisPoint.right == false)
				{/*
					vertex.color[0] = thisPoint.r;
					vertex.color[1] = thisPoint.g;
					vertex.color[2] = thisPoint.b;
					vertex.color[3] = 1.0f;
					//vertex.normal[0] = -1.0f;
					//vertex.normal[1] = 0.0f;
					//vertex.normal[2] = 0.0f;
					vertex.position[0] = location.x;
					vertex.position[1] = location.y;
					vertex.position[2] = location.z;*/
					pushEdge(2, vertex, map->tempVertices);
				}
				/*
				testPoint = location;
				testPoint.x--;
				result = map->getTileInfo(testPoint);
				*/
				if (thisPoint.left == false)
				{/*
					vertex.color[0] = thisPoint.r;
					vertex.color[1] = thisPoint.g;
					vertex.color[2] = thisPoint.b;
					vertex.color[3] = 1.0f;
					//vertex.normal[0] = 0.0f;
					//vertex.normal[1] = 1.0f;
					//vertex.normal[2] = 0.0f;
					vertex.position[0] = location.x;
					vertex.position[1] = location.y;
					vertex.position[2] = location.z;*/
					pushEdge(3, vertex, map->tempVertices);
				}
				/*
				testPoint = location;
				testPoint.y++;
				result = map->getTileInfo(testPoint);
				*/
				if (thisPoint.back == false)
				{/*
					vertex.color[0] = thisPoint.r;
					vertex.color[1] = thisPoint.g;
					vertex.color[2] = thisPoint.b;
					vertex.color[3] = 1.0f;
					//vertex.normal[0] = 0.0f;
					//vertex.normal[1] = -1.0f;
					//vertex.normal[2] = 0.0f;
					vertex.position[0] = location.x;
					vertex.position[1] = location.y;
					vertex.position[2] = location.z;*/
					pushEdge(4, vertex, map->tempVertices);
				}
				/*
				testPoint = location;
				testPoint.y--;
				result = map->getTileInfo(testPoint);
				*/
				if (thisPoint.front == false)
				{/*
					vertex.color[0] = thisPoint.r;
					vertex.color[1] = thisPoint.g;
					vertex.color[2] = thisPoint.b;
					vertex.color[3] = 1.0f;
					//vertex.normal[0] = 0.0f;
					//vertex.normal[1] = 0.0f;
					//vertex.normal[2] = 1.0f;
					vertex.position[0] = location.x;
					vertex.position[1] = location.y;
					vertex.position[2] = location.z;*/
					pushEdge(5, vertex, map->tempVertices);
				}
				}
			}
			//else
		
		}
	}
	std::vector<vertex_t> *temp = map->vertex_data;
	map->vertex_data = map->tempVertices;
	map->tempVertices = temp;
	endtime;
	/*
	endtime = clock();
	clock_t dif = (endtime - starttime) * 1000 / CLOCKS_PER_SEC;
	printf ("\n buffer generated in %i ms\n", dif );
	*/

}


void drawSelector(gamePointi selector)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_DST_ALPHA);
	glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
	drawOcto(selector.x-1, selector.y-1, selector.z+1, 1.0f);
	glDisable(GL_BLEND);
}

void drawSelector(gamePointf selector)
{
	gamePointi realSelector;
	realSelector.x = selector.x;
	realSelector.y = selector.y;
	realSelector.z = selector.z;
	drawSelector(realSelector);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_DST_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
	drawOcto(selector.x-1, selector.y-1, selector.z+1, 1.0f);
	glDisable(GL_BLEND);
}