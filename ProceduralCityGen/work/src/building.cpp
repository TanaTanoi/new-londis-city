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
#include "imageLoader.hpp"
#include "shaderLoader.hpp"
#include "building.hpp"
#include "generator.h"
#include "utility.hpp"
using namespace std;
using namespace comp308;

Building::Building(){
//	initShader();
}


GLuint tex_wall[2][TOTAL_WALL_TEXTURES];
GLuint tex_window[2][TOTAL_WINDOW_TEXTURES];
GLuint tex_door[2][2];
GLuint grass;
GLuint road;
GLuint conc;
void Building::initShader() {
	//Gets stuck here, i.e. cout won't print
	g_shader = makeShaderProgram("../work/res/shaders/shaderDemo.vert", "../work/res/shaders/shaderDemo.frag");
	cout<<"Init shader"<<endl;
}

/*Loads the given texture into the given GLunit */
void loadTexture(GLuint texture, const char* filename){

	image tex(filename);

	glBindTexture(GL_TEXTURE_2D,texture);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE , GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer());
}

void Building::initTexture() {

	initShader();
	glUseProgram(g_shader);
	glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);

	glGenTextures(TOTAL_WALL_TEXTURES, tex_wall[0]);
	glGenTextures(TOTAL_WALL_TEXTURES, tex_wall[1]);


	glGenTextures(TOTAL_WINDOW_TEXTURES, tex_window[0]);
	glGenTextures(1, tex_door[0]);
	glGenTextures(1, &grass);
	glGenTextures(1, &road);
	glGenTextures(1, &conc);

	loadTexture(tex_wall[0][0], "../work/res/textures/highrise001.jpg");
	loadTexture(tex_wall[0][1], "../work/res/textures/highrise002.jpg");
	loadTexture(tex_wall[0][2], "../work/res/textures/highrise003.jpg");
	loadTexture(tex_wall[0][3], "../work/res/textures/highrise004.jpg");

	loadTexture(tex_wall[1][0], "../work/res/textures/brick001.jpg");
	loadTexture(tex_wall[1][1], "../work/res/textures/brick002.jpg");
	loadTexture(tex_wall[1][2], "../work/res/textures/brick003.jpg");
	loadTexture(tex_wall[1][3], "../work/res/textures/brick004.jpg");

	loadTexture(tex_window[0][0], "../work/res/textures/glass01.jpg");
	loadTexture(tex_window[0][1], "../work/res/textures/window02.jpg");
	loadTexture(tex_window[0][2], "../work/res/textures/window03.jpg");
	loadTexture(tex_window[0][3], "../work/res/textures/window04.jpg");
	loadTexture(tex_window[0][4], "../work/res/textures/window05.jpg");

	loadTexture(tex_door[0][0], "../work/res/textures/wooddoor02.jpg");

	loadTexture(grass, "../work/res/textures/grass001.jpg");

	loadTexture(road, "../work/res/textures/road01.jpg");
	loadTexture(conc, "../work/res/textures/concrete.jpg");
}
/*Draws a thing of a given size*/
void Building::drawGround(float size){
	float floor = -0.01f;
	glBindTexture(GL_TEXTURE_2D,conc);
	glBegin(GL_QUADS);
	//Floor
	glNormal3f(0, 1, 0);
	glTexCoord2f(0,0);
	glVertex3f(-size,floor,-size);
	glTexCoord2f(0,size);
	glVertex3f(-size,floor,size);
	glTexCoord2f(size,size);
	glVertex3f(size,floor,size);
	glTexCoord2f(size,0);
	glVertex3f(size,floor,-size);
	glEnd();
}


float Building::extendBuilding(std::vector<comp308::vec2> floor, float elevation) {
	int n = floor.size();
	vec2 mid = Generator::centerPoint(floor);
	if(abs(length(floor[0]-mid))<EXTRUDE_THRESHOLD&&elevation>0.0f){return elevation;}//Do nothing if it is too small
	vector<vec3> bot;
	vector<vec3> top;
	/*Height is a value between 1 and 1.5 + the elevation (so height-elevation is the change in Y)*/
	float height = BLOCK_SIZE;//static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.5f)+1.0f;
	height+=elevation;
	for (vec2 v2 : floor) {

		bot.push_back(vec3(v2.x, elevation, v2.y));
		top.push_back(vec3(v2.x, height, v2.y));
	}

	/*n amount of walls*/

	for (int i = 0; i < n; i++) {
		//->

		vec3 topl = top[i];
		vec3 topr = top[(i + 1) % n];
		vec3 botr = bot[(i + 1) % n];
		vec3 botl = bot[i];


		vec3 normal = cross((botl - topr), (topl - topr));
		if (length(normal) > 0) {
			normal = normalize(normal);
		}
		float len = abs(length(botl-botr));	//length of the wall

		len /=tex_wall_width;	//the proportion of the wall
								//->
		//Use texture

		glBindTexture(GL_TEXTURE_2D,tex_wall[cur_tex_wall][cur_tex_wall_num]);
		glBegin(GL_QUADS);
			glNormal3f(normal.x, normal.y, normal.z);//baisc normal, probably the same as the other stuff
			glTexCoord2f(len,0);
			glVertex3f(topr.x, topr.y, topr.z);
			glTexCoord2f(len,1);
			glVertex3f(botr.x, botr.y, botr.z);
			glTexCoord2f(0,1);
			glVertex3f(botl.x, botl.y, botl.z);
			glTexCoord2f(0,0);
			glVertex3f(topl.x, topl.y, topl.z);
		glEnd();

		//Generate windows
		//if(cur_tex_wall!=SKYSCRAPER)
			//generateWindows(floor[i], floor[(i + 1) % n], elevation, normal);

	}


	glBegin(GL_TRIANGLES);
	/*Render a roof here, or at least a top*/
	glColor3f(0,0,0);
	glNormal3f(0, 1, 0);
	vec3 mid3d = vec3(mid.x,height,mid.y);
	for(int i = 0; i < n;i++){
		vec3 p1 = top[i];
		vec3 p2 = top[(i+1)%n];
		glVertex3f(mid3d.x,mid3d.y,mid3d.z);
		glVertex3f(p2.x,p2.y,p2.z);
		glVertex3f(p1.x,p1.y,p1.z);


	}
	glEnd();

	return height;
}

void Building::renderWindows(vector<vec2> floor, float elevation) {
	vec2 mid = Generator::centerPoint(floor);
	if (abs(length(floor[0] - mid))<EXTRUDE_THRESHOLD) { return; }
	int n = floor.size();
	glBindTexture(GL_TEXTURE_2D, tex_window[cur_tex_win][cur_tex_win_num]);
	for (int i = 0; i < n; i++) {
		//create a normal from the direction vector of the floors and the up vector, which is contant
		vec2 dir = (floor[(i + 1) % n] - floor[i]);
		vec3 normal = vec3(dir.x, 0, dir.y);
		normal = cross(vec3(0, 1, 0),normal);
		generateWindows(floor[i], floor[(i + 1) % n], elevation, normal);
	}
}


/*Generates a building based on the instructions from the input string,
 * and the floor points.
 */
void Building::generateFromString(std::vector<comp308::vec2> floor,string input) {
	/*Generate first floor REPLACE ME ONCE TEXTURES ARE IN AND WE CAN HAVE AN ACTUAL FLOOR*/
	//float height = extendBuilding(floor, 0.0f);
	float height = FOUNDATION_SIZE;
	for (int i = 0; i < input.length(); i++) {
		switch (input[i]) {
		case 'S':
			floor = Generator::shrinkPoints(floor);
			break;
		case 'E':
			//Add windows and then extrude the building.
			if (cur_tex_wall != SKYSCRAPER)
				renderWindows(floor, height);
			height = extendBuilding(floor, height);

			break;
		case 'R':
			if(rand()%3==0){
				generatePointRoof(floor, height);
			}else{
				generateFlatPointRoof(floor,height);
			}
			break;
		case 'D':
			if (floor.size() == 4) { floor = Generator::subdivide(floor)[rand()%2]; }
			break;
		case '$':
			vec3 normal = vec3(floor[1].x, 0, floor[1].y) - vec3(floor[0].x, 0, floor[0].y);
			normal = cross(normal, vec3(0, 1, 0));
			generateDoor(floor[0], floor[1],FOUNDATION_SIZE, -normal);
			height = extendBuilding(floor, height);
		}

	}
}

/*Generates a pointed roof from the input*/
void Building::generatePointRoof(std::vector<comp308::vec2> points, float elevation){
	float height = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.3f)+0.1f;
	vec2 mid = Generator::centerPoint(points);
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
/*Generates a set of windows along the line a - b at the elevation provided. */
void Building::generateWindows(vec2 a, vec2 b, float elevation, vec3 normal) {

	float bottom = elevation+(BLOCK_SIZE*0.2f);						//the bottom and top of the windows
	float top = elevation + (BLOCK_SIZE - (BLOCK_SIZE*0.2));		//

	float dist = distance(a,b);				//length of the wall

	float margin = dist;					//this will become the start point along the wall
	int i;									//i is the amount of window spaces available
	for (i = 0; margin > WINDOW_WIDTH;i++) {//keep going until we have no more window spaces
		margin -= WINDOW_WIDTH;
	}
	//margin is now the left over space that doesn't become a window
	margin /= 2.0f;//margin is now the start point along the wall's distance (not sure if /4 or /2)
	if (i % 2 == 0) {	//if even, then we want to add half the window_width to the margin to offset it
		margin += WINDOW_WIDTH / 2.0f;
		i--;			//i is now uneven (which is good)
	}

	//i = (int)(i / 2) + 1;	//this is the amount of windows we will place
	vec2 direction = b - a;
	if (length(direction) <= 1) {
		direction *= (1 / length(direction));
	}else{
		direction = normalize(direction);
	}

	glBegin(GL_QUADS);
	for (int j = 0; j < i; j+=2) {
		vec2 start = (direction*margin) + (direction*WINDOW_WIDTH*j)+a;
		vec2 end = (direction*WINDOW_WIDTH)+start;

		vec3 topL = vec3(start.x, top, start.y)		 + normal*0.001f;
		vec3 topR = vec3(end.x, top, end.y)			 + normal*0.001f;
		vec3 botR = vec3(end.x, bottom, end.y)		 + normal*0.001f;
		vec3 botL = vec3(start.x, bottom, start.y)	 + normal*0.001f;

		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f(1,0);
		glVertex3f(topR.x, topR.y, topR.z);
		glTexCoord2f(1,1);
		glVertex3f(botR.x, botR.y, botR.z);
		glTexCoord2f(0,1);
		glVertex3f(botL.x, botL.y, botL.z);
		glTexCoord2f(0,0);
		glVertex3f(topL.x, topL.y, topL.z);
	}
	glEnd();
}
/*Finds the center point and generates a door along this wall*/
void Building::generateDoor(vec2 a, vec2 b,float elevation, vec3 normal) {
	glBindTexture(GL_TEXTURE_2D, tex_door[0][0]);
	float bottom = elevation;
	float top =  (BLOCK_SIZE - (BLOCK_SIZE*0.1));		//top of the door

	float dist = (float)hypot(a.x - b.x, b.y - a.y);				//length of the wall
	if (dist < 0.2) { return; }
	vec2 direction = a - b;
	//make it a unit vector pointing from A to B
	if (length(direction) <= 1) {
		direction *= (1 / length(direction));
	}else{
		direction = normalize(direction);
	}
	vec2 center = b+direction*dist*0.5;
	glBegin(GL_QUADS);
	vec2 start = center + direction*0.1;
	vec2 end = center - direction*0.1;
	/*The 0.001 is to make it slightly jutt out from the wall*/
	vec3 topL = vec3(start.x, top, start.y) + normal*0.001f;
	vec3 topR = vec3(end.x, top, end.y) + normal*0.001f;
	vec3 botR = vec3(end.x, bottom, end.y) + normal*0.001f;
	vec3 botL = vec3(start.x, bottom, start.y) + normal*0.001f;
	glNormal3f(normal.x, normal.y, normal.z);
	glTexCoord2f(1, 0);
	glVertex3f(topR.x, topR.y, topR.z);
	glTexCoord2f(1, 1);
	glVertex3f(botR.x, botR.y, botR.z);
	glTexCoord2f(0, 1);
	glVertex3f(botL.x, botL.y, botL.z);
	glTexCoord2f(0, 0);
	glVertex3f(topL.x, topL.y, topL.z);
	glEnd();
}

/*Generates a truncated pointed roof*/
void Building::generateFlatPointRoof(std::vector<comp308::vec2> points, float elevation){
	int n = points.size();
		vector<vec3> bot;
		vector<vec2> topPlan = Generator::shrinkPoints(Generator::shrinkPoints(points));
		vector<vec3> top;
		/*Height is a value between 1 and 1.5 + the elevation (so height-elevation is the change in Y)*/
		vec2 mid = Generator::centerPoint(points);
		float height = abs(length(points[0]-mid))/4.0f;//static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.5f)+1.0f;
		height+=elevation;
		for (int i = 0;i<n;i++) {
			bot.push_back(vec3(points[i].x, elevation, points[i].y));
			top.push_back(vec3(topPlan[i].x, height, topPlan[i].y));
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
			glNormal3f(normal.x, normal.y, normal.z);//baisc normal, probably the same as the other stuff
			glTexCoord2f(1.0f,0.0f);
			glVertex3f(topr.x, topr.y, topr.z);
			glTexCoord2f(1.0f,1.0f);
			glVertex3f(botr.x, botr.y, botr.z);
			glTexCoord2f(0.0f,1.0f);
			glVertex3f(botl.x, botl.y, botl.z);
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(topl.x, topl.y, topl.z);

		}
		glEnd();
		glBegin(GL_TRIANGLES);
		/*roof*/
		glColor3f(0,0,0);
		glNormal3f(0, 1, 0);
		vec3 mid3d = vec3(mid.x,height,mid.y);
			for(int i = 0; i < n;i++){
				vec3 p1 = top[i];
				vec3 p2 = top[(i+1)%n];
				glVertex3f(p2.x,p2.y,p2.z);
				glVertex3f(p1.x,p1.y,p1.z);
				glVertex3f(mid3d.x,mid3d.y,mid3d.z);
			}
		glEnd();
		glBegin(GL_TRIANGLES);
		glNormal3f(0, -1, 0);
		for (vec3 floorPoint: bot) {
			glVertex3f(floorPoint.x, floorPoint.y, floorPoint.z);
		}

		glEnd();

}


/*Returns a display list containing fully random buildings */
int Building::generateRandomBuildings() {
	/*Generate a random bunch of floor plans*/
	int toReturn = glGenLists(1);
	float size = 2.4f;
	float disp = 1.2f;
	float building_size = 1.2f;
	glNewList(toReturn, GL_COMPILE);
	glUseProgram(g_shader);
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
	int current = 0;
	for (int i = 0; i < input.length(); i++) {
		int c = input[i];
		current += c;
	}
	cout << "Hash code for " << input << " is " << current << endl;
	return current;
}
/*Generates a single lot of buildings from a given vector of section and returns a display list
 * Applies a seed depending on the input string
 * */
int Building::generateBuildingsFromSections(string input, vector<util::section> sections,float range, vec2 min) {
	//Test code TODO REMOVE this was for testing combination of floor plans
	/* if(true){
		vector<vec2> floorA = Generator::generateFloorPlan(vec2(0,0),1.0f,6);
		vector<vec2> floorB = Generator::generateFloorPlan(vec2(0,0),1.0f,6);
		vector<vec2> floorC = Generator::combinePlans(floorA,floorB);

		int newList = glGenLists(1);
		glNewList(newList, GL_COMPILE);
		glDisable(GL_TEXTURE_2D);
		glUseProgram(0);

		int n = floorA.size();
		glBegin(GL_LINES);
			for(int i =0;i<n;i++){
				glColor3f(i/n,i/n,i/n);
				glLineWidth(i);
				glVertex3f(floorA[i].x,1.0f,floorA[i].y);
				glVertex3f(floorA[(i+1)%n].x,1.0f,floorA[(i+1)%n].y);
			}
			glEnd();
			n = floorB.size();
			glBegin(GL_LINES);
			for(int i =0;i<n;i++){
				glColor3f(i/n,i/n,i/n);
				glLineWidth(i);
				glVertex3f(floorB[i].x,1.0f,floorB[i].y);
				glVertex3f(floorB[(i+1)%n].x,1.0f,floorB[(i+1)%n].y);
			}
			glEnd();
			n = floorC.size();
			glBegin(GL_LINES);
			for(int i =0;i<n;i++){
				glColor3f(i/n,i/n,i/n);
			glLineWidth(i);
			glVertex3f(floorC[i].x,3.0f,floorC[i].y);
			glVertex3f(floorC[(i+1)%n].x,3.0f,floorC[(i+1)%n].y);
		}
		glEnd();
		glEndList();
		return newList;
	}*/

	vector<buildingLOD> buildings;
	int randStringInc = Building::basicHashcode(input);	//Generate seed from input
	srand(randStringInc);								//Reset srand
	return generateBuildingsFromSections(sections,range,min);
}

/*Generates a display list containing a lot of buildings generated by the sections
 * Assumes that srand has already been called (i.e. seed has been chosen)*/
int Building::generateBuildingsFromSections(vector<util::section> sections,float range, vec2 min) {
	vector<buildingLOD> buildings;
	vector<buildingParams> params = Generator::sectionsToParams(sections,heightmap_points,range,min);
	for (int i = 0; i < params.size(); i++) {
		buildingLOD building;
		generateBuilding(&params[i], &building);
		buildings.push_back(building);
	}
	int newList = glGenLists(1);
	glNewList(newList, GL_COMPILE);
	glUseProgram(g_shader);
	for (buildingLOD b : buildings) {
		glCallList(b.low);
	}
	glEndList();
	return newList;
}

/* Create a single building based on the given parameters
 * This is used for show casing a single type of building.
 * Types:
 * 0 : 3 Tier
 * 1 : Cut edges L-System
 * 2 : N-Sided Shape L-System
 * 3 : Combined Shape  L-System
 * 4 : Standard L-System
 * 5 : Combined Shape L-System
 * 6 : Modern Style Building
 * 7 : Park
 */
int Building::generateSingleBuilding(int type,float size) {

	cur_tex_wall = rand()%2;srand(rand());
	cur_tex_wall_num = rand()%TOTAL_WALL_TEXTURES;srand(rand());
	if (cur_tex_wall != SKYSCRAPER) {
		cur_tex_win_num = rand() % TOTAL_WINDOW_TEXTURES;
	}
	int toReturn = glGenLists(1);
	vec2 center = vec2(0, 0);
	vector<vec2> floorPlan = vector<vec2>();
	floorPlan.push_back(vec2(size, -size));
	floorPlan.push_back(vec2(size, size));
	floorPlan.push_back(vec2(-size, size));
	floorPlan.push_back(vec2(-size, -size));
	glNewList(toReturn, GL_COMPILE);
	glUseProgram(g_shader);
	switch (type) {
	case 0:

		generateResdientialBuilding(floorPlan, rand()%6+2);
		glEndList();
		return toReturn;
	case 1:
		floorPlan = Generator::cutEdges(floorPlan);
		generateFromString(floorPlan, Generator::generateRandomBuildingString(rand() % 6 + 3));
		glEndList();
		return toReturn;
	case 2:
		floorPlan = Generator::generateFloorPlan(center, size, (rand() % 4) + 4);
		generateFromString(floorPlan, Generator::generateRandomBuildingString(rand() % 6 + 3));
		glEndList();
		return toReturn;
	case 3:
		floorPlan = Generator::generateFloorPlan(center, size*0.8, (rand() % 4) + 4);
		floorPlan = Generator::combinePlans(floorPlan, Generator::generateFloorPlan(center, size*0.8, (rand() % 4) + 4));
	case 4:
		generateFromString(floorPlan, Generator::generateRandomBuildingString(rand()%6+3));
		glEndList();
		return toReturn;
	case 5:
		generateFromString(floorPlan, Generator::generateRandomBuildingString(rand() % 6 + 3));
		glEndList();
		return toReturn;
	case 6:
		generateModernBuilding(floorPlan, center, size);
		glEndList();
		return toReturn;
	case 7:
		generatePark(floorPlan);
		glEndList();
		return toReturn;
	}
	return toReturn;
}


/*Takes a parameters struct and creates a building based on that.
Result display lists are saved in the *result struct */
void Building::generateBuilding(buildingParams* parameters, buildingLOD* result) {
	vector<vec2> floorPlan = parameters->boundingArea;
	//Get min distance
	vec2 center = Generator::centerPoint(floorPlan);
	float minDist = 100000.0f;
	for (vec2 v : floorPlan) {
		minDist = min(minDist, (float)hypot(v.x-center.x,v.y-center.y));
	}
	minDist*=0.8f;//shrink minDist a little to be safe
	srand(parameters->seed);
	//floorPlan = Generator::generateModernFloorPlan(center,minDist);
	int chance = rand()%100+1;
	if (chance < 10) {
		glNewList(result->low, GL_COMPILE);
		generatePark(floorPlan);
		glEndList();
		return;
	}else if (chance < 40) {
		//40% for residential. Standard building type
		result->low = glGenLists(1);
		glNewList(result->low, GL_COMPILE);
		generateResdientialBuilding(floorPlan,parameters->height);
		//generatePark(floorPlan);
		glEndList();
		return;
	}else if (chance < 90) {
		//50% chance to be square type (regular,shrink, 4cut) Varied slightly, but most common type
		if (chance < 50) {
			//10% chance for shrink
			floorPlan = Generator::shrinkPoints(floorPlan);
		}else if (chance < 60) {
			//10%chance for cut edges
			if(floorPlan.size()==4)
				floorPlan = Generator::cutEdges(floorPlan);
		}//else 40% chance to stay as given style
	}else if(chance < 100){
		//10% chance to be different shape (modern, single different shape)
		if (chance < 95) {
			//5% single different shape
			floorPlan = Generator::generateFloorPlan(center, minDist, (rand() % 4) + 4);
		}else if (chance < 100) {
			//2% chance to be crazyish
			floorPlan = Generator::generateFloorPlan(center, minDist*0.6, (rand() % 4) + 4);
			floorPlan = Generator::combinePlans(floorPlan, Generator::generateFloorPlan(center, minDist*0.6, (rand() % 4) + 4));
		}else{
			//3% modern crazy style
			glNewList(result->low, GL_COMPILE);
			generateModernBuilding(parameters->boundingArea, center, minDist);
			glEndList();
			return;
		}
	}

	cur_tex_wall = parameters->b_type;
	cur_tex_wall_num = rand()%TOTAL_WALL_TEXTURES;
	if (cur_tex_wall != SKYSCRAPER) {
		cur_tex_win_num = rand() % TOTAL_WINDOW_TEXTURES;
	}
	result->low = glGenLists(1);
	glNewList(result->low, GL_COMPILE);
		generateFromString(floorPlan, Generator::generateRandomBuildingString(parameters->height));
	glEndList();

}

void Building::generateResdientialBuilding(vector<vec2> points,int height) {
	//set first two points to subdivion of original floor
	vector<vector<vec2>> tiers = Generator::subdivide(points);
	vector<vec2> temp = tiers[0];
	tiers = Generator::subdivide(tiers[1]);
	tiers.push_back(temp);

	srand(rand());
	int n = rand() % height + 5;		//between 5 and 15
	float cur_elev = FOUNDATION_SIZE;
	//extend main tier random amount of times
	for(int i = 0; i < n ;i++){
		if (cur_tex_wall != SKYSCRAPER&&i>0)
			renderWindows(tiers[2],cur_elev);
		cur_elev = extendBuilding(tiers[2], cur_elev);
	}
	n = rand() % (n-((int)(n *0.75) -1)) + (int)(n *0.75);//between 3/4 and n (inclusive)
	cur_elev = FOUNDATION_SIZE;
	for (int i = 0; i < n; i++) {
		if (cur_tex_wall != SKYSCRAPER)
			renderWindows(tiers[0], cur_elev);
		cur_elev = extendBuilding(tiers[0], cur_elev);
	}
	n = rand() % (n - ((int)(n *0.75) - 1)) + (int)(n *0.75);//between 3n/4 and n (inclusive)
	cur_elev = FOUNDATION_SIZE;
	for (int i = 0; i < n; i++) {
		if (cur_tex_wall != SKYSCRAPER&&i>0)
			renderWindows(tiers[1], cur_elev);
		cur_elev = extendBuilding(tiers[1], cur_elev);
	}

}

void Building::generateBlock(util::section bounding, float elevation){
	vector<vec2> floor = Generator::sectionToPoints(bounding);
	int n = floor.size();
	vec2 mid = Generator::centerPoint(floor);
	vector<vec3> bot;
	vector<vec3> top;
	/*Height is a value between 1 and 1.5 + the elevation (so height-elevation is the change in Y)*/
	float height = FOUNDATION_SIZE;//static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.5f)+1.0f;
	height+=elevation;
	for (vec2 v2 : floor) {

		bot.push_back(vec3(v2.x, elevation, v2.y));
		top.push_back(vec3(v2.x, height, v2.y));
	}


	for (int i = 0; i < n; i++) {
		//->

		vec3 topl = top[i];
		vec3 topr = top[(i + 1) % n];
		vec3 botr = bot[(i + 1) % n];
		vec3 botl = bot[i];


		vec3 normal = cross((botl - topr), (topl - topr));
		if (length(normal) > 0) {
			normal = normalize(normal);
		}
		float len = abs(length(botl-botr));	//length of the wall

		len /=tex_wall_width;	//the proportion of the wall
		//Use texture
		glBindTexture(GL_TEXTURE_2D,conc);
		glBegin(GL_QUADS);
		glNormal3f(normal.x, normal.y, normal.z);//baisc normal, probably the same as the other stuff
		glTexCoord2f(len,0);
		glVertex3f(topr.x, topr.y, topr.z);
		glTexCoord2f(len,1);
		glVertex3f(botr.x, botr.y, botr.z);
		glTexCoord2f(0,1);
		glVertex3f(botl.x, botl.y, botl.z);
		glTexCoord2f(0,0);
		glVertex3f(topl.x, topl.y, topl.z);
		glEnd();
	}
	glBegin(GL_TRIANGLES);
	/*Render a roof here, or at least a top*/
	glColor3f(0,0,0);
	glNormal3f(0, 1, 0);
	vec3 mid3d = vec3(mid.x,height,mid.y);
	for(int i = 0; i < n;i++){
		vec3 p1 = top[i];
		vec3 p2 = top[(i+1)%n];
		glVertex3f(mid3d.x,mid3d.y,mid3d.z);
		glVertex3f(p2.x,p2.y,p2.z);
		glVertex3f(p1.x,p1.y,p1.z);
	}
	glEnd();
}


void Building::generateModernBuilding(vector<vec2> points,vec2 mid, float minDist) {

	vector<vector<vec2>> levels = vector<vector<vec2>>();
	levels.push_back(Generator::generateFloorPlan(mid, minDist, rand() % 4 + 4));//add base
	srand(rand());
	//generate 2 to 5 plan changes
	for (int i = 0; i < rand() % 3 + 2; i++) {
		cout<<"Level " << i <<" has " << levels[i].size() << "edges"<<endl;
		srand(rand());
		float xmul = rand() % 2 - 1;//-1 to 1;
		srand(rand());
		float ymul = rand() % 2 - 1;//-1 to 1;
		srand(rand());
		vec2 offset = vec2(xmul*(minDist/2.0f),(minDist/2.0f)*ymul);
		//in this case, i is the most recently added level (i.e. the largest floorplan)
		vector<vec2> toAdd =Generator::combinePlans(levels[i], Generator::generateFloorPlan(mid+offset, minDist, rand() % 4 + 4));
		//if the plan didn't combine, try again with another plan
		if(toAdd.size() == levels[i].size()){
			srand(rand());
			Generator::combinePlans(levels[i], Generator::generateFloorPlan(mid+offset, minDist, rand() % 4 + 4));
		}
		levels.push_back(toAdd);
	}
	//the end of levels now contains the ground floor

	float elevation = FOUNDATION_SIZE;
	for (int i = levels.size() - 1; i >= 0;) {
		srand(rand());
		if (cur_tex_wall != SKYSCRAPER)
			renderWindows(levels[i], elevation);
		elevation = extendBuilding(levels[i], elevation);
		if (rand() % 3 <= 1) {//66% chance to change floor plan
			i--;
		}
	}


}
/*Generates a park area. Grass with brick fences */
void Building::generatePark(vector<vec2> floor) {

	int n = floor.size();
	vector<vec2> boundingBox = Generator::getBoundingBox(floor);
	// / 2 for scaling purposes
	float bb_width = abs(boundingBox[0].x - boundingBox[1].x)*0.25;
	float bb_height = abs(boundingBox[0].y - boundingBox[1].y)*0.25f;

	glBindTexture(GL_TEXTURE_2D, grass);
	glBegin(GL_POLYGON);
	for (int i = n-1; i >= 0;i--){
		vec2 v = floor[i];
		//tex coord is the length
		glTexCoord2f((v.x-boundingBox[0].x)/bb_width, (v.y - boundingBox[0].y) / bb_height);
		glVertex3f(v.x, FOUNDATION_SIZE+0.001f, v.y);
	}

	glEnd();

	vec2 mid = Generator::centerPoint(floor);
	glBindTexture(GL_TEXTURE_2D,tex_wall[1][0]);

	for (int i = 0; i < n; i++) {
		vec2 dir = (floor[(i + 1) % n]-floor[i]);
		float dist = abs(length(dir));
		if(i==n%2){
			generateParkWall(floor[i],floor[i]+(dir*0.4),mid);
			generateParkWall(floor[(i + 1) % n] - (dir*0.4),floor[(i + 1) % n],mid);
		}else{
			generateParkWall(floor[i], floor[(i + 1) % n], mid);

		}
	}
}

void Building::generateParkWall(vec2 a, vec2 b, vec2 mid) {
	float height = 0.08f;
	float wall_width = 0.05f;
	//the vectors on the inner part of the wall. This prevents glitching textures
	vec2 inner_a = (mid-a)*wall_width + a;
	vec2 inner_b = (mid - b)*wall_width + b;

	float len = abs(length(a - b));	//length of the wall
	len /= tex_wall_width;	//the proportion of the wall
	float yLen = 0.5f;
	glBegin(GL_QUADS);

	//outer facing wall of fence
	glTexCoord2f(len, yLen);
	glVertex3f(b.x, FOUNDATION_SIZE, b.y);
	glTexCoord2f(0, yLen);
	glVertex3f(a.x, FOUNDATION_SIZE, a.y);
	glTexCoord2f(0, 0);
	glVertex3f(a.x, height, a.y);
	glTexCoord2f(len, 0);
	glVertex3f(b.x, height, b.y);

	//inner facing wall of fence
	glTexCoord2f(len, yLen);
	glVertex3f(inner_a.x, FOUNDATION_SIZE, inner_a.y);
	glTexCoord2f(0, yLen);
	glVertex3f(inner_b.x, FOUNDATION_SIZE, inner_b.y);
	glTexCoord2f(0, 0);
	glVertex3f(inner_b.x, height, inner_b.y);
	glTexCoord2f(len, 0);
	glVertex3f(inner_a.x, height, inner_a.y);

	//top of wall
	glVertex3f(inner_a.x, height, inner_a.y);
	glVertex3f(inner_b.x, height, inner_b.y);
	glVertex3f(b.x, height, b.y);
	glVertex3f(a.x, height, a.y);
	glEnd();

}


/*Renders a road between two points of a given width*/
void Building::generateRoad(vec2 a, vec2 b,float width){
	srand(a.x+b.x*a.y*b.y);
	float ground = ((float)rand()/RAND_MAX)*0.02+0.04;
	vec2 dir = (b-a);
//	a += normalize(dir) * 0.1;
//	b -= normalize(dir) * 0.1;
	vec3 dir3D = vec3(dir.x,0,dir.y);
	dir3D = normalize(dir3D);
	vec3 right3D = cross(dir3D, vec3(0,1,0));
	right3D = normalize(right3D);
	vec2 right = vec2(right3D.x,right3D.z);
	float leng = abs(length(dir));
	leng/=tex_wall_width;

	vec2 pointA = a;
	vec2 pointB = b;

	b-=normalize(dir)*(width);
	a+=normalize(dir)*(width);
	vector<vec2> endATri;
	endATri.push_back(a+(right*width));
	endATri.push_back(a-(right*width));
	endATri.push_back(pointA);
	vector<vec2> boundingBox = Generator::getBoundingBox(endATri);
	float bb_width = abs(boundingBox[0].x - boundingBox[1].x);
	float bb_height = abs(boundingBox[0].y - boundingBox[1].y);
	glBindTexture(GL_TEXTURE_2D, road);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i <3;i++){
		vec2 v = endATri[i];
		//tex coord is the length
		glTexCoord2f((v.x-boundingBox[0].x)/bb_width, (v.y - boundingBox[0].y) / bb_height);
		glVertex3f(v.x, ground, v.y);
	}
	glEnd();
	vector<vec2> endBTri;
	endBTri.push_back(b-(right*width));
	endBTri.push_back(b+(right*width));
	endBTri.push_back(pointB);
	boundingBox = Generator::getBoundingBox(endBTri);
	bb_width = abs(boundingBox[0].x - boundingBox[1].x);
	bb_height = abs(boundingBox[0].y - boundingBox[1].y);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i <3;i++){
		vec2 v = endBTri[i];
		//tex coord is the length
		glTexCoord2f((v.x-boundingBox[0].x)/bb_width, (v.y - boundingBox[0].y) / bb_height);
		glVertex3f(v.x, ground, v.y);
	}

	glEnd();

	vector<vec2> roadPoints = vector<vec2>();
	roadPoints.push_back(a+(right*width));
	roadPoints.push_back(b+(right*width));
	roadPoints.push_back(b-(right*width));
	roadPoints.push_back(a-(right*width));


	glBegin(GL_QUADS);
	glTexCoord2d(0,0);
	glVertex3f(roadPoints[0].x,ground,roadPoints[0].y);
	glTexCoord2d(0,leng);
	glVertex3f(roadPoints[1].x,ground,roadPoints[1].y);
	glTexCoord2d(1,leng);
	glVertex3f(roadPoints[2].x,ground,roadPoints[2].y);
	glTexCoord2d(1,0);
	glVertex3f(roadPoints[3].x,ground,roadPoints[3].y);
	glEnd();
}
