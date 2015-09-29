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

/**
 * Divides the sections
 */

void SectionDivider::divideLot(lot l) {
	cout << "Number of sections " << l.sections.size() << endl;
	lots[l.ID] = recDivideSection(l,l.boundingBox);
}


/**
 * Recursively divides sections by extending a perpendicular bisector
 * of the longest edge slightly off centre. Continuely recurses until
 * it is smaller than the largest acceptable size for a section.
 */

lot SectionDivider::recDivideSection(lot lot, section s) {
	vector<section> secs = splitSection(s);
	cout << "section size" << secs.size() << endl;
	//	for (section l : secs) {
	//		if (l.area <= goalArea) {
	//			l.ID = sectionID++;
	//			lot.sections.push_back(l);
	//		}
	//		else {
	//			cout << "Recursively dividing" << endl;
	//			cout << "Size is " << l.area << endl;
	//
	//			lot = recDivideSection(lot, l);
	//		}
	//	}
	return lot;
}

//
vector<section> SectionDivider::splitSection(section s) {
	line l = findLongestEdge(s);
	//cout << "Longest edge ID " << l.ID << endl;
	vec2 lineVec =  l.end - l.start;
	vec2 perpBi = vec2(-lineVec.y, lineVec.x); // gets perpendicular bisector to longest edge

	// Randomly decides a position around the centre of the longest edge
	// to extend the bisector from. This will be within the middle sixth of the section

	float random = ((float)rand() / (RAND_MAX));

	float leng = abs(l.end.x - l.start.x);
	float centreX = 0.0f;
	float centreY = 0.0f;
	if(l.end.x == l.start.x){
		centreX = l.start.x;
		leng = abs(l.end.y - l.start.y);
		centreY = min(l.end.y, l.start.y) + leng*(5.0f / 12.0f) + random * (leng*(1.0f / 6.0f));
	}
	else{
		centreX = min(l.end.x, l.start.x) + leng*(5.0f / 12.0f) + random * (leng*(1.0f / 6.0f));
		float m = (l.end.y - l.start.y) / (l.end.x - l.start.x);
		float c = l.end.y - m*l.end.x;
		centreY = m*centreX + c;
	}

	//cout<<"center x " << centreX<<endl;

	//cout << m << endl;




	//cout<<"center y " << centreY<<endl;

	//cout << "Found all variables" << endl;
	// Now finds the first intersection point with another line within the section
	vector<line> intersectors = vector<line>();
	//cout << "Section size " <<s.lines.size() << endl;
	for (int i = 0; i < (int)s.lines.size(); i++) {
		cout << "Checking intersects line ID " << s.lines[i].ID << endl;
		if(s.lines[i].ID != l.ID){
			//cout << "Checking intersects" << endl;
			if (intersects(s.lines[i],perpBi,vec2(centreX,centreY))) { // FIXME : Issue is here, further subdivisions are not finding any intersections
				//cout << "Found intersector" << endl;
				intersectors.push_back(s.lines[i]);
			}
		}
	}

	cout<< "Intersecting Lines found" << intersectors.size() << endl;

	line toCut;
	float close = 0.0f;

	for (int i = 0; i < (int)intersectors.size(); i++) {
		float distance = abs((getIntersection(intersectors[i], perpBi, vec2(centreX, centreY)).y) - centreY);
		if (i == 0 || distance < close) {
			close = distance;
			toCut = intersectors[i];
		}
	}


	// Now creates two new sections from the split

	section a, b; // the two new sections
	a.lines = vector<line>(); b.lines = vector<line>(); // creates line vectors for them

	line bi = {getIntersection(toCut,perpBi,vec2(centreX,centreY)), getIntersection(l,perpBi,vec2(centreX,centreY)),0};
	line antiBi = {getIntersection(l,perpBi,vec2(centreX,centreY)), getIntersection(toCut,perpBi,vec2(centreX,centreY)), 0};

	a = getInnerSection(s, bi, toCut, l);
	b = getInnerSection(s, antiBi, l, toCut);

	vector<section> x = vector<section>();
	x.push_back(a);
	x.push_back(b);

	for(section l : x){
		cout << "Section " << l.ID << endl;
		for(line lin : l.lines){
			cout<< lin.start.x << " " << lin.start.y << " " << lin.end.x << " " << lin.end.y << "  ID  " <<  lin.ID << endl;
		}
	}

	return x;

}

section SectionDivider::getInnerSection(section s, line bi, line toCut, line longLine) {
	section a;
	a.lines.push_back(bi);
	int lineID = (toCut.ID + 1)%s.lines.size();
//	if (lineID >= (int)s.lines.size()) {
//		lineID = 0;
//	}
	int newID = 1;

	// Add first half line
	vec2 end = getIntersection(bi, toCut);
	cout << "Pre shared point line ID " << lineID << "  " << s.lines[lineID].ID << endl;
	vec2 start = getSharedPoint(toCut, s.lines[lineID]);

	//cout << "Section cutter " << start.x << "  " << start.y << "  " << end.x << "  " << end.y << "  " << endl;

	line startHalf = { start,end,newID++ };
	a.lines.push_back(startHalf);

	//	//Adds all whole middle lines
	while (lineID != longLine.ID) {

		//cout << "Line ID " << lineID << endl;

		//Add line
		line toAdd = s.lines[lineID];
		toAdd.ID = newID; // Gives line appropriate ID
		a.lines.push_back(toAdd);

		// Get correct lineID
		lineID++;
		if (lineID >= (int)s.lines.size()) {
			lineID = 0;
		}
		// Increment new ID
		newID++;
	}

	// Add last half line

	vec2 end2 = getIntersection(bi, longLine);
	lineID --;
	if(lineID < 0){
		lineID = (int)s.lines.size() - 1;
	}

	cout << "Pre shared point at the end line ID " << lineID << "  " << s.lines[lineID].ID << endl;
	vec2 start2 = getSharedPoint(longLine, s.lines[lineID]);

	line endHalf = { start2,end2,newID++ };
	a.lines.push_back(endHalf);

	// Needs to calculate area here
	a.area = getSectionSize(a);
	return a;
}

void SectionDivider::testSection() {
	line a = {vec2(100,100),vec2(400,100),0};
	line d = {vec2(150,300), vec2(100,100), 3 };
	line c = {vec2(350,300),vec2(150,300), 2};
	line b = { vec2(400,100),  vec2(350,300),1 };

	vector<line > lines = vector<line >();
	lines.push_back(a);
	lines.push_back(b);
	lines.push_back(c);
	lines.push_back(d);

	section s = { lines };
	s.area = getSectionSize(s);
	//sections.push_back(s);

	lot l;
	l.boundingBox = s;
	l.ID = 0;
	l.sections = vector<section>();

	lots.push_back(l);
	divideLot(l);

	cout<< endl;
	cout << "Subdividing New Section " << endl;
	cout << " ------------------------ " << endl;
	cout <<  endl;

	vector<section> newSec = splitSection(s);
	//	for(section sec : newSec){
	//		sections.push_back(sec);
	//	}

		for(section sec: newSec){
			vector<section> miniSec = splitSection(sec);
			for(section sl : miniSec){
				vector<section> s2sec= splitSection(sl);
				for(section s2 : s2sec){
				sections.push_back(s2);
				}
			}
		}

//	sections.push_back(
//			splitSection(newSec[0])[1]
//	);

	//for(section sec : newSec){
	//	cout<< endl;
	//	cout << "Subdividing New Section " << endl;
	//	cout << " ------------------------ " << endl;
	//	cout <<  endl;
	//	vector<section> miniSecs = splitSection(newSec[0]);
	//	for(section mini : miniSecs){
	//		cout<< endl;
	//		cout << "Subdividing New Section " << endl;
	//		cout << " ------------------------ " << endl;
	//		cout <<  endl;
	//		vector<section> mm = splitSection(mini);
	//		for(section m : mm){
	//			sections.push_back(m);
	//		}
	//	}
	//	//}
	//
	//
	//	cout << endl;
//		sections.push_back(newSec[0]);
//		vector<section> miniSecs = splitSection(newSec[0]);
//		for(section mini : miniSecs){
//			sections.push_back(mini);
//		}



	//sections.push_back(newSec[1]);
}

vec2 SectionDivider::getSharedPoint(line a, line b) {
	cout << "ID's being compared " << a.ID << "  " << b.ID << endl;

	try{
		getIntersection(a,b);
	}catch(const noIntersectionException &e){cout << "Parallel in shared point" << endl;}


	if ((a.start.x == b.start.x && a.start.y == b.start.y )||( a.start.x == b.end.x && a.start.y == b.end.y)){
		return a.start;
	}
	else if ((a.end.x == b.start.x && a.end.y == b.start.y) || (a.end.x == b.end.x && a.end.y == b.end.y)) {
		return a.end;
	}
	cout << "Returning 0 0"  << endl;
	return vec2(0,0);
}

float SectionDivider::getSectionSize(section s) {
	float size = 0.0f;
	for (int i = 0; i < (int)s.lines.size(); i++) {
		line l = s.lines[i];
		size = size + (l.start.x * l.end.y + l.end.x * l.start.y);
	}
	size /= 2.0f;
	return size;
}

void SectionDivider::renderTest() {
	//section s = sections.back();


	//cout<< "rendering " << endl;

	for(int i = 0; i < (int)sections.size(); i++){ // (int)sections.size()
		glBegin(GL_LINES);
		//cout << "Section " << i << endl;
		//	line longLine = findLongestEdge(sections[0]);
		int longID  = findLongestEdge(sections[i]).ID;
		//	cout << "LongLIne: " << longLine.ID << endl;
		for (line l : sections[i].lines) {
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
}

//void SectionDivider::cleanUp(){
//	for(line * l : sections[0].lines){
//		free(l);
//	}
//}



