/*
 * utility.hpp
 *
 *  Created on: 26/09/2015
 *      Author: craighhann
 */

#include "comp308.hpp"
#include <cmath>
#include <algorithm>

using namespace comp308;
using namespace std;

namespace util{



vec2 getIntersection(line l, vec2 cut, vec2 cutPoint){
	float m_l = (l.end.y - l.start.y) / (l.start.x, l.end.x - l.start.x);
	float c_l = l.end.y - m_l*l.end.x;


	float m = cut.y/cut.x;
	float c = cutPoint.y - m*cutPoint.x; // y = mx + c so c =  y - mx

	// Calculates the intersection point
	float x = (c - c_l) / (m_l - m);
	float y = m_l * x + c_l;

	return vec2(x,y);
}

bool intersects(line lon, vec2 cut, vec2 cutPoint) {

	vec2 intersection = getIntersection(lon,cut,cutPoint);

	// Now uses the intersection point of these two lines to determine if this is
	//the line it should split

	if (intersection.x >= max(lon.start.x, lon.end.x) && intersection.x <= min(lon.start.x, lon.end.x)
			&& intersection.y >= max(lon.start.y, lon.start.y) && intersection.y <= min(lon.start.y, lon.start.y)) {
		return true;
	}

	return false;
}

}
