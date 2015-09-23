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
line* SectionDivider::findLongestEdge(section *s) {
	float maxLength = 0.0f;
	line* longLine;

	for (int i = 0; i < (int)s->lines.size(); i++) {
		if (s->lines[i].length > maxLength) {
			maxLength = s->lines[i].length;
			longLine = &(s->lines[i]);
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
		//if (l.area <= goalArea) {
	//		sections.push_back(l);
		//}
	//	else {
	//		recDivideSection(&l);
	//	}
	}
}

/**
 Finds whether two lines intersect
*/

bool SectionDivider::intersects(line* lon,float m_l, float c_l, line* other) {
	float m = (other->end.y - other->start.y) / (other->end.x - other->start.x);
	float c = other->end.y - m*other->end.x;

	if (lon != other){
		// Calculates the intersection point
		float x = (c - c_l) / (m_l - m);
		float y = m_l * x + c_l;

		// Now uses the intersection point of these two lines to determine if this is
		//the line it should split

		if (x >= max(other->start.x, other->end.x) && x <= min(other->start.x, other->end.x)
			&& y >= max(other->start.y, other->start.y) && y <= min(other->start.y, other->start.y)) {
			return true;
		}
	}
	return false;
}

vector<section> SectionDivider::splitSection(section *s) {
	line* l = findLongestEdge(s);
	vec2 lineVec =  l->end - l->start;
	vec2 perpBi = vec2(-lineVec.y, lineVec.x); // gets perpendicular bisector to longest edge

	// Randomly decides a position around the centre of the longest edge
	// to extend the bisector from. This will be within the middle third of the section

	float random = ((float)rand() / (RAND_MAX)) + 1;
	float centre = l->length*(1.0f / 3.0f) + random * (l->length*(1.0f / 3.0f));

	float m_l = (l->end.y - l->start.y) / (l->end.x - l->start.x);
	float c_l = l->end.y - m_l*l->end.x;

	// Now finds the first intersection point with another line within the section
	vector<line *> intersectors;
	for (int i = 0; i < (int)s->lines.size(); i++) {
		if (intersects(l, m_l, c_l, &(s->lines[i]))) {
			intersectors.push_back(&(s->lines[i]));
		}
	}
	free(l); // frees pointer

	// Now cretes two new sections from the split

	
	vector<section> x;
	return x;

}

void SectionDivider::testSection() {
	line a = {vec2(100,100),vec2(400,100)};
	line b = { vec2(100,100), vec2(150,300) };
	line c = {vec2(150,300), vec2(350,300)};
	line d = { vec2(350,300), vec2(400,100) };

	vector<line> lines = vector<line>();
	lines.push_back(a);
	lines.push_back(b);
	lines.push_back(c);
	lines.push_back(b);

	section s = { lines };
	sections.push_back(s);
}

void SectionDivider::renderTest() {
	section s = sections.back();
	glBegin(GL_LINES);
	for (line l : s.lines) {
		glVertex2f(l.start.x, l.start.y);
		glVertex2f(l.end.x, l.end.y);
	}
	glEnd();
}



