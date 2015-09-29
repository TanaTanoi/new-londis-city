#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include "comp308.hpp"
#include "generator.h"
#include "utility.hpp"

using namespace std;
using namespace comp308;



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
		case 9://30% chance of increasing in height
			return "EE";
		case 3:
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
		return "S*";
	case 4:
	case 3:
		return "ED*";
	case 5:
	case 6:
		return "ES*";

	case 8:
	case 9:
		return "R";
	}
}
return "" + c;
}


/*Creates a random string with *itrs amount of iterations */
string Generator::generateRandomBuildingString(int itrs) {
	itrs = max(min(itrs, 6), 3);
	string result = "*";			//axoim
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


/*Combines two plans together in a logical OR fashion.*/
vector<vec2> Generator::combinePlans(vector<vec2> shapeA, vector<vec2> shapeB) {
	//get intersection points, if none, return
	vector<vec2> newPlan = vector<vec2>();
	int n1 = shapeA.size();
	int n2 = shapeB.size();
	int n[] = {n1,n2};
	//current shape we are tracing
	int curShape = 0;
	cout<<"Lines :";
	int index = 0;//TODO find better starting index, (i.e. out of the other shape)
	//find point furthest away from the center of B
	vec2 midB = centerPoint(shapeB);
	for(int i =0; i < n1;i++){
		if(abs(length(shapeA[index]-midB)) < abs(length(shapeA[i]-midB))){
			index = i;
		}
	}

	vec2 currentPoint = shapeA[index];
	//while we don't contain the next point in the trace (currentPoint)
	while (!containsVec(newPlan,currentPoint)) {
//		cout<<"In loop. Index: "<<index<< " On shape " << (curShape+1)<<endl;
		//add the point
		newPlan.push_back(currentPoint);
		cout<< " " << index<<":"<<curShape<<endl;
		//check this line against the other shape for intersections
		bool hasIntersection = false;
		for (int j = 0; j < n[!curShape]; j++) {//cycle through other shape
			//if we are currently on the first shape
			if (curShape == 0) {
				//cout<<"On first shape, testing intersecting on other point "<< j << endl;
				if (util::intersects(currentPoint, shapeA[(index + 1) % n1], shapeB[j], shapeB[(j + 1) % n2])) {
//					cout<<"found intersection at " << j << endl;
					//if we find an intersection, add it
					newPlan.push_back(util::getIntersection(currentPoint, shapeA[(index + 1) % n1], shapeB[j], shapeB[(j + 1) % n2]));
					cout << " I1" << endl;
					//and then add point on second shape
					index = (j + 1) % n2;
					hasIntersection = true;
					break;
				}
			}else {//if we are on the second shape
				if (util::intersects(currentPoint, shapeB[(index + 1) % n2], shapeA[j], shapeA[(j + 1) % n1])) {
					//if we find an intersection, add it
					newPlan.push_back(util::getIntersection(currentPoint, shapeB[(index + 1) % n2], shapeA[j], shapeA[(j + 1) % n1]));
					cout<< " I2"<<endl;
					//and then add point on second shape
					index = (j + 1) % n1;
					hasIntersection = true;
					break;
				}
			}
		}
		if (hasIntersection) {
//			cout<<"WE had an intersection"<<endl;
			//swap shape we are currently on
			curShape = !curShape;
			if(curShape ==0){
				currentPoint = shapeA[index];
			}else{
				currentPoint = shapeB[index];
			}

		}else {
			if (curShape == 0) {
				index = (index + 1) % n1;
				currentPoint = shapeA[index];
			}else {
				index = (index + 1) % n2;
				currentPoint = shapeB[index];
			}
		}

	}
	return newPlan;

}
