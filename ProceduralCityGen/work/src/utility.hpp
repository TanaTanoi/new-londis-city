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
	if (_isnan(equation.x)||!_finite(equation.x)){//if x is nan, then it is paralell to the y-axis, and thus can't be represented properly
		//instead we leave x as nan and change y to the x point along the axis where the line is
		//cout << "Equation from " << a.x << " " << a.y << " to " << b.x << " " << b.y << " is y-axis" << endl;
		equation.y = a.x;
	}
	return equation;
}

inline vec2 getIntersection(line l, vec2 cutDir, vec2 cutPoint){
	cout << "Line ID " << l.ID;
	float m_l = (l.end.y - l.start.y) / (l.end.x - l.start.x);
	float c_l = l.end.y - m_l*l.end.x;
	cout << "Original line "<< m_l << "  " << c_l << endl;
	cout << "Cut x " << cutDir.x <<  "Cut y " << cutDir.y << endl;

	float m = cutDir.y/cutDir.x;
	if(cutDir.x == 0){ // vertical line
		cout << "Cut point " << cutPoint.x << "  " << cutPoint.y << endl;
		return vec2(cutPoint.x,l.start.y);
	}
	float c = cutPoint.y - m*cutPoint.x; // y = mx + c so c =  y - mx


	cout << "Bisector line "<< m << "  " << c << endl;

	// Calculates the intersection point
	float x = (c - c_l) / (m_l - m);
	float y = m_l * x + c_l;

	return vec2(x,y);
}

//inline vec2 getIntersection(line l, line o){
//	float m_l = (l.end.y - l.start.y) / (l.end.x - l.start.x);
//	float c_l = l.end.y - m_l*l.end.x;
//
//	float m = (o.end.y - o.start.y) / (o.end.x - o.start.x);
//
//	float c = o.end.y - m*o.end.x;
//
//	// Calculates the intersection point
//	float x = (c - c_l) / (m_l - m);
//	float y = m_l * x + c_l;
//
//	return vec2(x,y);
//}

/*REQUIRES: The lines to actually intersect
Intersection method given two lines, represented by 2 start and end vectors*/
inline vec2 getIntersection(vec2 a1, vec2 a2, vec2 b1, vec2 b2) {
	vec2 e1 = getEquation(a1, a2);
	vec2 e2 = getEquation(b1, b2);
	//cout << "Equations " << e1.x << " " << e1.y << " to " << e2.x << " " << e2.y << "|" << endl;
	//If paralell lines
	//get x value in which they meet
	float x = (e2.y - e1.y) / (e1.x - e2.x);
	//then also find the y value
	float y = e1.x * x + e1.y;
	vec2 toReturn = vec2(x, y);
	if (!_finite(e1.x)||_isnan(e1.x)) {//if equation one is parallel to Y
		//Set the toReturn.x value to the value of e1.y (which is the x-intercept)
		toReturn.x = e1.y;
		toReturn.y = e2.x*toReturn.x + e2.y;
	}else if (!_finite(e2.x) || _isnan(e2.x)) {//if equation two is parallel to Y
							 //Set the toReturn.x value to the value of e2.y (which is the x-intercept)
		toReturn.x = e2.y;   //then set the y value to mx+c using the new x value
		toReturn.y = e1.x*toReturn.x + e1.y;
	}
		return toReturn;
}

/*Tests if the two lines, represented as two vec2s each, intersect within the bounds of the points*/
inline bool intersects(vec2 a1, vec2 a2, vec2 b1, vec2 b2) {
	vec2 e1 = getEquation(a1, a2);
	vec2 e2 = getEquation(b1, b2);
	//If paralell lines or if both are paralell to the y-axis
	if (e1.x == e2.x){
		cout << "gradients are equal" << e1.x <<  " " << e2.x <<endl;
		return false;
	}

	if ((!_finite(e1.x)) && (!_finite(e2.x))) {
		cout << "both are y axis" << endl;
		return false;
	}
	vec2 value = getIntersection(a1, a2, b1, b2);
	//float x = (e2.y - e1.y) / (e1.x - e2.x);
	float x = value.x;
	//if x is out of bounds X VALUE GOTTEN HERE IS WRONG , SHOULD BE 0.5
	if (x < min(a1.x,a2.x) || x > max(a2.x,a1.x) || x < min(b2.x,b1.x)||x > max(b1.x,b2.x)) {
		cout << "X is out of bounds "<<x << " " << a1.x  << " to " << a2.x<< endl;
		return false;
	}

	//if y is out of bounds
	if (value.y > max(a1.y,a2.y)||value.y < min(a2.y,a1.y)|| value.y > max(b1.y, b2.y) || value.y < min(b2.y, b1.y)) {
		cout << a1.y << " to " << a2.y << endl;
		cout << "Y value is out of bounds " << value.y <<  endl;
		return false;
	}

	//Return true if not parallel and if the point is within the bounds.
	return true;
}



inline bool intersects(line lon, vec2 cut, vec2 cutPoint) {
	cout << "cutPoint " <<cutPoint.x << "  " << cutPoint.y << endl;

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
