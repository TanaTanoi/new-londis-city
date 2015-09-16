//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Tana Tanoi
//
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "comp308.hpp"
#include "building.hpp"

using namespace std;
using namespace comp308;

int Building::generateRandomBuilding(std::vector<comp308::vec2> floor) {
	int n = floor.size();
	vector<vec3> bot;
	vector<vec3> top;
	float height = (float)(rand() % 3);
	for (vec2 v2 : floor) {
		bot.push_back(vec3(v2.x, 0, v2.y));
		top.push_back(vec3(v2.x, height, v2.y));
	}
	//int toReturn = glGenLists(1); //TODO find out what I wanna do with the lists here
	//glNewList(toReturn, GL_COMPILE);
	glBegin(GL_QUADS);
	

	/*N amount of walls*/
	for (int i = 0; i < n; i++) {
		vec3 topR = top[i];
		vec3 topL = top[(i + 1) % n];
		vec3 botL = bot[(i + 1) % n];
		vec3 botR = bot[i];
		glVertex3f(topR.x, topR.y, topR.z);
		glVertex3f(topL.x, topL.y, topL.z);
		glVertex3f(botL.x, botL.y, botL.z);
		glVertex3f(botR.x, botR.y, botR.z);
		//FIXME the normals need doing
		vec3 normal = cross((botL - topL), (topR - topL));
		glNormal3f(normal.x, normal.y, normal.z);

	}	
	/*Roof*/
	for (vec3 roofPart : top) {
		glVertex3f(roofPart.x, roofPart.y, roofPart.z);
	}
	glNormal3f(0, 1, 0);
	glEnd();
	//glEndList();
	//return toReturn;
	return 0;
}
