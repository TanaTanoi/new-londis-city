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

/*Generates a block from the given floor plan *floor from the given
 * height *elevation.
 * *floor must be a list of points where a wall is connected between
 * point i and point i +1 (where last and first point are connected)
 *
 */
int Building::generateBlock(std::vector<comp308::vec2> floor, float elevation) {
	int n = floor.size();
	vector<vec3> bot;
	vector<vec3> top;
	float height = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.0f)+1.0f;
	//float height = 1.0f;
	height+=elevation;
	for (vec2 v2 : floor) {
		bot.push_back(vec3(v2.x, elevation, v2.y));
		top.push_back(vec3(v2.x, height, v2.y));
	}
	//int toReturn = glGenLists(1); //TODO find out what I wanna do with the lists here
	//glNewList(toReturn, GL_COMPILE);
	glBegin(GL_QUADS);

	/*n amount of walls*/
	for (int i = 0; i < n; i++) {
		if(i %2==0){
			glColor3f(0.7f,0.1f,0.4f);
		}else{
			glColor3f(0.1f,0.2f,0.9f);
		}
		vec3 topl = top[i];
		vec3 topr = top[(i + 1) % n];
		vec3 botr = bot[(i + 1) % n];
		vec3 botl = bot[i];
		vec3 normal = cross((botl - topr), (topl - topr));
		normal = normalize(normal);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(topr.x, topr.y, topr.z);
		glVertex3f(topl.x, topl.y, topl.z);
		glVertex3f(botl.x, botl.y, botl.z);
		glVertex3f(botr.x, botr.y, botr.z);

	}
	glEnd();
	glBegin(GL_POLYGON);
	/*roof*/
	glColor3f(0,0,0);
	glNormal3f(0, 1, 0);
	for (vec3 roofpart : top) {
		glVertex3f(roofpart.x, roofpart.y, roofpart.z);
	}
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0, -1, 0);
	for (vec3 floorPoint: bot) {
		glVertex3f(floorPoint.x, floorPoint.y, floorPoint.z);
	}

	glEnd();

	if(rand()%10 > 2){

		generateBlock(shrinkPoints(floor),height);

	}


	//glEndList();
	//return toReturn;
	return 0;
}


vector<vec2> Building::shrinkPoints(std::vector<vec2> points){
	vec2 mid = centerPoint(points);
	vector<vec2> smallPoints;
	float dist = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.2f)+0.1f;
	for(vec2 v2:points){
		vec2 diff = (mid - v2)*dist;

		smallPoints.push_back(v2+diff);
	}
	return smallPoints;
}
/*Returns a vec2 that represents a point in the center of the
 * shape created by *points.
 */
vec2 Building::centerPoint(vector<vec2> points){
	vec2 max = points[0];
	vec2 min = max;
	for(int i = 1; i < points.size();i++){
		if(points[i].x > max.x && points[i].y > max.y){
			max = points[i];
		}
		if(points[i].x < min.x && points[i].y < min.y){
			min= points[i];
		}
	}
	vec2 mid = ((max - min)/2) + min;
	return mid;
}
