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
			return "E";
		case 3:
		case 4://30% chance of increasing in height
		case 5:
			return "EE";
		case 6:
		case 7://30% chance of shrinking
		case 8:
			return "ES";
		case 9://10% chance for roof
			return "ER";
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
			return "R";
		case 7://20% chance for double shrink
		case 8:
			return "SS";
		case 9:
			return "SES";
		}
	}else {
		return "" + c;
	}
	return "" + c;
}


/*Creates a random string with *itrs amount of iterations */
string Generator::generateRandomString(int itrs) {
	string result = "E";
	for (int i = 0; i < itrs; i++) {
		string next = "";
		for (int c = 0; c < result.length(); c++) {
			string addition = LSystemLookup(result[c]);
			next = next + addition;
			if (addition[addition.size() - 1] == 'R') {
				return next;
			}
		}
		result = next;
		cout << "Result is " << result << " after " << i << " iterations" << endl;
	}
	cout << "Returning " << result;
	return result;
}
/*Returns a floor plan based on *n the number of edges it should have
 * and a *radius.
 * REQUIRES n > 2
 */
vector<vec2> Generator::generateFloorPlan(vec2 center, float radius, int n){
	if( n <=2)n=3;
	vector<vec2> points;
	float dr = 180-((n-2)*180)/n;
	vec2 dir = vec2(0,radius);//direction for next point from center
	for(int i = 0; i < n;i++){

		points.push_back(center+dir);
		//rotate dir by function of n
		float theta = radians(dr);
		float cs = cos(theta);
		float sn = sin(theta);
		vec2 newDir = vec2((dir.x * cs) - (dir.y * sn), (dir.x * sn) + (dir.y * cs));
		cout<<"Direction "<< dir.x << " " << dir.y<<endl;
		dir = newDir;
	}
//	points.push_back(center);


	return points;
}
