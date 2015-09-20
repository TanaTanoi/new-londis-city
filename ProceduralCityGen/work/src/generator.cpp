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
	if (c == 'E') {
		switch ((int)(rand() % 10)) {
		case 0://30% chance of staying same
		case 1:
		case 2:
		case 6:
		case 8:
			return "E";
		case 3:
		case 4://30% chance of increasing in height
		case 5:
			return "EE";
		
		case 7://30% chance of shrinking
		
			return "ES";
		case 9://10% chance for roof
		//	return "ER";
			return c + "";
		}
	}else if (c == 'S') {
		switch ((int)(rand() % 10)) {
		case 0://20% chance to stay same
		case 1:
			return "S";
		case 2://40% chance to increase in height
		case 3:
		case 4:
		case 5:
			return "SE";
		case 6://10% chance for roof
			//return "R";
		case 7://20% chance for double shrink
		case 8:
			return "S";
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
		case 5:
		case 6:
		case 7:
			return "E*";
		case 8:
		case 9:
			return "R";

		}
	}
	return "" + c;
}


/*Creates a random string with *itrs amount of iterations */
string Generator::generateRandomString(int itrs) {
	itrs = max(min(itrs, 6),2);
	string result = "*";
	for (int i = 0; i < itrs; i++) {
		string next = "";
		for (int c = 0; c < result.length(); c++) {
			string addition = LSystemLookup(result[c]);
			next = next + addition;
		}
		result = next;
		//cout << "Result is " << result << " after " << i << " iterations" << endl;
	}
	return result;
}

/*Returns a floor plan based on *n the number of edges it should have
 * and a *radius.
 * REQUIRES n > 2
 */
vector<vec2> Generator::generateFloorPlan(vec2 center, float radius, int n){
	if( n <=3)n=4;
	vector<vec2> points;
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
//	points.push_back(center);


	return points;
}
