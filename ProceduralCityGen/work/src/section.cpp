#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "section.hpp"
#include "comp308.hpp"

using namespace std;
using namespace comp308;

SectionDivider::SectionDivider() {};

/**
* Finds the longest edge in a section and returns it
*/
line SectionDivider::findLongestEdge(section *s) {
	float maxLength = 0.0f;
	line longLine;

	for (int i = 0; i < (int)s->lines.size(); i++) {
		if (s->lines[i].length > maxLength) {
			maxLength = s->lines[i].length;
			longLine = s->lines[i];
		}
	}
	return longLine;
}

/**
* Divides the sections
*/

void SectionDivider::divideSection(section s) {
	recDivideSection(&s);
}

/**
* Recursively divides sections by extending a perpendicular bisector
* of the longest edge slightly off centre. Continuely recurses until
* it is smaller than the largest acceptable size for a section.
*/

void SectionDivider::recDivideSection(section *s) {
	vector<section> lots = splitSection(s);

	for (section l : lots) {
		if (l.area <= goalArea) {
			sections.push_back(l);
		}
		else {
			recDivideSection(&l);
		}
	}
}

vector<section> SectionDivider::splitSection(section *s) {
	line l = findLongestEdge(s);
	vec2 lineVec = vec2(l.x2-l.x1, l.y2-l.y1);
	vec2 perpBi = vec2(-lineVec.y, lineVec.x); // gets perpendicular bisector to longest edge

	// Randomly decides a position around the centre of the longest edge
	// to extend the bisector from. This will be within the middle third of the section

	float random = ((float)rand() / (RAND_MAX)) + 1;
	float centre = l.length*(1.0f / 3.0f) + random * (l.length*(1.0f / 3.0f));

	float m_l = (l.y2 - l.y1) / (l.x2 - l.x1);
	float c_l = l.y2 - m_l*l.x2;
	// Now finds the first intersection point with another line within the section
	for (int i = 0; i < (int)s->lines.size(); i++) {
		// need a check here to see if they are the same
		line other = s->lines[i];
		float m = (other.y2 - other.y1) / (other.x2 - other.x1);
		float c = other.y2 - m_l*other.x2;

		if (m != m_l && c != c_l) {
			float x = (c - c_l) / (m_l - m);
			float y = m_l * x + c_l;
			// Now uses the intersection point of these two lines to determine if this is
			//the line it should split

			if (x >= max(other.x1, other.x2) && x <= min(other.x1, other.x2)
				&& y >= max(other.y1, other.y2) && y <= min(other.y1, other.y2)) {

			}

		}
	}
	vector<section> x;
	return x;

}



