#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "comp308.hpp"
#include "generator.h"

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
		case 6:

			return "E";
		case 8:
		case 3:
		case 4:
		case 9://40% chance of increasing in height
			return "EE";
		case 5:
		case 7://10% chance of shrinking
			return "ES";
		}
	}else if (c == 'S') {
		switch ((int)(rand() % 10)) {
		case 0://40% chance to stay same
		case 1:
		case 6:
		case 7:
			return "S";
		case 2://40% chance to increase in height
		case 3:
		case 4:

			return "SE";
		case 8://20% chance to subDivide
		case 5:
			return "DE";
		case 9:
			return "SE";
		}
	}else if (c =='*'){
		switch ((int)(rand() % 10)) {
		case 0:
		case 1:
		case 2:
		case 3:
			return "S*";
		case 4:
			return "ED*";
		case 5:
		case 6:
			return "E*";
		case 7:
		case 8:
		case 9:
			return "R";
		}
	}
	return "" + c;
}


/*Creates a random string with *itrs amount of iterations */
string Generator::generateRandomBuildingString(int itrs) {
	itrs = max(min(itrs, 6),2);
	string result = "*";
	for (int i = 0; i < itrs; i++) {
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
	itrs = max(min(itrs, 6), 2);
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
vector<vec2> Generator::generateFloorPlan(vec2 center, float radius, int n){
	if( n <=3)n=4;
	vector<vec2> points;
	//set the first point to be randomized as well
	float dr = 180-((n-2)*180)/n;
	double randx = rand() % 20+1;
	double randy = rand() % 20 + 1;
	vec2 dir = normalize(vec2(randx, randy));//direction for next point from center
	dir *= radius;
	float theta = radians(dr);
	float cs = cos(theta);
	float sn = sin(theta);
	for(int i = 0; i < n;i++){
		points.push_back(center+dir);
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
