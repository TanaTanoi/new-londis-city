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
using namespace std;
using namespace comp308;

Building::Building(){
//	initShader();
}


GLuint tex_wall[2][2];
GLuint tex_window[2][2];

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
	// Set our sampler (texture0) to use GL_TEXTURE0 as the source
	glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);
	for(int i = 0; i < 2;i++){
		glGenTextures(2, tex_wall[i]);
		glGenTextures(2, tex_window[i]);
	}
	loadTexture(tex_wall[0][0], "../work/res/textures/highrise001.jpg");
	loadTexture(tex_wall[0][1], "../work/res/textures/highrise002.jpg");
	loadTexture(tex_wall[1][0], "../work/res/textures/brick001.jpg");
	loadTexture(tex_wall[1][1], "../work/res/textures/brick002.jpg");
	loadTexture(tex_window[0][0], "../work/res/textures/glass01.jpg");
}

/*From the given floor plan, extrude to create a block of height 0.5f
 *
 * *floor must be a list of points where a wall is connected between
 * point i and point i +1 (where last and first point are connected)
 */
vector<vector<vec2>> Building::subdivide(vector<vec2> points) {
	vector<vector<vec2>> result;
	if (points.size() != 4) { result.push_back(points); result.push_back(points); return result; }
	float cutDist = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.5f) + 1.5f;
	vec2 cutP1 = ((points[1] - points[0]) / cutDist) + points[0];
	vec2 cutP2 = ((points[2] - points[3]) / cutDist) + points[3];
	result.push_back(vector<vec2>());
	result.push_back(vector<vec2>());

	int i = cutDist>=2.0f;
	result[i].push_back(cutP1);
	result[i].push_back(cutP2);
	result[i].push_back(points[3]);
	result[i].push_back(points[0]);

	result[!i].push_back(cutP2);
	result[!i].push_back(cutP1);
	result[!i].push_back(points[1]);
	result[!i].push_back(points[2]);

	return result;
}
float Building::extendBuilding(std::vector<comp308::vec2> floor, float elevation) {

	int n = floor.size();
	vec2 mid = Generator::centerPoint(floor);
	if(abs(length(floor[0]-mid))<EXTRUDE_THRESHOLD){return elevation;}//Do nothing if it is too small
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
		vec3 topl = top[i];
		vec3 topr = top[(i + 1) % n];
		vec3 botr = bot[(i + 1) % n];
		vec3 botl = bot[i];
		vec3 normal = cross((botl - topr), (topl - topr));
		normal = normalize(normal);

		float len = abs(length(botl-botr));	//length of the wall
		len /=tex_wall_width;	//the proportion of the wall

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
	cout << "Making " << endl;
	float height = extendBuilding(floor, 0.0f);
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
			if(rand()%2==0){
				generatePointRoof(floor, height);
			}else{
				generateFlatPointRoof(floor,height);
			}
			//return;
			break;
		case 'D':
			if (floor.size() == 4) { floor = subdivide(floor)[rand()%2]; }
			break;
		case '$':
			height = extendBuilding(floor, height);
		}

	}
}

/*Generates a pointed roof from the input*/
void Building::generatePointRoof(std::vector<comp308::vec2> points, float elevation){
	float height = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.3f)+0.3f;
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

	float dist = (float)hypot(a.x - b.x, b.y - a.y);				//length of the wall

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
		if((j/2)%2 == 0){
			glColor3f(1,0,0);
		}else{
			glColor3f(0,1,0);
		}
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

/*Generates a truncated pointed roof*/
void Building::generateFlatPointRoof(std::vector<comp308::vec2> points, float elevation){
	int n = points.size();
		vector<vec3> bot;
		vector<vec2> topPlan = Generator::shrinkPoints(Generator::shrinkPoints(points));
		vector<vec3> top;
		/*Height is a value between 1 and 1.5 + the elevation (so height-elevation is the change in Y)*/
		vec2 mid = Generator::centerPoint(points);
		float height = abs(length(points[0]-mid))/2.0f;//static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.5f)+1.0f;
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

int Building::generateBuildingFromString(string input) {
	int toReturn = glGenLists(1);

	/*Size of buildings and stuff for this thing*/
	float size = 10.0f;
	float disp = 1.5f;
	float building_size = 1.0f;
	vector<vec2> points;

	vector<buildingLOD> buildings;
	int randStringInc = Building::basicHashcode(input);	//Generate seed from input
	srand(randStringInc);								//Reset srand
	for (float i = -size; i <=size; i += disp) {
		for (float j = -size; j <=size; j += disp) {
			//create a bounding box-like area
			points.clear();
			points.push_back(vec2(i, j));
			points.push_back(vec2(i + building_size, j));
			points.push_back(vec2(i + building_size, j + building_size));
			points.push_back(vec2(i, j + building_size));
			buildingParams p;
			p.boundingArea = points;
			srand(randStringInc+=14);
			p.seed = rand();
			p.b_type = static_cast<building_type>(rand()%2);
			buildingLOD result;
			buildings.push_back(result);
			generateBuilding(&p,&result);
		}
	}
	glNewList(toReturn, GL_COMPILE);
	cout << "Total buildings: " << buildings.size() << endl;
	//Compile all buildings into one display list and return
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
	vec2 center = Generator::centerPoint(floorPlan);
	float minDist = 100000.0f;
	for (vec2 v : floorPlan) {
		minDist = min(minDist, (float)hypot(v.x-center.x,v.y-center.y));
	}
	//minDist /= 1.5f;
	minDist*=1.0f;
	srand(parameters->seed);
	//floorPlan = Generator::generateModernFloorPlan(center,minDist);
	int chance = rand()%5;srand(rand());
	if(chance <=3){
		// 80% chance keep 4 sided square
		chance = rand()%5;srand(rand());
		if(chance <=3){
			//80% to stay square
			chance = rand()%5;srand(rand());
			if(chance <=2){
				//60% chance to be normal square
				chance = rand() % 5; srand(rand());
				if (chance <= 2) {
					//60% chance to stay same
					//floor plan unchanged
					glNewList(result->low, GL_COMPILE);
					generateModernBuilding(parameters->boundingArea, center, minDist);
					glEndList();
					return;
				}else {
					//40% chance for residential building
					result->low = glGenLists(1);
					glNewList(result->low, GL_COMPILE);
					generateResdientialBuilding(floorPlan);
					glEndList();
					return;
				}


			}else{
				//40% chance to shrink
				floorPlan = Generator::shrinkPoints(floorPlan);
			}

		}else{
			//20% chance to be a cut square
			floorPlan = Generator::cutEdges(floorPlan);
		}
	}else{
		//40% chance to change to more exciting shape
		chance = rand() % 5; srand(rand());
		if (chance <= 2) {
			//60% chance for single shape of >=4 sides
			chance = rand() % 5; srand(rand());
			if (chance <= 1) {
				//40% chance of rotated square
				floorPlan = Generator::generateFloorPlan(center, minDist*1.5f, 4);
			}else if(chance<=4){
				//40% chance of new shape
				floorPlan = Generator::generateFloorPlan(center, minDist, (rand()%4)+4);
			}else {
				//20% chance of really odd building
				vector<vec2> shapeA = Generator::generateFloorPlan(center+vec2(minDist/2,minDist/2), minDist*2.0f, (rand() % 4) + 4);
				floorPlan = Generator::combinePlans(shapeA, Generator::generateFloorPlan(center, minDist*2.0f, (rand() % 4) + 4));
			}

		}else {
			//40% chance of modern building
			result->low = glGenLists(1);
			glNewList(result->low, GL_COMPILE);
			generateModernBuilding(parameters->boundingArea, center, minDist);
			glEndList();
			return;
		}
	}

	cur_tex_wall = parameters->b_type;
	cur_tex_wall_num = rand()%2;

	result->low = glGenLists(1);
	glNewList(result->low, GL_COMPILE);
	generateFromString(floorPlan, Generator::generateRandomBuildingString(rand() % 4 + 3));//TODO fix this so the iterations are a function of height or other parameter
	glEndList();

}

void Building::generateResdientialBuilding(vector<vec2> points) {
	//set first two points to subdivion of original floor
	vector<vector<vec2>> tiers = subdivide(points);
	vector<vec2> temp = tiers[0];
	tiers = subdivide(tiers[1]);
	tiers.push_back(temp);
	//tiers.push_back(subdivide(tiers[1])[1]);
	//tiers[1] = subdivide(tiers[1])[0];

	srand(rand());
	int n = rand() % 10 + 5;		//between 5 and 15
	float cur_elev = 0.0f;
	//extend main tier random amount of times
	for(int i = 0; i < n ;i++){
		renderWindows(tiers[2],cur_elev);
		cur_elev = extendBuilding(tiers[2], cur_elev);
	}
	n = rand() % (n-((int)(n *0.75) -1)) + (int)(n *0.75);//between 3/4 and n (inclusive)
	cur_elev = 0.0f;
	for (int i = 0; i < n; i++) {
		renderWindows(tiers[0], cur_elev);
		cur_elev = extendBuilding(tiers[0], cur_elev);
	}
	n = rand() % (n - ((int)(n *0.75) - 1)) + (int)(n *0.75);//between 3n/4 and n (inclusive)
	cur_elev = 0.0f;
	for (int i = 0; i < n; i++) {
		renderWindows(tiers[1], cur_elev);
		cur_elev = extendBuilding(tiers[1], cur_elev);
	}


}

void Building::generateModernBuilding(vector<vec2> points,vec2 mid, float minDist) {

	vector<vector<vec2>> levels = vector<vector<vec2>>();
	levels.push_back(Generator::generateFloorPlan(mid, minDist, rand() % 4 + 4));//add base
	srand(rand());
	//generate 1 to 4 plan changes
	for (int i = 0; i < rand() % 3 + 2; i++) {
		srand(rand());
		float xmul = rand() % 2 - 1;//-1 to 1;
		srand(rand());
		float ymul = rand() % 2 - 1;//-1 to 1;
		srand(rand());
		vec2 offset = vec2(xmul*(minDist/2.0f),(minDist/2.0f)*ymul);
		//in this case, i is the most recently added level (i.e. the largest floorplan)
		levels.push_back(Generator::combinePlans(levels[i], Generator::generateFloorPlan(mid+offset, minDist, rand() % 4 + 4)));
	}
	//the end of levels now contains the ground floor

	float elevation = 0.0f;
	for (int i = levels.size() - 1; i >= 0;) {
		srand(rand());
		renderWindows(levels[i], elevation);
		elevation = extendBuilding(levels[i], elevation);
		if (rand() % 3 <= 1) {//66% chance to change floor plan
			i--;
		}
	}


}

/*Subdivides the input 4 point floor plan into 2 sets of floor plans
 * Returns a vector of floor plans where index 0 is the larger half
 * and index 1 is the smaller
 */
