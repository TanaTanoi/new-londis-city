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
	vector<vec2> shapeA = generateFloorPlan(center+vec2(2.0f,2.0f), 2.0f, 5);
	vector<vec2> shapeB = generateFloorPlan(center, 2.0f, 4);
	//vector<vec2> shapeA = vector<vec2>();
	//
	//shapeA.push_back(vec2(1,0 ));
	//shapeA.push_back(vec2(1,1));
	//shapeA.push_back(vec2(0,1));
	//shapeA.push_back(vec2(0, 0));
	//vector<vec2> shapeB = vector<vec2>();
	//shapeB.push_back(vec2(-0.5f, -0.5f));
	//shapeB.push_back(vec2(0.5f,-0.5f));
	//shapeB.push_back(vec2(0.5f,0.5f));
	//shapeB.push_back(vec2(-0.5f,0.5f));
	//glBegin(GL_LINES);
	//for (int i = 0; i < 4; i++) {
	//	glVertex3f(shapeA[i].x, 1.0f, shapeA[i].y);
	//	glVertex3f(shapeA[(i + 1) % 4].x, 1.0f, shapeA[(i+1)%4].y);
	//	glVertex3f(shapeB[i].x, 1.0f, shapeB[i].y);
	//	glVertex3f(shapeB[(i + 1) % 4].x, 1.0f, shapeB[(i + 1) % 4].y);
	//}
	//glEnd();
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
	cout<<"Returning floor plan"<<endl;
	return newPlan;

}
