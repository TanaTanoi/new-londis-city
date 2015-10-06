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

/**
 * Divides the sections
 */

void SectionDivider::divideLot(lot l) {
	cout << "Number of sections " << l.sections.size() << endl;
	lots[l.ID] = recDivideSection(l,l.boundingBox);
	//lots[l.ID] = removeUnusableSections(lots[l.ID]);
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
	//cout << "Longest edge ID " << l.ID << endl;
	vec2 lineVec =  l.end - l.start;
	vec2 lineVec2 =  l.start - l.end;

	cout << "Line vectors" << lineVec.x << "," << lineVec.y << " "<< lineVec2.x << "," << lineVec2.y << endl;

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
		centreY = min(l.end.y, l.start.y) + 0.51*leng;//min(l.end.y, l.start.y) + leng*(5.0f / 12.0f) + random * (leng*(1.0f / 6.0f));
	}
	else{
		centreX = min(l.end.x, l.start.x) + 0.51*leng;        //min(l.end.x, l.start.x) + leng*(5.0f / 12.0f) + random * (leng*(1.0f / 6.0f));
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
		//cout << "Checking intersects line ID " << s.lines[i].ID << endl;
		if(s.lines[i].ID != l.ID){
			//cout << "Checking intersects" << endl;
			if (intersects(s.lines[i],perpBi,vec2(centreX,centreY))) { // FIXME : Issue is here, further subdivisions are not finding any intersections
				//cout << "Found intersector" << endl;
				intersectors.push_back(s.lines[i]);
			}
		}
	}

	//cout<< "Intersecting Lines found" << intersectors.size() << endl;

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

	line bi = { getIntersection(l,perpBi,vec2(centreX,centreY)), getIntersection(toCut,perpBi,vec2(centreX,centreY)), 0};
	line antiBi = {bi.end, bi.start, bi.ID};

	line antiLong = {l.end, l.start, l.ID};
	line antiCut = {toCut.end, toCut.start, toCut.ID};

	a = getInnerSection(s, bi, toCut, l);
	b = getInnerSection(s, antiBi, antiLong, antiCut);

	vector<section> x = vector<section>();
	x.push_back(a);
	x.push_back(b);

	for(section l : x){
		//cout << "Section " << l.ID << endl;
		for(line lin : l.lines){
			if(lin.end.y > 300|| lin.start.y > 300){
				//cout << "____________ Line is higher than 300 _____________" << endl;
				if(lin.ID == 0){
					//cout << "Issue is bisector" << endl;
				}
			}
		}
	}

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

void SectionDivider::testSection() {
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

	vector<line > lines = vector<line >();
	lines.push_back(a);
	lines.push_back(b);
	lines.push_back(c);
	lines.push_back(d);

	//section s = { lines };
	section s = Generator::createRandomSection();
	s.area = getSectionSize(s);
	cout << s.area << endl;
	//sections.push_back(s);

	lot l;
	l.boundingBox = Generator::createRandomSection();
	l.ID = 0;
	l.sections = vector<section>();

	lots.push_back(l);
	divideLot(l);

	//	vector<section> sec1 = splitSection(newSec[1]);
	//	vector<section> sec2 = splitSection(sec1[1]);
	//	vector<section> sec3 = splitSection(sec2[0]);
	//	vector<section> sec4 = splitSection(sec3[0]);
	//	vector<section> sec45 = splitSection(sec3[1]);
	//
	//	sections.push_back(newSec[1]);
	//	sections.push_back(sec1[1]);
	//	sections.push_back(sec2[1]);
	//	sections.push_back(sec3[1]);
	//
	//	sections.push_back(sec45[0]);
	//	sections.push_back(sec45[1]);
	//	sections.push_back(sec4[0]);
	//	sections.push_back(sec4[1]);

	//	for(section sec : newSec){d
	//		sections.push_back(sec);
	//	}
	int i = 0;
	int count = 4;
	vector<section> newSec = splitSection(s);

	//	while(i < count){
	//		vector<section> holder = vector<section>();
	//
	//		for(int j = 0; j < newSec.size(); j ++){
	//			vector<section> split = splitSection(newSec[j]);
	//			holder.push_back(split[0]);
	//			holder.push_back(split[1]);
	//		}
	//
	//		newSec = holder;
	//
	//		i++;
	//	}

	for(section t : newSec){
		sections.push_back(t);
	}

	//	for(section sec: newSec){
	//		vector<section> miniSec = splitSection(sec);
	//		for(section sl : miniSec){
	//			vector<section> s2sec= splitSection(sl);
	//			for(section s2 : s2sec){
	//				vector<section> s3sec= splitSection(s2);
	//				for(section s3: s3sec){
	//					vector<section> s4sec= splitSection(s3);
	//					for(section s4: s4sec){
	//													for(section s5:splitSection(s4)){
	//														for(section s6:splitSection(s5)){
	//															sections.push_back(s6);
	//														}
	//						//sections.push_back(s4);
	//						//
	//						//
	//													}
	//					}
	//					//sections.push_back(s3);
	//				}
	//			}
	//		}
	//	}

	//	sections.push_back(
	//			splitSection(newSec[0])[1]
	//	);
	//	sections.push_back(splitSection(splitSection(splitSection(newSec[0])[1])[0])[0]);
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
	renderWireFrame();
	//renderPoly();
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



