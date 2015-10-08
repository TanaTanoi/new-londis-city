/*
 * roadNetwork.cpp
 *
 *  Created on: 30/09/2015
 *      Author: craighhann
 */

#include "roadNetwork.hpp"
#include "comp308.hpp"
#include "utility.hpp"

using namespace util;
using namespace comp308;
using namespace std;

RoadNetwork::RoadNetwork(){}

/**
 * Checks whether a road will be fully inside a the world
 * which is represented by a section
 *
 * Returns 0 for inside, 1 for intersecting and 2 for completely outside
 */
int RoadNetwork::insideWorld(road r ){
	line road = {r.start.location, r.end.location};

	// should check is roadLine intersects any of the sectionLines
	for(line l : outline.lines){
		//		if(intersects(road,line)){ //write this at some point
		//			return 1;
		//		}
	}

	// if not then it should somehow determine the highest/lowest rightmost and leftmost values and check against those
	if(road.start.y > maxHeight && road.end.y > maxHeight){ return 2; } // too high
	if(road.start.y < minHeight && road.end.y < minHeight){ return 2; } // too low
	if(road.start.x > farRight && road.end.x > farRight){ return 2; } // too far right
	if(road.start.x < farLeft && road.end.x < farLeft){ return 2; } // too far left

	return 0;

}

/**
 * Checks whether a point is inside or outside the world outline
 */
int RoadNetwork::insideWorld(vec2 point){
	// Quick bounding box check for efficiency
	if(point.y > maxHeight || point.y < minHeight || point.x > farRight || point.x < farLeft){
		return false;
	}
	// Now actually checks per line
	// Algorithm from http://www.codeproject.com/Tips/84226/Is-a-Point-inside-a-Polygon

	bool c = false;
	for(line l : outline.lines){
		if ( ((l.end.y>point.y) != (l.start.y>point.y)) &&
				(point.x < (l.start.x-l.end.x) * (point.y-l.end.y) / (l.start.y-l.end.y) + l.end.x) )
			c = !c;
	}
	return c;
}

/**
 * Truncates the road up to 2/3 of it's original size so it
 * will fit inside the map. Will throw an exception if this is not possible
 *
 * This method assumes that the road has already been confirmed to have an intersection
 */
road RoadNetwork::truncate(road r){
	line l = {r.start.location, r.end.location};
	// Need to get intersection put
	// Then pull inside a little
	// Get length of line
	// return if at least 2/3 of original length
	return r; // just for compilation
}

/**
 * Adds an intersection between to nodes to the adjacency list
 */
void RoadNetwork::addIntersection(int id1, int id2){
	adjacencyList[id1].push_back(id2);
	adjacencyList[id2].push_back(id1);
	// NOTE: need to deal with when these intersections destroy other intersections
}

/**
 * Adds new node to the list of all road nodes
 * giving it a new ID
 */
void RoadNetwork::addNode(vec2 point){
	roadNode r = {point, nodeID++};
	allNodes.push_back(r);
}

/**
 * Adds new road to list of all roads
 */
void RoadNetwork::addRoad(roadNode start,roadNode end){
	road r = {start,end};
	allRoads.push_back(r);
}

/**
 * Calulates the most extreme points of the section
 * (i.e. min and max heights as well as the left and rightmost values)
 */
void RoadNetwork::calulateBoundary(){
	for(int i = 0; i < (int)outline.lines.size(); i++){
		if(i ==0){ // Sets all to be a value at first line
			maxHeight = max(outline.lines[i].start.y, outline.lines[i].end.y);
			minHeight = min(outline.lines[i].start.y, outline.lines[i].end.y);
			farLeft = min(outline.lines[i].start.x, outline.lines[i].end.x);
			farRight = max(outline.lines[i].start.x, outline.lines[i].end.x);
		}
		else{
			maxHeight = max(maxHeight, max(outline.lines[i].start.y, outline.lines[i].end.y));
			minHeight = min(minHeight, min(outline.lines[i].start.y, outline.lines[i].end.y));
			farLeft = min(farLeft, min(outline.lines[i].start.x, outline.lines[i].end.x));
			farRight = max(farRight, max(outline.lines[i].start.x, outline.lines[i].end.x));
		}
	}
}

// Assumes square world
void RoadNetwork::createNewYorkGrid(section s){
	line l = s.lines[0]; // gets first line in outline

	// Creates first road
	vec2 perpBi = getBisector(l);
	vec2 start = centrePointOfLine(l);
	vec2 end = vec2(start.x, 400);
}

void RoadNetwork::renderRoads(){
	for(road r : allRoads){
		// draw
	}
}

void RoadNetwork::createRoads(section world){
	outline = world;
	calulateBoundary();
	//createNewYorkGrid(outline);
	// Now take in population density
	// Now generate highways
	// Now generate minor roads
}

void RoadNetwork::testNetwork(){

	line a = {vec2(500,500), vec2(100,500), 0};
	line b = {vec2(100,500), vec2(100,100), 1};
	line c = {vec2(100,100), vec2(500,100), 2};
	line d = {vec2(500,100), vec2(500,500), 3};

	vector<line> lines;
	lines.push_back(a);
	lines.push_back(b);
	lines.push_back(c);
	lines.push_back(d);


	section s;
	s.lines = lines;

	createRoads(s);

}

void RoadNetwork::renderRoads(){
	glColor3f(1.0f,1.0f,0.0f);
	glBegin(GL_LINES);
		for(line l : outline.lines){
			cout << " beep" << endl;
			glVertex2f(l.start.x, l.start.y);
			glVertex2f(l.end.x, l.end.y);
		}
	glEnd();
}



