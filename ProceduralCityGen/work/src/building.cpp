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
#include "imageLoader.hpp"
#include "shaderLoader.hpp"
#include "generator.h"
using namespace std;
using namespace comp308;

Building::Building(){
//	initShader();
}


//GLuint g_texture = 0;

void Building::initShader() {
	//Gets stuck here, i.e. cout won't print
	g_shader = makeShaderProgram("../work/res/shaders/shaderDemo.vert", "../work/res/shaders/shaderDemo.frag");
	cout<<"Initd shader"<<endl;
}
void Building::initTexture() {//TODO this method
}

/*From the given floor plan, extrude to create a block of height 0.5f
 *
 * *floor must be a list of points where a wall is connected between
 * point i and point i +1 (where last and first point are connected)
 */
float Building::extendBuilding(std::vector<comp308::vec2> floor, float elevation) {
	//glUseProgram(g_shader);
	int n = floor.size();
	vector<vec3> bot;
	vector<vec3> top;
	/*Height is a value between 1 and 1.5 + the elevation (so height-elevation is the change in Y)*/
	float height = BLOCK_SIZE;//static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.5f)+1.0f;
	height+=elevation;
	for (vec2 v2 : floor) {
		bot.push_back(vec3(v2.x, elevation, v2.y));
		top.push_back(vec3(v2.x, height, v2.y));
	}

	glBegin(GL_QUADS);
	/*n amount of walls*/
	for (int i = 0; i < n; i++) {
		vec3 topl = top[i];
		vec3 topr = top[(i + 1) % n];
		vec3 botr = bot[(i + 1) % n];
		vec3 botl = bot[i];
		vec3 normal = cross((botl - topr), (topl - topr));
		normal = normalize(normal);
		//glTexCoord2d(0, 0);
		glNormal3f(normal.x, normal.y, normal.z);//baisc normal, probably the same as the other stuff
		glVertex3f(topl.x, topl.y, topl.z);
		glVertex3f(botl.x, botl.y, botl.z);
		glVertex3f(botr.x, botr.y, botr.z);
		glVertex3f(topr.x, topr.y, topr.z);
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

	return height;
}

/*Generates a building based on the instructions from the input string,
 * and the floor points.
 */
void Building::generateFromString(std::vector<comp308::vec2> floor,string input) {
	/*Generate first floor REPLACE ME ONCE TEXTURES ARE IN AND WE CAN HAVE AN ACTUAL FLOOR*/

	float height = extendBuilding(floor, 0.0f);
	for (int i = 0; i < input.length(); i++) {
		switch (input[i]) {
		case 'S':
			floor = shrinkPoints(floor);
			break;
		case 'E':
			height = extendBuilding(floor, height);
			break;
		case 'R':
			if(rand()%2==0){
				generatePointRoof(floor, height);
			}else{
				generateFlatPointRoof(floor,height);
			}
			return;
		}

	}
}

/*Generates a pointed roof from the input*/
void Building::generatePointRoof(std::vector<comp308::vec2> points, float elevation){
	float height = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.3f)+0.3f;
	vec2 mid = centerPoint(points);
	vec3 peak = vec3(mid.x, elevation +height,mid.y);
	int n = points.size();
	glBegin(GL_TRIANGLES);
		for (int i = 0; i < n; i++) {
			glVertex3f(points[i].x, elevation, points[i].y);
			glVertex3f(peak.x, peak.y, peak.z);
			glVertex3f(points[(i + 1) % n].x, elevation, points[(i + 1) % n].y );
		}
	glEnd();
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		glVertex3f(peak.x, peak.y, peak.z);
		glVertex3f(peak.x, peak.y+0.3f, peak.z);
	glEnd();
}

/*Generates a truncated pointed roof*/
void Building::generateFlatPointRoof(std::vector<comp308::vec2> points, float elevation){
	int n = points.size();
		vector<vec3> bot;
		vector<vec2> topPlan = shrinkPoints(shrinkPoints(points));
		vector<vec3> top;
		/*Height is a value between 1 and 1.5 + the elevation (so height-elevation is the change in Y)*/
		float height = 0.2f;//static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.5f)+1.0f;
		height+=elevation;
		for (int i = 0;i<n;i++) {
			bot.push_back(vec3(points[i].x, elevation, points[i].y));
			top.push_back(vec3(topPlan[i].x, height, topPlan[i].y));
		}

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
			glNormal3f(normal.x, normal.y, normal.z);//baisc normal, probably the same as the other stuff
			glVertex3f(topl.x, topl.y, topl.z);
			glVertex3f(botl.x, botl.y, botl.z);
			glVertex3f(botr.x, botr.y, botr.z);
			glVertex3f(topr.x, topr.y, topr.z);
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

}

/*Shrinks the input points by a factor of 0.15 and returns the result*/
vector<vec2> Building::shrinkPoints(std::vector<vec2> points){
	vec2 mid = centerPoint(points);
	vector<vec2> smallPoints;
	float dist = 0.15f;// static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.2f)+0.1f;
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
	float x = 0;
	float y = 0;

	for(int i = 0; i < points.size();i++){
		x+=points[i].x;
		y+=points[i].y;
	}
	y/=points.size();
	x/=points.size();
	return vec2(x,y);

}


/*Returns a display list containing fully random buildings */
int Building::generateRandomBuildings() {
	/*Generate a random bunch of floor plans*/
	int toReturn = glGenLists(1);
	float size = 2.4f;
	float disp = 1.2f;
	float building_size = 1.2f;
	glNewList(toReturn, GL_COMPILE);

	vector<vec2> points;
	for (float i = -size; i <= size; i += disp) {
		for (float j = -size; j <= i; j += disp) {
			points.clear();
			points.push_back(vec2(i, j));
			points.push_back(vec2(i + building_size, j));
			points.push_back(vec2(i + building_size, j + building_size));
			points.push_back(vec2(i, j + building_size));
			glColor3f(i + 1, j + 1, (i + j) / 2 + 1);
			glColor3f((i + size) / 2, (i + size) / 2, (i + size) / 2);
			extendBuilding(points, -1.0f);
		}
	}
	glEndList();
	return toReturn;
}

int Building::basicHashcode(string input) {
	//string current = "";//should be a string made of numbers by the end
	//for (int i = 0; i < input.length(); i++) {
	//	int c = input[i];
	//	cout << "c is " << c << endl;
	//	current += to_string(c);

	//}
	//return stoi(current);
	int current = 0;
	for (int i = 0; i < input.length(); i++) {
		int c = input[i];
		cout << "c is " << c << endl;
		current += c;

	}
	return current;
}

int Building::generateBuildingFromString(string input) {
	/*Generate a random bunch of floor plans*/
	int toReturn = glGenLists(1);
	float size = 8.0f;
	float disp = 1.0f;
	float building_size = 1.0f;
	vector<vec2> points;

	vector<buildingLOD> buildings;
	int randStringInc = Building::basicHashcode(input);
	for (float i = -size; i <= size; i += disp) {
		for (float j = -size; j <size; j += disp) {
			//square floor plan
			points.clear();
			points.push_back(vec2(i, j));
			points.push_back(vec2(i + building_size, j));
			points.push_back(vec2(i + building_size, j + building_size));
			points.push_back(vec2(i, j + building_size));
			buildingParams p;
			p.boundingArea = points;
			srand(randStringInc+=rand()%rand());
			p.seed = rand();// % ();
			cout << p.seed << " ";
			buildingLOD result;
			buildings.push_back(result);
			generateBuilding(&p,&result);
		}
		cout << endl;
	}
	glNewList(toReturn, GL_COMPILE);
	cout << "Total buildings: " << buildings.size() << endl;
	for (buildingLOD b : buildings) {
		glCallList(b.low);
	}
	glEndList();
	return toReturn;
}



/*Takes a parameters struct and creates a building based on that.
Result display lists are saved in the *result struct */
void Building::generateBuilding(buildingParams* parameters, buildingLOD* result) {
	vector<vec2> floorPlan = parameters->boundingArea;
	//Get min distance
	vec2 center = centerPoint(floorPlan);
	float minDist = 100000.0f;
	for (vec2 v : floorPlan) {
		minDist = min(minDist, hypot(v.x-center.x,v.y-center.y));
	}
	minDist /= 2;
	srand(parameters->seed);
	if ((rand()%2==0)) {//50% chance to generate differently shaped building
		srand(rand());
		floorPlan = Generator::generateFloorPlan(center,minDist, rand() % 4 + 4);
	}else{				//50% chance to generate building in same floor plan

	}
	result->low = glGenLists(1);
	glNewList(result->low, GL_COMPILE);
	generateFromString(floorPlan, Generator::generateRandomString(rand() % 4 + 1));//TODO fix this so the iterations are a function of height
	glEndList();

}

