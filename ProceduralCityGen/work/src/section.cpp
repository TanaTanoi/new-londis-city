#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "section.hpp"
#include "comp308.hpp"
#include "utility.hpp"

using namespace std;
using namespace comp308;
using namespace util;

SectionDivider::SectionDivider() {};

/**
 * Finds the longest edge in a section and returns it
 */
line SectionDivider::findLongestEdge(section s) {
	float maxLength = 0.0f;
	line longLine;

	for (int i = 0; i < (int)s.lines.size(); i++) {
		int newLength = abs(length(s.lines[i].start-s.lines[i].end));
		if (newLength > maxLength) {
			maxLength = newLength;
			longLine = s.lines[i];
		}
	}
	return longLine;
}

///**
//* Divides the sections
//*/
//
//void SectionDivider::divideLot(lot l) {
//	recDivideSection(&l.boundingBox);
//}
//

/**
 * Recursively divides sections by extending a perpendicular bisector
 * of the longest edge slightly off centre. Continuely recurses until
 * it is smaller than the largest acceptable size for a section.
 */

void SectionDivider::recDivideSection(section s) {
	vector<section> lots = splitSection(s);
	for (section l : lots) {
		splitSection(s);
	}

	//		if (l.area <= goalArea) {
	//			sections.push_back(l);
	//		}
	//		else {
	//			recDivideSection(&l);
	//		}
}

//
vector<section> SectionDivider::splitSection(section s) {
	line l = findLongestEdge(s);
	vec2 lineVec =  l.end - l.start;
	vec2 perpBi = vec2(-lineVec.y, lineVec.x); // gets perpendicular bisector to longest edge

	// Randomly decides a position around the centre of the longest edge
	// to extend the bisector from. This will be within the middle sixth of the section

	float random = ((float)rand() / (RAND_MAX)) + 1;
	float centreX = l.length*(5.0f / 12.0f) + random * (l.length*(1.0f / 6.0f));

	float m = (l.end.y - l.start.y) / (l.start.x, l.end.x - l.start.x);
	float c = l.end.y - m*l.end.x;

	float centreY = m*centreX + c;



	// Now finds the first intersection point with another line within the section
	vector<line> intersectors;
	for (int i = 0; i < (int)s.lines.size(); i++) {
		if(s.lines[0].ID != l.ID){
			if (intersects(s.lines[i],perpBi)) {
				intersectors.push_back(s.lines[i]);
			}
		}
	}

	cout<< "Intersecting Lines found" << intersectors.size() << endl;

	// Now creates two new sections from the split

	line toCut = intersects[0];

	section a, b; // the two new sections
	line bi = {getIntersection(toCut,perpBi), getIntersection(l,perpBi)};

	vector<section> x;
	return x;

}

void SectionDivider::testSection() {
	line a = {vec2(100,100),vec2(400,100),0};
	line b = { vec2(100,100), vec2(150,300),1 };
	line c = {vec2(150,300), vec2(350,300),2};
	line d = { vec2(350,300), vec2(400,100),3 };

	vector<line > lines = vector<line >();
	lines.push_back(a);
	lines.push_back(b);
	lines.push_back(c);
	lines.push_back(d);

	section s = { lines };
	sections.push_back(s);

	for (line l : sections[0].lines) {
		cout << l.ID << endl;
	}
}

void SectionDivider::renderTest() {
	//section s = sections.back();
	glBegin(GL_LINES);

	//	line longLine = findLongestEdge(sections[0]);
	int longID  = findLongestEdge(sections[0]).ID;
	//	cout << "LongLIne: " << longLine.ID << endl;
	for (line l : sections[0].lines) {
		//		cout <<"Checking ID "<< l.ID << endl;
		if(longID == l.ID){ // draw green if longest line
			glColor3f(0.0,1.0,0.0);
			//			cout <<"Drawing longest line: "<< l.ID << endl;
		}
		else{
			//			cout <<"Not     longest line: "<< l.ID << endl;
			glColor3f(1.0,0.0,0.0); // otherwise draw red
		}

		glVertex2f(l.start.x, l.start.y);
		glVertex2f(l.end.x, l.end.y);
	}
	glEnd();
}

//void SectionDivider::cleanUp(){
//	for(line * l : sections[0].lines){
//		free(l);
//	}
//}



