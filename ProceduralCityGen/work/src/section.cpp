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
#include "generator.h"
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

void SectionDivider::divideAllLots(vector<section> lotOutlines){
	for(section s: lotOutlines){
		s.area = getSectionSize(s);
		lot l = {s,vector<section>(),lotID++};
		lots.push_back(l);
		divideLot(l);
	}
}

/**
 * Divides the sections
 */

void SectionDivider::divideLot(lot l) {
	cout << "Number of sections " << l.sections.size() << endl;
	lots[l.ID] = recDivideSection(l,l.boundingBox);
	lots[l.ID] = removeUnusableSections(lots[l.ID]);
}

void SectionDivider::addBuildingToLot(lot l){
	lots[l.ID] = l;
}


/**
 * Recursively divides sections by extending a perpendicular bisector
 * of the longest edge slightly off centre. Continuely recurses until
 * it is smaller than the largest acceptable size for a section.
 */

lot SectionDivider::recDivideSection(lot lot, section s) {
	cout << "Rec divide" << endl;
	vector<section> secs = splitSection(s);
	//cout << "section size" << secs.size() << endl;
	//cout << "Goal size" << goalArea <<endl;
	for (section l : secs) {
		//cout << " Testing  "<<l.area << endl;
		if (l.area <= goalArea) {
			l.ID = sectionID++;
			lot.sections.push_back(l);
			//cout << " Found a section " << endl;
		}
		else {
			//			//	cout << "Recursively dividing" << endl;
			//			//	cout << "Size is " << l.area << endl;
			//
			lot = recDivideSection(lot, l);
		}
	}
	return lot;
}

//
vector<section> SectionDivider::splitSection(section s) {
	line l = findLongestEdge(s);
	vec2 perpBi = getBisector(l);

	// Randomly decides a position around the centre of the longest edge
	// to extend the bisector from. This will be within the middle sixth of the section

	float random = ((float)rand() / (RAND_MAX));

	vec2 centrePoint = centrePointOfLine(l);


	// Now finds the first intersection point with another line within the section
	vector<line> intersectors = linesIntersectingWithSection(s, perpBi, centrePoint, l);

	line toCut;
	float close = 0.0f;

	for (int i = 0; i < (int)intersectors.size(); i++) {
		float dist = abs(getIntersection(intersectors[i], perpBi, centrePoint).y - centrePoint.y);
		if (i == 0 || dist < close) {
			close = dist;
			toCut = intersectors[i];
		}
	}


	// Now creates two new sections from the split

	section a, b; // the two new sections
	a.lines = vector<line>(); b.lines = vector<line>(); // creates line vectors for them

	line bi = { getIntersection(l,perpBi,centrePoint), getIntersection(toCut,perpBi,centrePoint), 0};
	line antiBi = {bi.end, bi.start, bi.ID};

	line antiLong = {l.end, l.start, l.ID};
	line antiCut = {toCut.end, toCut.start, toCut.ID};

	a = getInnerSection(s, bi, toCut, l);
	b = getInnerSection(s, antiBi, antiLong, antiCut);

	vector<section> x = vector<section>();
	x.push_back(a);
	x.push_back(b);

	return x;

}

section SectionDivider::getInnerSection(section s, line bi, line toCut, line longLine) {

	//cout<< endl;
	//cout << "Getting New Section " << endl;
	//cout << " ------------------------ " << endl;
	//cout <<  endl;

	bool isTriangle = true;

	section a;
	a.lines.push_back(bi);
	int lineID = (toCut.ID + 1)%s.lines.size();
	//	if (lineID >= (int)s.lines.size()) {
	//		lineID = 0;
	//	}
	int newID = 1;

	// Add first half line
	vec2 start = getIntersection(bi, toCut);
	//cout << "Pre shared point line ID " << lineID << "  " << s.lines[lineID].ID << endl;
	vec2 end = getSharedPoint(toCut, s.lines[lineID]);
	if(end.y > 300){
		//cout << "Issue here" << endl;
	}

	//cout << "Section cutter " << start.x << "  " << start.y << "  " << end.x << "  " << end.y << "  " << endl;


	line startHalf = { start,end,newID++ };
	a.lines.push_back(startHalf);

	//	//Adds all whole middle lines
	while (lineID != longLine.ID) {
		isTriangle = false;
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
	//	if(isTriangle){
	//		cout<< endl;
	//		cout<< "I am a triangle " << endl;
	//		bool bibi =  bi.ID == lineID;
	//		bool longli = longLine.ID == lineID ;
	//		cout << "Bi: "  << bibi <<  "  LongLine: " << longli << endl;
	//	}
	//else{
	lineID --;
	//	}


	if(lineID < 0){
		lineID = (int)s.lines.size() - 1;
	}

	//cout << "Pre shared point at the end line ID " << lineID << "  " << s.lines[lineID].ID << endl;
	vec2 start2 = getSharedPoint(longLine, s.lines[lineID]);

	line endHalf = { start2,end2,newID++ };
	a.lines.push_back(endHalf);

	// Needs to calculate area here
	a.area = getSectionSize(a);
	return a;
}

lot SectionDivider::testSection() {
				line a = {vec2(100,100),vec2(400,100),0};
				line d = {vec2(150,300), vec2(100,100), 3 };
				line c = {vec2(350,300),vec2(150,300), 2};
				line b = { vec2(400,100),  vec2(350,300),1 };
	//
//	line a = {vec2(150,100),vec2(400,100),0};
//	line d = {vec2(100,400), vec2(150,100), 3 };
//	line c = {vec2(400,400),vec2(100,400), 2};
//	line b = { vec2(400,150),  vec2(400,400),1 };

//		line a = {vec2(100,100),vec2(400,100),0};
//		line b = {vec2(400,100),vec2(250,300),1};
//		line c = {vec2(250,300),vec2(100,100),2};
//
//	vector<line > lines = vector<line >();
//	lines.push_back(a);
//	lines.push_back(b);
//	lines.push_back(c);
//	lines.push_back(d);


	section s = Generator::createRandomSection();
	s.area = getSectionSize(s);

	lot l;
	l.boundingBox = Generator::createRandomSection();
	l.ID = 0;
	l.sections = vector<section>();

	lots.push_back(l);
	divideLot(l);

	return lots[0];
}

vec2 SectionDivider::getSharedPoint(line a, line b) {
	//cout << "ID's being compared " << a.ID << "  " << b.ID << endl;
	vec2 inter;
	try{
		inter = getIntersection(a,b);
	}catch(const noIntersectionException &e){cout << "Parallel in shared point" << endl;}

	int asX = (int) round((a.start* 10.0).x); int aeX = (int) round((a.end* 10.0).x); int bsX = (int) round((b.start* 10.0).x); int beX = (int) round((b.end* 10.0).x);
	int asY = (int) round((a.start* 10.0).y); int aeY = (int) round((a.end* 10.0).y); int bsY = (int) round((b.start* 10.0).y); int beY = (int) round((b.end* 10.0).y);

	if ((asX == bsX && asY == bsY) || (asX == beX && asY == beY)){
		return a.start;
	}
	else if ((aeX == bsX && aeY == bsY) || (aeX == beX && aeY == beY)) {
		return a.end;
	}

	//cout << "A: " << a.start* 10 << endl;
	//cout << "B: " << b.start*10 << endl;
	//cout << "A: " << asX << " " << asY << " , "  << aeX << " " << aeY << endl;
	//cout << "B: " << bsX << " " << bsY << " , "  << beX << " " << beY << endl;

	return inter;
}

float SectionDivider::getSectionSize(section s) {
	float size = 0.0f;
	for (int i = 0; i < (int)s.lines.size(); i++) {
		line l = s.lines[i];
		float change = (l.end.x + l.start.x) * (l.end.y-l.start.y);
		if(!isNAN(change)){
			size = size +  change;
		}
	}
	size /= 2.0f;
	//cout << "Size is " << size << endl;
	return size;
}

/**
 * Removes all sections from a lot that do not have street access
or are too small to have a building
 */
lot SectionDivider::removeUnusableSections(lot l) {
	vector<section> toKeep = vector<section>();

	for (section s : l.sections) { // checks each section
		if (s.area > minArea) { // checks if section size is big enough
			cout << "Section is big enough" << endl;
			if (hasStreetAccess(s,l)) {
				cout << "Section added" << endl;
				toKeep.push_back(s); // adds to collection to keep
			}
		}
	}
	l.sections = toKeep;
	return l;
}

bool SectionDivider::hasStreetAccess(section s, lot l) {
	cout << "Section ID " << s.ID;
	for (line edge : l.boundingBox.lines) { // checks against each edge of section
		for (line sectionEdge : s.lines) {
			if(s.ID == 7){
				cout << "Seven found" << endl;
				if(shareSlope(edge, sectionEdge)){ cout << "Seven has same slope" << endl;}
			}
			if(shareSide(edge, sectionEdge)){
				return true;
			}
		}
	}
	return false;
}

void SectionDivider::renderTest() {
	//renderWireFrame();
	renderPoly();
}

void SectionDivider::renderWireFrame(){
	for(int i =0; i < (int)lots[0].sections.size(); i++){ // (int)sections.size()

//		if(i == 8){
//			cout << lots[0].sections[8].lines[0].start.x << "," <<  lots[0].sections[8].lines[0].start.y
//					<< "  " <<  lots[0].sections[8].lines[0].end.x << "," << lots[0].sections[8].lines[0].end.y << endl;
//		}


		glBegin(GL_LINES);
		//cout << "Section " << i << endl;
		//	line longLine = findLongestEdge(sections[0]);
		int longID  = findLongestEdge(lots[0].sections[i]).ID;
		//	cout << "LongLIne: " << longLine.ID << endl;
		srand(lots[0].sections[i].ID);
		float r = ((float)rand() / (RAND_MAX));srand(rand());
		float g = ((float)rand() / (RAND_MAX));srand(rand());
		float b = ((float)rand() / (RAND_MAX));srand(rand());
		for (line l : lots[0].sections[i].lines) {
			glColor3f(r,g, b);
			glVertex2f(l.start.x, l.start.y);
			glVertex2f(l.end.x, l.end.y);
		}
		glEnd();
	}
}

void SectionDivider::renderPoly(){
	for(int i = 0; i<	(int)lots[0].sections.size(); i++){ // 55 -14
		//cout << "area " << lots[0].sections[i].area << endl;


		glBegin(GL_POLYGON);
		int longID  = findLongestEdge(lots[0].sections[i]).ID;
		srand(lots[0].sections[i].ID);
		float r = ((float)rand() / (RAND_MAX));srand(rand());
		float g = ((float)rand() / (RAND_MAX));srand(rand());
		float b = ((float)rand() / (RAND_MAX));srand(rand());
		for (line l : lots[0].sections[i].lines) {
			glColor3f(r,g, b);
			glVertex2f(l.start.x, l.start.y);
			glVertex2f(l.end.x, l.end.y);
		}
		glEnd();
	}
}



