#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include "comp308.hpp"
#include "generator.h"
#include "utility.hpp"
#include "section.hpp"
#include "building.hpp"
#include "spline.hpp"
using namespace std;
using namespace comp308;
using namespace util;

static const float SECTION_TO_POINTS_SCALE = 0.04f;//1/25 default 0.04
float Generator::SECTION_TO_POINT_SCALE(){return SECTION_TO_POINTS_SCALE;}
//float Generator::NETWORK_TO_SECTION_SCALE(){return NETWORK_TO_SECTION_SCALE;}
/*Method that looks up the potential replacements/extensions for a given char*/
string LSystemLookup(char c) {
	srand(rand());
	if (c == 'E') {
		switch ((int)(rand() % 10)) {
		case 0://40% chance of staying same
		case 1:
		case 2:
		case 4:
			return "E";
		case 6:
		case 8:
		case 3:
		case 9://30% chance of increasing in height
			return "EE";

		case 5:
		case 7://30% chance of shrinking
			return "ES";
		}
	}else if (c == 'S') {
		switch ((int)(rand() % 10)) {
		case 0://30% chance to stay same
		case 1:
		case 6:
			return "S";
		case 2://40% chance to increase in height
		case 3:
		case 4:
		case 7:
			return "SE";
		case 8://20% chance to subDivide
		case 5:
			return "DE";
		case 9:
			return "SE";
		}
	}
else if (c == '*') {
	switch ((int)(rand() % 10)) {
	case 0:
	case 1:
	case 2:
	case 7:
		return "ES*";
	case 4:
	case 3:
		return "ED*";
	case 5:
	case 6:
		return "E*";

	case 8:
	case 9:
		return "R";
	}
}
else if (c == '$') {
	return "$";
}
return "" + c;
}

/*Creates a random string with *itrs amount of iterations */
string Generator::generateRandomBuildingString(int itrs) {
	itrs = max(min(itrs, 6), 3);
	string result = "$*";			//axoim
	for (int i = 0; i < itrs; i++) {
		//for each string
		string next = "";
		for (int c = 0; c < result.length(); c++) {
			string addition = LSystemLookup(result[c]);
			next = next + addition;
		}
		result = next;
	}
	return result;
}

string Generator::generateResdidentialBuildingString(int itrs) {
	itrs = max(min(itrs, 6), 3);
	string result = "";
	for (int i = 0; i < rand() % 12; i++) {
		result += "E";
	}
	result += "D";
	return result;
}


/*Returns a floor plan based on *n the number of edges it should have
 * and a *radius.
 * If n <= 3, it will default to 4
 */
vector<vec2> Generator::generateFloorPlan(vec2 center, float radius, int n) {
	if (n <= 3)n = 4;
	vector<vec2> points;
	//set the first point to be randomized as well
	float dr = 180 - ((n - 2) * 180) / n;
	double randx = rand() % 20 + 1;
	double randy = rand() % 20 + 1;
	vec2 dir = normalize(vec2(randx, randy));//direction for next point from center
	dir *= radius;
	float theta = radians(dr);
	float cs = cos(theta);
	float sn = sin(theta);
	for (int i = 0; i < n; i++) {
		points.push_back(center + dir);
		//rotate dir by function of n
		vec2 newDir = vec2((dir.x * cs) - (dir.y * sn), (dir.x * sn) + (dir.y * cs));
		dir = newDir;
	}

	return points;
}

/*Turns a 4 point floor plan into an 8 point floor plan by cutting off the edges*/
vector<vec2> Generator::cutEdges(vector<vec2> points) {
	int n = points.size();
	if (n != 4)return points;
	vector<vec2> result;
	for (int i = 0; i < n; i++) {
		vec2 diff = points[(i + 1) % n] - points[i];
		diff *= 0.2;
		result.push_back(points[i] + diff);
		result.push_back(points[(i + 1) % n] - diff);
	}
	return result;
}

/*Generates a modern floorPlan for testing combinePlans*/
vector<vec2> Generator::generateModernFloorPlan(vec2 center, float radius) {
	cout<<"Generating modern floors"<<endl;
	radius = max(0.3f,radius);
	vector<vec2> shapeA = generateFloorPlan(center+vec2(radius/2,radius/2), radius*2, rand()%4+3);
	srand(rand());
	vector<vec2> shapeB = generateFloorPlan(center, radius*2, rand()%4+3);
	srand(rand());
	shapeA = combinePlans(shapeA, shapeB);
	shapeB = generateFloorPlan(center-vec2(radius,radius), radius*2, rand()%4+3);
	srand(rand());
	shapeA = combinePlans(shapeA, shapeB);
	shapeB = generateFloorPlan(center+vec2(radius,-radius/2), radius*2, rand()%4+3);
	srand(rand());
	return combinePlans(shapeA, shapeB);
}

/*Checks if a vector is contained within the array*/
bool containsVec(vector<vec2> array, vec2  toFind){
	if(array.empty()){return false;}
	for(vec2 v:array){
		if(v.x == toFind.x &&v.y == toFind.y){
			return true;
		}
	}
	return false;

}

/*Returns a vec2 that represents a point in the center of the
 * shape created by *points.
 */
vec2 Generator::centerPoint(vector<vec2> points){
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

/*Shrinks the input points by a factor of 0.15 and returns the result*/
vector<vec2> Generator::shrinkPoints(std::vector<vec2> points) {
	vec2 mid = Generator::centerPoint(points);
	vector<vec2> smallPoints;
	float dist = 0.15f;// static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.2f)+0.1f;
	for (vec2 v2 : points) {
		vec2 diff = (mid - v2)*dist;
		smallPoints.push_back(v2 + diff);
	}
	return smallPoints;
}

/*From the given floor plan, extrude to create a block of height 0.5f
*
* *floor must be a list of points where a wall is connected between
* point i and point i +1 (where last and first point are connected)
*/
vector<vector<vec2>> Generator::subdivide(vector<vec2> points) {
	vector<vector<vec2>> result;
	if (points.size() != 4) { result.push_back(points); result.push_back(points); return result; }
	float cutDist = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.5f) + 1.5f;
	vec2 cutP1 = ((points[1] - points[0]) / cutDist) + points[0];
	vec2 cutP2 = ((points[2] - points[3]) / cutDist) + points[3];
	result.push_back(vector<vec2>());
	result.push_back(vector<vec2>());

	int i = cutDist >= 2.0f;
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


/*Combines two plans together in a logical OR fashion.*/
vector<vec2> Generator::combinePlans(vector<vec2> shapeA, vector<vec2> shapeB) {
	//get intersection points, if none, return
	cout<<"START COMBINE"<<endl;
	vector<vec2> newPlan = vector<vec2>();
	int n[] = { shapeA.size(),shapeB.size() };
	vector<vec2> shapes[] = {shapeA,shapeB};
	//current shape we are tracing
	int curShape = 0;
	int index = 0;
	//find point furthest away from the center of B
	vec2 midB = centerPoint(shapes[1]);
	for(int i =0; i < n[0];i++){
		if(abs(length(shapes[0][index]-midB)) < abs(length(shapes[0][i]-midB))){
			index = i;
		}
	}
	vec2 currentPoint = shapes[0][index];
	//TODO current problem: Doesn't add the zero point of the second shape or sometimes first shape
	while (!containsVec(newPlan,currentPoint)) {
		//while we don't contain the next point in the trace (currentPoint)
		//add the point
		newPlan.push_back(currentPoint);
		cout<< " " << index<<":"<<curShape<<endl;
		//check this line against the other shape for intersections
		bool hasIntersection = false;
		for (int j = 0; j < n[!curShape]; j++) {//for each line in the other shape
			int o_index = (index+j-1)%n[!curShape];//this makes it find the first following intersection (hacky)
			if(o_index<0)o_index =n[!curShape]-1;
//			int o_index = index+j;
			if (util::intersects(currentPoint, shapes[curShape][(index + 1) % n[0]], shapes[!curShape][o_index],
					shapes[!curShape][(o_index + 1) % n[1]])) {
									cout<<"found intersection at " << o_index << endl;
				//if we find an intersection, add it
				newPlan.push_back(util::getIntersection(currentPoint, shapes[curShape][(index + 1) % n[0]],
						shapes[!curShape][o_index],
						shapes[!curShape][(o_index + 1) % n[1]]));
				//and then add point on second shape
				index = (o_index+1);
				if(index == n[!curShape])index =0;
				hasIntersection = true;
				break;
			}
		}
		if (hasIntersection) {
			//swap shape we are currently on and get next point in the series
			curShape = !curShape;
			currentPoint = shapes[curShape][index];

		}else {
			index = (index + 1) % n[curShape];
			currentPoint = shapes[curShape][index];
		}

	}
	return newPlan;


}

/*Generates a section based on a randomly created floor plan of points*/
section Generator::createRandomSection(){
	section s;
	vector<vec2> points = generateFloorPlan(vec2(0,0),200,rand()%6+4);

	vector<util::line> lines = vector<util::line>();
	int n = points.size();
	for(int i =0;i<n;i++){
		points[i] = points[i] + vec2(rand()%200-100,rand()%200-100);

	}
	for(int i =0; i < n;i++){

		lines.push_back({points[i],points[(i+1)%n],i});
	}
	return {lines,0,0};
}

/*Converts a section and its lines into a vector of points*/
vector<vec2> Generator::sectionToPoints(util::section sec){
	vector<vec2> toReturn = vector<vec2>();
	int n = sec.lines.size();
	for (int i = 0; i < n;i++) {
		//The mod 2 is to offset the front door by a certain amount.
		toReturn.push_back(sec.lines[(i+2)%n].start*SECTION_TO_POINTS_SCALE);
	}
	return toReturn;
}

util::section Generator::shrinkSection(util::section sec){
	vector<vec2> points = vector<vec2>();
	int n = sec.lines.size();
	for (int i = 0; i < n;i++) {
		points.push_back(sec.lines[i].start);
	}
	vector<line> lines = vector<line>();
	points = shrinkPoints(points);
	n = points.size();
	for(int i =0; i <n;i++){
		line l = {points[i],points[(i+1)%n],i};
		lines.push_back(l);
	}

	return {lines,0,0};
}

/*Converts a vector of sections into a vector of building parameters*/
vector<buildingParams> Generator::sectionsToParams(vector<section> sections,vector<vec2> heightmap) {
	//TODO convert a lot into a list of building parameters I can send to the main then building class
	vector<buildingParams> toReturn;
	float maxDist = 0.0f;
	for(int i=0;i<sections.size();i++){
		float compare_x = sectionToPoints(sections[i])[0].x;
		if(maxDist<compare_x){
			maxDist = compare_x;
		}
	}
	for (int i = 0; i < sections.size(); i++) {
		buildingParams p;
		p.boundingArea = sectionToPoints(sections[i]);
		p.seed = rand();
		p.b_type = (building_type)(p.seed % 2);//TODO make this have an effect, possibly
		float ymap = Spline::calculateYValue(heightmap,abs(( p.boundingArea[0].x/maxDist)*640));
		p.height = (int)(((480-ymap)/480)*5+2);
		srand(p.seed);
		toReturn.push_back(p);
	}
	return toReturn;
}
/*Returns a bounding box for a given shape where r[0] == min and r[1] == max*/
vector<vec2> Generator::getBoundingBox(vector<vec2> floor) {
	vec2 min = vec2(1000000, 1000000);
	vec2 max = vec2(-1000000, -100000);
	for (vec2 v : floor) {
		if (v.x < min.x)min.x = v.x;
		if (v.y < min.y)min.y = v.y;
		if (v.x > max.x)max.x = v.x;
		if (v.y > max.y)max.y = v.y;
	}
	vector<vec2> toReturn = vector<vec2>();
	toReturn.push_back(min);
	toReturn.push_back(max);
	return toReturn;

}
float NETWORK_TO_SECTION_SCALE = 2.0f;
section Generator::pointsToSections(vector<vec2> points){
	vector<line> lines = vector<line>();
	int n = points.size();


//	vec2 mid = Generator::centerPoint(points);
//	vector<vec2> smallPoints;
//	float dist = 0.01f;// static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.2f)+0.1f;
//	for (vec2 v2 : points) {
//		vec2 diff = (mid - v2)*dist;
//		smallPoints.push_back(v2 + diff);
//	}
//	points = smallPoints;
	points = shrinkPoints(points);
	for(int i =0; i <n;i++){
		line l = {points[i]*NETWORK_TO_SECTION_SCALE,points[(i+1)%n]*NETWORK_TO_SECTION_SCALE,i};
		lines.push_back(l);
	}
	return {lines,0,0};
}
