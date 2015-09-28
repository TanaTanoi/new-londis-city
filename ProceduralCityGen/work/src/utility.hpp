/*
 * utility.hpp
 *
 *  Created on: 26/09/2015
 *      Author: craighhann
 */
#pragma once

#include "comp308.hpp"
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>
using namespace comp308;
using namespace std;

namespace util{

/**
		A line represents a single line in a section outline
*/
struct line {
	comp308::vec2 start;
	comp308::vec2 end;
	int ID;
	float length;
};


/*Returns a vec2 where r.x = gradient and r.y = y-offset*/
inline vec2 getEquation(vec2 a, vec2 b) {
	vec2 equation;
	equation.x = (b.y - a.y) / (b.x - a.x);
	equation.y = (a.y - (equation.x*a.x));
	return equation;
}

inline vec2 getIntersection(line l, vec2 cut, vec2 cutPoint){
	cout << "Line ID " << l.ID;
	float m_l = (l.end.y - l.start.y) / (l.end.x - l.start.x);
	float c_l = l.end.y - m_l*l.end.x;
	cout << "Original line "<< m_l << "  " << c_l << endl;
	cout << "Cut x " << cut.x <<  "Cut y " << cut.y << endl;

	float m = cut.y/cut.x;
	if(cut.x == 0){
		return vec2(cutPoint.y,cut.y);
	}
	float c = cutPoint.y - m*cutPoint.x; // y = mx + c so c =  y - mx


	cout << "Bisector line "<< m << "  " << c << endl;

	// Calculates the intersection point
	float x = (c - c_l) / (m_l - m);
	float y = m_l * x + c_l;

	return vec2(x,y);
}

/*REQUIRES: The lines to actually intersect
Intersection method given two lines, represented by 2 start and end vectors*/
inline vec2 getIntersection(vec2 a1, vec2 a2, vec2 b1, vec2 b2) {
	vec2 e1 = getEquation(a1, a2);
	vec2 e2 = getEquation(b1, b2);
	//If paralell lines
	//get x value in which they meet
	float x = (e2.y - e1.y) / (e1.x - e2.x);
	//then also find the y value
	float y = e1.x * x + e1.y;

	return vec2(x, y);
}

/*Tests if the two lines, represented as two vec2s each, intersect within the bounds of the points*/
inline bool intersects(vec2 a1, vec2 a2, vec2 b1, vec2 b2) {
	vec2 e1 = getEquation(a1, a2);
	vec2 e2 = getEquation(b1, b2);
	//If paralell lines
	if (e1.x == e2.x) {
		return false;
	}
	float x = (e2.y - e1.y) / (e1.x - e2.x);
	//if x is out of bounds
	if (x < a1.x || x > a2.x) {
		return false;
	}
	//Return true if not parallel and if the point is within the bounds.
	return true;
}



inline bool intersects(line lon, vec2 cut, vec2 cutPoint) {

	vec2 intersection = getIntersection(lon,cut,cutPoint);
	cout << "Intersection Point " << intersection.x << " " << intersection.y << endl;

	// Now uses the intersection point of these two lines to determine if this is
	//the line it should split

	if (intersection.x <= max(lon.start.x, lon.end.x) && intersection.x >= min(lon.start.x, lon.end.x)
			&& intersection.y <= max(lon.start.y, lon.start.y) && intersection.y >= min(lon.start.y, lon.start.y)) {
		return true;
	}

	return false;
}

}
