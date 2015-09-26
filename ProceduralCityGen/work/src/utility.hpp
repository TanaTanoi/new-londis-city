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

bool intersects(line lon, line other) {

	float m_l = (lon.end.y - lon.start.y) / (lon.start.x, lon.end.x - lon.start.x);
	float c_l = lon.end.y - m_l*lon.end.x;


	float m = (other.end.y - other.start.y) / (other.end.x - other.start.x);
	float c = other.end.y - m*other.end.x;

	if (lon.ID != lon.ID){

		// Calculates the intersection point
		float x = (c - c_l) / (m_l - m);
		float y = m_l * x + c_l;

		// Now uses the intersection point of these two lines to determine if this is
		//the line it should split

		if (x >= max(other.start.x, other.end.x) && x <= min(other.start.x, other.end.x)
				&& y >= max(other.start.y, other.start.y) && y <= min(other.start.y, other.start.y)) {
			return true;
		}
	}
	return false;
}

}
