/*
 * roadNetwork.cpp
 *
 *  Created on: 30/09/2015
 *      Author: craighhann
 */

#include "roadNetwork.hpp"
#include "comp308.hpp"
#include "utility.hpp"
#include "cycleUtil.hpp"

using namespace util;
using namespace cycle;
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
roadNode RoadNetwork::addNode(vec2 point){
	roadNode r = {point, nodeID++};
	allNodes.push_back(r);
	adjacencyList[r.ID] =  vector<int>();
	return r;
}

/**
 * Adds new road to list of all roads
 */
road RoadNetwork::addRoad(roadNode start,roadNode end){
	road r = {start,end,roadID++};
	allRoads.push_back(r);
	return r;
}

/**
 * Note, this method assumes you know that the road is split by
 * the given node
 */
void RoadNetwork::updateAdjacencyList(road r, roadNode n){
	// remove r from allRoads

	// remove end from start and vice versa in adjacency list
	for(int i = 0; i < (int)adjacencyList[r.start.ID].size(); i++){
		if(adjacencyList[r.start.ID][i] == r.end.ID){
			adjacencyList[r.start.ID][i] = n.ID;
			break;
		}
	}

	for(int i = 0; i < (int)adjacencyList[r.end.ID].size(); i++){
		if(adjacencyList[r.end.ID][i] == r.start.ID){
			adjacencyList[r.end.ID][i]= n.ID;
			break;
		}
	}

	// adds start and end of road to n in adjacency list
	adjacencyList[n.ID].push_back(r.start.ID);
	adjacencyList[n.ID].push_back(r.end.ID);

	// create two new roads start-n and n-end
	road r1 = {r.start,n,roadID++};
	road r2 = {n,r.end,roadID++};
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

void RoadNetwork::recDivideGrid(road r, int level,bool halfLength){
	cout << "Dividing grid" << endl;

	line l = {r.start.location,r.end.location};
	vec2 perpBi = getBisector(l);
	vec2 centrePoint = centrePointOfLine(l);

	// Now needs to create a node at centre point and update the adjacency list
	roadNode rn = addNode(centrePoint);
	updateAdjacencyList(r,rn);
	// Then needs to extend in to opposite directions from centrePoint and add
	float length = getLength(l);
	if (halfLength) {
		length = length / 2.0;
	}

	vec2 a = getLineForLength(centrePoint,perpBi,length);
	vec2 b = getLineForLength(centrePoint,-perpBi,length);

	roadNode r1 = addNode(a);
	roadNode r2 = addNode(b);
	road left = addRoad(rn,r1);
	road right = addRoad(rn,r2);

	if(level < 6){
		recDivideGrid(left, level+1,!halfLength);
		recDivideGrid(right, level+1,!halfLength);
	}

}

// Assumes square world
void RoadNetwork::createNewYorkGrid(section s){
	line l = s.lines[0]; // gets first line in outline

	// Creates first road
	vec2 perpBi = getBisector(l);
	vec2 start = centrePointOfLine(l);
	vec2 end = vec2(start.x, 100);

	// Adds first road
	addNode(start);
	addNode(end);
	addRoad(allNodes[0],allNodes[1]);

	// Now recursively subdivide
	recDivideGrid(allRoads[0],0,true);

}

void RoadNetwork::createRoads(section world){
	outline = world;
	calulateBoundary();
	testAntiClockwise();
	createNewYorkGrid(outline);
	// Now take in population density
	// Now generate highways
	// Now generate minor roads
}

void RoadNetwork::testNetwork(){

	line a = {vec2(400,400), vec2(100,400), 0};
	line b = {vec2(100,400), vec2(100,100), 1};
	line c = {vec2(100,100), vec2(400,100), 2};
	line d = {vec2(400,100), vec2(400,400), 3};

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
	glColor3f(1.0f,1.0f,0.0f); // yellow world bounds
	glBegin(GL_LINES);
	for(line l : outline.lines){
		glVertex2f(l.start.x, l.start.y);
		glVertex2f(l.end.x, l.end.y);
	}
	glEnd();

	glColor3f(1.0f,0.0f,0.0f); // red roads
	glBegin(GL_LINES);
	for(road r : allRoads){
		glVertex2f(r.start.location.x, r.start.location.y);
		glVertex2f(r.end.location.x, r.end.location.y);
	}
	glEnd();

	glColor3f(0.0,0.0,1.0); // blue intersections
	glPointSize(10);

	glBegin(GL_POINTS);
	for(roadNode n : allNodes){
		glVertex2f(n.location.x, n.location.y);
	}
	glEnd();
}


vector<primitive> RoadNetwork::extractPrimitives(){
	vector<primitive> primitives;
	vector<roadNode> heap = sortPoints(allNodes);
	map<int,vector<int>> adjacencies = adjacencyList;
	vector<road> roads = sortRoads(allRoads);

	while((int)heap.size() > 0){
		roadNode vertex = heap[0];
		vector<int> adjs = adjacencies[vertex.ID];
		int noAdj = (int)adjs.size();

		if(noAdj == 0){
			// extract isolated vertex
			extractIsolatedVertex(&primitives, &heap, &adjacencies);
		}

		else if ( noAdj == 1){
			// extract filament
			extractFilament(vertex.ID, adjacencies[vertex.ID][0] ,&primitives, &heap, &adjacencies, &roads);
		}

		else{
			// extract filament or cycle
		}

	}

	return primitives;
}

void RoadNetwork::extractIsolatedVertex(vector<primitive> * primitives, vector<roadNode> * heap, map<int,vector<int>> * adjs){
	vector<roadNode> vertices;
	vertices.push_back(heap->front()); // removes roadNode from heap
	allNodes[vertices[0].ID].visited = true;
	primitive p = {vertices,0}; // adds vertex to primitive and sets it as isolated vertex
	adjs->erase(vertices[0].ID); // removes from adjacency list
	primitives->push_back(p);
}

// Note: visiting a vertex should remove it from adjacency list

void RoadNetwork::extractFilament(int startID, int endID, vector<primitive> * primitives, vector<roadNode> * heap, map<int,vector<int>> * adjs, vector<road> * roads){
	if((*roads)[findRoadIndex((*roads),startID,endID)].isCycleEdge){// the edge between startID (heap[0]) and an adjacent vertex of ID endID
		if((int)(*adjs)[startID].size() >= 3){

			int toRemove = findRoadIndex((*roads),startID,endID); //remove edge v0 to v1
			roads->erase(roads->begin() + toRemove);

			startID = endID;
			if((int)(*adjs)[startID].size() == 1){
				endID = (*adjs)[startID][0];
			}
		}

		while((int)(*adjs)[startID].size() == 1){
			endID = (*adjs)[startID][0];

			if((*roads)[findRoadIndex((*roads),startID,endID)].isCycleEdge){//  if cycle edge
				removeFromHeap(heap,startID);// remove from heap
				int toRemove = findRoadIndex((*roads),startID,endID); //remove edge v0 to v1
				roads->erase(roads->begin() + toRemove);// remove edge
				removeAdjacencyLinks(startID,adjs); //remove vertex
				startID = endID;
			}
			else{
				break;
			}
		}

		if((int)(*adjs)[startID].size() == 0){
			removeFromHeap(heap,startID);//remove from heap
			removeAdjacencyLinks(startID,adjs); //remove vertex
		}
	}
	else{
		primitive p;
		p.type = 1; // sets to filament

		if((int)adjs[startID].size() >= 3){
			p.vertices.push_back(allNodes[startID]);
			int toRemove = findRoadIndex((*roads),startID,endID); //remove edge v0 to v1
			roads->erase(roads->begin() + toRemove);//remove edge
			startID = endID;
			if((int)(*adjs)[startID].size() == 1){
				endID = (*adjs)[startID][0];
			}
		}

		while((int)(*adjs)[startID].size() == 1){
			p.vertices.push_back(allNodes[startID]);
			endID = (*adjs)[startID][0];
			removeFromHeap(heap,startID);//remove from heap
			int toRemove = findRoadIndex((*roads),startID,endID); //remove edge v0 to v1
			roads->erase(roads->begin() + toRemove);//remove edge
			removeAdjacencyLinks(startID,adjs); //remove vertex
			startID = endID;
		}

		p.vertices.push_back(allNodes[startID]);
		if((int)(*adjs)[startID].size() == 0){
			removeFromHeap(heap,startID);//remove from heap
			removeAdjacencyLinks(startID,adjs); //remove vertex
		}
		primitives->push_back(p);
	}
}

void RoadNetwork::extractPrimitive(vector<primitive> * primitives, vector<roadNode> * heap, map<int,vector<int>> * adjs, vector<road> * roads){
	vector<roadNode> visited;
	vector<roadNode> sequence;

	roadNode start = (*heap)[0]; // v0
	roadNode v1 = getClockwiseMost(start, (*adjs)[start.ID]); // v1

	sequence.push_back(start);

	roadNode vprev = start;
	roadNode vcurr = v1;



	while(vcurr.ID != start.ID && contains(visited,vcurr)){
		if((int)(*adjs)[vcurr.ID].size() == 1){
			break;
		}
		sequence.push_back(vcurr);
		visited.push_back(vcurr);

		roadNode vnext = getAntiClockwiseMost(vprev,vcurr,(*adjs)[vcurr.ID]);
		vprev = vcurr;
		vcurr = vnext;
	}

	if((int)(*adjs)[vcurr.ID].size() == 1){
		//Filament found, may not actaully start at vprev, but it is a part of it
		extractFilament(vprev.ID, (*adjs)[vprev.ID][0], primitives, heap, adjs, roads);
	}

	else if(vcurr.ID == start.ID){
		// Minimal cycle found
		primitive p = {sequence, 2};

		// for each edge in the cycle
		int j = (int)sequence.size() -1;
		for(int i = 0; i < (int)sequence.size(); i++){
			setCycleEdge(roads,i,j);
		}

		int toRemove = findRoadIndex((*roads),start.ID,v1.ID); //remove edge v0 to v1
		roads->erase(roads->begin() + toRemove);//remove edge

		if((int)(*adjs)[start.ID].size() == 1){
			extractFilament(start.ID, (*adjs)[start.ID][0], primitives, heap, adjs, roads);
		}

		if((int)(*adjs)[v1.ID].size() == 1){
			extractFilament(v1.ID, (*adjs)[v1.ID][0], primitives, heap, adjs, roads);
		}
	}

	else{

		while((int)(*adjs)[start.ID].size() == 2){
			if((*adjs)[start.ID].at(0) != v1.ID){
				v1 = start;
				start = (*adjs)[start.ID][0];
			}
			else{
				v1 = start;
				start = (*adjs)[start.ID][1];
			}
		}
		extractFilament(start.ID, v1.ID, primitives, heap, adjs, roads);
	}

}

void RoadNetwork::setCycleEdge(vector<road> * roads, int startID, int endID){
	int toSet = findRoadIndex((*roads),startID, endID);
	(*roads)[toSet].isCycleEdge = true;
}

road RoadNetwork::findClosestRoads(vec3 position) {
	return road();
}

// Gets the clockwiseMost adjacent vertex
// Returns the ID of it
roadNode RoadNetwork::getClockwiseMost(roadNode current, vector<int> adj){
	vec2 dCurr = vec2(0,-1); //support line
	roadNode vnext = allNodes[adj[0]]; // sets up first adjacent vertex
	vec2 dnext = vnext.location -current.location;
	bool vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;

	for(int i = 1; i < (int)adj.size(); i++){
		vec2 dirAdj = allNodes[adj[i]].location - current.location; // gets direction of adjacent vertex
		if(vcurrIsConvex){
			if(DotPerp(dCurr,dirAdj) < 0 || DotPerp(dnext,dirAdj) < 0){
				vnext = allNodes[adj[i]];
				dnext = dirAdj;
				vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;
			}
		}
		else{
			if(DotPerp(dCurr,dirAdj) < 0 && DotPerp(dnext,dirAdj) < 0){
				vnext = allNodes[adj[i]];
				dnext = dirAdj;
				vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;
			}

		}
	}
	return vnext;
}

// Gets the clockwiseMost adjacent vertex
// Returns the ID of it
roadNode RoadNetwork::getAntiClockwiseMost(roadNode vprev, roadNode current, vector<int> adj){
	vec2 dCurr = current.location - vprev.location; //support line
	roadNode vnext = allNodes[adj[0]]; // sets up first adjacent vertex

	if(adj[0] == vprev.ID){
		vnext = allNodes[adj[1]];
	}

	vec2 dnext = vnext.location -current.location;
	bool vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;

	for(int i = 1; i < (int)adj.size(); i++){
		if(adj[i] != vprev.ID){
			vec2 dirAdj = allNodes[adj[i]].location - current.location; // gets direction of adjacent vertex
			if(vcurrIsConvex){
				if(DotPerp(dCurr,dirAdj) > 0 && DotPerp(dnext,dirAdj) > 0){
					vnext = allNodes[adj[i]];
					dnext = dirAdj;
					vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;
				}
			}
			else{
				if(DotPerp(dCurr,dirAdj) > 0 || DotPerp(dnext,dirAdj) > 0){
					vnext = allNodes[adj[i]];
					dnext = dirAdj;
					vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;
				}

			}
		}
	}
	return vnext;
}

void RoadNetwork::removeAdjacencyLinks(int ID, map<int,vector<int>> * adjs){
	// Removes itself from other vertices links
	for(int i : (*adjs)[ID]){
		for(int j = 0; j < (int)(*adjs)[i].size(); j++){
			if(ID == (*adjs)[i].at(j)){				vector<int> near = (*adjs)[i];
			near.erase(near.begin() + j);
			(*adjs)[i] = near;
			}
		}
	}
	//Deletes it's own links
	adjs->erase(ID);
}
