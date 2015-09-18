#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

}



