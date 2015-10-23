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
#include "Voronoi.hpp"
#include "VEdge.hpp"

using namespace util;
using namespace cycle;
using namespace comp308;
using namespace std;
using namespace vor;

RoadNetwork::RoadNetwork(){}
/**
 * Checks whether a road will be fully inside a the world
 * which is represented by a section
 *
 * Returns 0 for inside, 1 for intersecting and 2 for completely outside
 */


void RoadNetwork::setNetMode(){
	netMode = true;
}

void RoadNetwork::genGridPoints() {
	for (int i = farLeft + gridSpace; i < farRight; i = i + gridSpace) {
		for (int j = minHeight + gridSpace; j < maxHeight; j = j + gridSpace) {
			vec2 point = vec2(i,j);
			if (insideWorld(point)) {
				points.push_back(point);
			}
		}
	}
}

void RoadNetwork::genBranchRoads(vec2 start) {
	roadNode rn = addNode(start); // adds to adj list
	canBranch.insert(rn);

	for (int i = 0; i < recurseSize; i++) {
		cout << "I " << i << endl;
		vector<roadNode> toAdd;
		vector<roadNode> toRemove;
		for (roadNode n: canBranch){
			branch(n, &toAdd, &toRemove);
		}
		cout <<"Branched" << endl;
		for(roadNode n : toAdd){
			canBranch.insert(n);
		}
		for(roadNode n : toRemove){
			canBranch.erase(n);
		}
		toAdd.clear();
		toRemove.clear();
	}

	if(netMode){
		removeFilaments();
	}


	// Forces number of cycles
	if((city_size == 0 && (int)cycles.size() < 2) || (city_size == 1 && (int)cycles.size() < 15) ||(city_size == 2 && (int)cycles.size() < 150)){
		reset();
		genBranchRoads(start);
	}

}

void RoadNetwork::removeFilaments(){
	int count = 0;
	int i= 0;

	while(i < (int)allRoads.size()){
		road r = allRoads[i];
		if((int)adjacencyList[r.start.ID].size() ==1 ||  (int)adjacencyList[r.end.ID].size() ==1){

			for(int i = 0; i < (int)adjacencyList[r.start.ID].size(); i++){
				if(adjacencyList[r.start.ID][i] == r.end.ID){
					cout<<"REMOVE EDGE: Remove start from end"<<endl;
					adjacencyList[r.start.ID].erase(adjacencyList[r.start.ID].begin() + i);
					if((int)adjacencyList[r.start.ID].size() == 0){
						allNodes[r.start.ID].ID = -1;
					}
					break;
				}
			}

			for(int i = 0; i < (int)adjacencyList[r.end.ID].size(); i++){
				if(adjacencyList[r.end.ID][i] == r.start.ID){
					cout<<"REMOVE EDGE: Remove end from start"<<endl;
					adjacencyList[r.end.ID].erase(adjacencyList[r.end.ID].begin() + i);
					if((int)adjacencyList[r.end.ID].size() == 0){
						allNodes[r.end.ID].ID = -1;
					}
					break;
				}
			}
			allRoads.erase(allRoads.begin() + i);
			count++;
		}
		else{
			i++;
		}
	}

	if(count > 0){
		removeFilaments();
	}
}


vec2 RoadNetwork::direction(roadNode n){
	cout << "Getting direction" << endl;

	// Gets angle of new road
	float random = ((float)rand() / (RAND_MAX));
	float angle = 0.0f;
	vec2 dir;
	// If first branch

	int count = (int)adjacencyList[n.ID].size();
	switch(count){

	case 0: {
		angle = minAngle + random*  (maxAngle - minAngle);
		float radAngle = radians(angle);
		//Creates new road
		dir = normalize(vec2((float)cos(radAngle), (float)sin(radAngle)));
		return dir;
	}
	case 1:{
		angle = minRotateAngle + random *  (maxRotateAngle - minRotateAngle);
		float radAngle = radians(angle);
		float cs = cos(radAngle);
		float sn = sin(radAngle);

		roadNode other = allNodes[adjacencyList[n.ID].at(0)];
		dir = normalize(n.location - other.location);

		return vec2(dir.x*cs - dir.y*sn, dir.x*sn + dir.y*cs);
	}
	case 2: {
		angle = minRotateAngle + random*  (maxRotateAngle - minRotateAngle);
		float radAngle = radians(angle);
		float cs = cos(radAngle);
		float sn = sin(radAngle);

		roadNode other = allNodes[adjacencyList[n.ID][0]];

		cout << "Length " <<length(other.location - n.location) << endl;
		dir = findPerp(other.location - n.location);

		return vec2(dir.x*cs - dir.y*sn, dir.x*sn + dir.y*cs);
	}
	case 3:{
		angle = minRotateAngle + random*  (maxRotateAngle - minRotateAngle);
		float radAngle = radians(angle);
		float cs = cos(radAngle);
		float sn = sin(radAngle);

		roadNode other = allNodes[adjacencyList[n.ID][0]];
		cout << "Length " <<length(other.location - n.location) << endl;
		//		if(util::isNAN(length(n.location-other.location))){
		//			return vec2(0,1);
		//		}
		cout << "Other " << other.location.x << "  " << other.location.y << "  N  " << n.location.x << " " << n.location.y <<endl;
		dir = findPerp(other.location - n.location);

		return -1*vec2(dir.x*cs - dir.y*sn, dir.x*sn + dir.y*cs);
	}

	}

	return vec2(0,1);

}

void RoadNetwork::reset(){
	allNodes.clear();
	adjacencyList.clear();
	allRoads.clear();
	cycles.clear();
	canBranch.clear();
	nodeID = 0;
	roadID = 0;
}


/* REQUIRES: If inter is a point that intersects with the road r:
 * Returns the start or end points if they are close enough, else returns -1
 *   */
roadNode RoadNetwork::snapToExisting(vec2 inter, road r){
	float startDist = distance(inter,r.start.location);
	if(startDist < snapDistance){
		return r.start;
	}
	float endDist = distance(inter,r.end.location);
	if(endDist < snapDistance){
		return r.end;
	}
	return {vec2(),-1};//don't change if here
}
/* Returns a node that should be the end of the road, creating a new one
 * in the case of : not intersecting; or intersecting an existing road,
 * else attaches to existing road node. */
roadNode RoadNetwork::snapToIntersection(roadNode start, vec2 end){
	cout << "snap to intersection " << endl;

	vec2 snapEnd = end + normalize(end-start.location)*snapDistance;

	vector<road> intersect;
	for(road r: allRoads){
		if(intersects(start.location, snapEnd, r.start.location, r.end.location)){
			if(r.start.ID != start.ID && r.end.ID != start.ID){
				intersect.push_back(r);
			}
		}
	}

	if((int)intersect.size() == 0){
		roadNode oldEnd = addNode(end);
		return oldEnd;//make a new node if nothing intersects
	}

	sortByIntersection(&intersect, start.location, end);//sort by distance

	vec2 firstInter = getIntersection(start.location ,snapEnd, intersect[0].start.location, intersect[0].end.location);
	roadNode newEnd = snapToExisting(firstInter, intersect[0]);//if it should snap to existing point

	if(newEnd.ID == -1){				//if it doesn't snap to existing, make a new road node that intersects line
		newEnd = addNode(firstInter);
		updateAdjacencyList(intersect[0],newEnd);
	}else if((int)adjacencyList[newEnd.ID].size()>3){//if it does, and it already has 4 nodes, then don't do anything at all.
		return {vec2(),-1};
	}

	return newEnd;
}

void RoadNetwork::branch(roadNode n, vector<roadNode> * toAdd, vector<roadNode> * toRemove) {
	cout << "Branch" << endl;
	// Gets length of new road
	float random = ((float)rand() / (RAND_MAX));
	float length = minLength + random*(maxLength - minLength);
	random = ((float)rand() / (RAND_MAX));

	vec2 dir = direction(n);
	if(isNAN(dir.x) || isNAN(dir.y)){
		return;
	}

	vec2 road = dir*length;
	vec2 endPoint = n.location + road;

	// Now checks for intersection and snap
	roadNode end = snapToIntersection(n,endPoint);

	if(end.ID == -1){return;}

	// Adds new road

	if (distance(n.location, end.location) > minLength ) { // forces roads to be of at least minimum length
		if ((int)adjacencyList[end.ID].size() < 4) {
			toAdd->push_back(end);
			addRoad(n, end);
		}
	}

	updateBranchList(n, toRemove);
}

void RoadNetwork::updateBranchList(roadNode n, vector<roadNode> * toRemove) {
	cout << "Updating branch list " << endl;
	float random = ((float)rand() / (RAND_MAX));
	int noAdj = (int)adjacencyList[n.ID].size();

	if (noAdj == 1) {
		if (random < 0.1) { toRemove->push_back(n); } // 10% chance of being removed
	}
	else if (noAdj == 2) {
		if (random < 0.35) { toRemove->push_back(n); } // 35% chance of being removed
	}
	else if (noAdj == 3) {
		if (random < 0.7) { toRemove->push_back(n); } // 70% chance of being removed
	}
	else {
		toRemove->push_back(n); //remove from canBranch
	}
	cout << "Updated branch list" << endl;
}

void RoadNetwork::genRadialPoints() {
	int maxHalfLength = max(farRight-farLeft, maxHeight - minHeight)/2 ;
	vec2 centrePoint = vec2(farLeft + (farRight-farLeft)/2, minHeight + (maxHeight-minHeight)/2);
	cout << "Circle Points " << circlePoints << endl;
	points.push_back(centrePoint);
	for (int i = radOut; i < maxHalfLength; i = i + radOut) {
		cout << "Max half length: " << maxHalfLength << " Rad out : " << i << endl;
		for (int j = 0; j < circlePoints; j++) {
			cout << "J is " << j << endl;
			vec2 startCirc;
			if (j == 0) {
				startCirc = vec2(centrePoint.x, centrePoint.y + i);
				if (insideWorld(startCirc)) {
					cout << "Start is inside world"<< endl;
					points.push_back(startCirc);
				}
			}
			else {
				vec2 p = rotate(centrePoint,startCirc, ((float)j/(float)circlePoints)*360);
				cout << "new vector " << j << " is " << p.x << "  " << p.y << endl;
				if (insideWorld(p)) {
					cout << "Non start "  << j << " is inside world" << endl;
					points.push_back(p);
				}
			}
		}
	}
}


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
	//	cout<<" ADD NODE " << allNodes[allNodes.size()-1].location.x  << " " << allNodes[allNodes.size()-1].location.y<< endl;
	adjacencyList[r.ID] =  vector<int>();
	return r;
}

/**
 * Adds new road to list of all roads
 */
road RoadNetwork::addRoad(roadNode start,roadNode end){
	road r = {start,end,roadID++};
	cout<<" Adding road "<<endl;

	for(int i = 0; i < (int)adjacencyList[start.ID].size(); i++){
		if(adjacencyList[start.ID][i] == end.ID){
			return {};
		}
	}

	allRoads.push_back(r);
	adjacencyList[start.ID].push_back(end.ID);
	adjacencyList[end.ID].push_back(start.ID);
	return r;
}

/**
 * Note, this method assumes you know that the road is split by
 * the given node. Also replaces that road in allRoads with the
 * resulting two roads
 */
void RoadNetwork::updateAdjacencyList(road r, roadNode n){
	// remove r from allRoads
	for(int i = 0; i < allRoads.size(); i++){
		if(r.ID == allRoads[i].ID){
			allRoads.erase(allRoads.begin() + i);
			break;
		}
	}

	// remove end from start and vice versa in adjacency list
	for(int i = 0; i < (int)adjacencyList[r.start.ID].size(); i++){
		if(adjacencyList[r.start.ID][i] == r.end.ID){
			adjacencyList[r.start.ID].erase(adjacencyList[r.start.ID].begin() + i);
			break;
		}
	}

	for(int i = 0; i < (int)adjacencyList[r.end.ID].size(); i++){
		if(adjacencyList[r.end.ID][i] == r.start.ID){
			adjacencyList[r.end.ID].erase(adjacencyList[r.end.ID].begin() + i);
			break;
		}
	}

	// create two new roads start-n and n-end
	addRoad(r.start,n);
	addRoad(n,r.end);
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
	bool aIntersect = false;
	bool bIntersect = false;

	roadNode r1;
	roadNode r2;

	// need to check here if a and b intercept
	// now checks if the end point intersects any existing roads
	int n = allRoads.size();
	for(int i = 0; i < n; i++){
		line l =  {allRoads[i].start.location, allRoads[i].end.location,0};
		if(!aIntersect && isPointOnLine(a,l)){
			cout << "Point is on line " << "" << endl;
			cout<<"ALL ROADS END "<< allRoads[i].end.location.x<<endl;
			// check existing
			//			if((int)round(a.x == allRoads[i].start.location.x) && a.y == allRoads[i].start.location.y){
			if(floatEqual(a.x,allRoads[i].start.location.x)&&floatEqual(a.y,allRoads[i].start.location.y)){
				//cout << "Cuaght" << endl;
				r1 = allRoads[i].start;
				aIntersect = true;
			}
			//			else if(a.x == allRoads[i].end.location.x && a.y == allRoads[i].end.location.y){
			else if(floatEqual(a.x,allRoads[i].end.location.x)&&floatEqual(a.y,allRoads[i].end.location.y)){
				r1 = allRoads[i].end;
				aIntersect = true;
			}else{
				n--;
				r1 = addNode(a);
				cout << "Old road split "  << r1.ID << "Splitting " << allRoads[i].start.ID << " " << allRoads[i].end.ID  <<endl;
				updateAdjacencyList(allRoads[i],r1);
				aIntersect = true;
			}
		}
		else if(!bIntersect && isPointOnLine(b,l)){
			// check existing
			if(floatEqual(b.x,allRoads[i].start.location.x)&&floatEqual(b.y,allRoads[i].start.location.y)){
				r2 = allRoads[i].start;
				bIntersect = true;
			}
			else if(floatEqual(b.x,allRoads[i].end.location.x)&&floatEqual(b.y,allRoads[i].end.location.y)){
				r2 = allRoads[i].end;
				bIntersect = true;
			}else{
				n--;
				r2 = addNode(b);
				cout << "Old road split "  << r2.ID << "Splitting " << allRoads[i].start.ID << " " << allRoads[i].end.ID  <<endl;
				updateAdjacencyList(allRoads[i],r2);

				bIntersect = true;
			}
		}
	}

	if(!aIntersect){
		r1 = addNode(a);
		cout << "Node created "  << r1.ID << endl;
	}
	if(!bIntersect){
		r2 = addNode(b);
		cout << "Node created "  << r2.ID << endl;
	}

	road left = addRoad(rn,r1);
	road right = addRoad(rn,r2);

	cout<< "First line added " << endl;
	cout<< "Adjacents size" << adjacencyList.size() << " Nodes size " << allNodes.size() << "  Egdes size " << allRoads.size() <<endl;

	if(level < recurseSize){
		cout<< "First div on level " << level << endl;
		recDivideGrid(left, level+1,!halfLength);
		cout<< "Second div on level " << level << endl;
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
	cout<<"END " << end.x << " " << end.y<<endl;
	// Adds first road
	addNode(start);
	addNode(end);
	addRoad(allNodes[0],allNodes[1]);
	cout << "Node 1 " << allNodes[0].location.x << " " << allNodes[0].location.y << " node 2 " << allNodes[1].location.x << " "<< allNodes[1].location.y << endl;

	cout<< "First line added " << endl;
	cout<< "Adjacents size" << adjacencyList.size() << " Nodes size " << allNodes.size() << "  Egdes size " << allRoads.size() <<endl;

	// Now recursively subdivide
	recDivideGrid(allRoads[0],0,true);

}

void RoadNetwork::createRoads(bool branchOn, int size){

	city_size = size;

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
	calulateBoundary();
	if(!branchOn){

		switch(size){
		case 0:
			recurseSize = 2;
			break;
		case 1:
			recurseSize = 3;
			break;
		case 2:
			recurseSize = 4;
			break;
		default:
			recurseSize = 3;
			break;
		}

		createNewYorkGrid(s);

	}
	else{

		switch(size){
		case 0:
			recurseSize = rand()%3 + 8; // 8 - 10
			break;
		case 1:
			recurseSize = rand()%5 + 12; // 12 - 16
			break;
		case 2:
			recurseSize = rand()%5 + 18; // 18 -22
			break;
		default:
			recurseSize = rand()%5 + 12;
			break;
		}

		genBranchRoads(vec2(0,0));
	}
	//genRadialPoints();
	//createVoronoiRoads();
	//testIsolatedVertex();
	//testFilamentVertex();
	//testCycle();

	//	cout << endl;
	cout << "Adjacency List" << endl;
	for(int i = 0; i < adjacencyList.size(); i++){
		cout << " key " << i << ": ";
		for(int j = 0; j < adjacencyList[i].size(); j++){
			cout << " " << adjacencyList[i][j];
		}
		cout << endl;
	}
	//
	//	cout << endl;
	//	cout << "Road List" << endl;
	//	for(int i = 0; i < allRoads.size(); i++){
	//		cout << "Start: " << allRoads[i].start.ID << " End: " <<  allRoads[i].end.ID << endl;
	//	}

	findMinimumCycles();
	cout << "Done !" << endl;
	// Now take in population density
	// Now generate highways
	// Now generate minor roads
}

void RoadNetwork::testNetwork(){

	createRoads(false,1);
}

void RoadNetwork::networkModeGen(int type, int size, bool cycle){
	drawCycles = cycle;
	createRoads(type,size);
}

void RoadNetwork::createVoronoiRoads(){
	//genGridPoints();
	vec2 a = vec2(200,300);
	vec2 b = vec2(300,200);
	vec2 c = vec2(200,200);
	vec2 d = vec2(350,150);
	vec2 e = vec2(300,100);
	points.push_back(a);
	points.push_back(b);
	points.push_back(d);
	points.push_back(c);
	points.push_back(e);
	points.push_back(e-a);
	points.push_back(e-c);

	cout<< "Points generated" << endl;
	voro = Voronoi();
	vor::edges edgeList = voro.GetEdges(points, 600, 600);

	cout << "edgeList size " << edgeList.size() << endl;
	for(VEdge e : edgeList){
		allRoads.push_back(e.edge);
		cout <<" road " << e.edge.start.location.x <<" , " << e.edge.start.location.y << "    " << e.edge.end.location.x << " , " << e.edge.end.location.y << endl;
	}
}

void RoadNetwork::renderRoads(){
	//cout <<"rendering roads" << endl;

	//glColor3f(0.5,0.0,0.5);
	//glBegin(GL_POINTS);
	//for(vec2 dot : points){
	//	glVertex2f(dot.x,dot.y);
	//}
	//glEnd();

	//glColor3f(1.0f,1.0f,0.0f); // yellow world bounds
	//glBegin(GL_LINES);
	//for(line l : outline.lines){
	//	glVertex2f(l.start.x, l.start.y);
	//	glVertex2f(l.end.x, l.end.y);
	//}
	//glEnd();

	//glColor3f(1.0f,0.0f,0.0f); // red roads
	//glLineWidth(2.0f);


	//	glColor3f(0.0,1.0,0.0);
	if(drawCycles){
		for(primitive p :cycles){
			float red = (float)rand()/(RAND_MAX);
			srand(rand());
			float gr = (float)rand()/RAND_MAX;
			srand(rand());
			float br = (float)rand()/RAND_MAX;
			glColor3f(red,gr,br);
			glBegin(GL_POLYGON);

			for(roadNode v : p.vertices){
				glVertex2f(v.location.x, v.location.y);
			}
			glEnd();
		}
	}






	glBegin(GL_LINES);
	int j = 0;
	//	//	for(road r : allRoads){
	//	//	for(int i = allRoads.size()-1;i>=0;i--){
	for (int i = 0; i < allRoads.size(); i++) {
		road r = allRoads[i];
		float chance = r.ID / (float)allRoads.size(); j++;
		srand(r.ID);
		float red = (float)rand() / (RAND_MAX);
		srand(rand());
		float gr = (float)rand() / RAND_MAX;
		srand(rand());
		float br = (float)rand() / RAND_MAX;
		glColor3f(red, gr, br);
		glVertex2f(r.start.location.x, r.start.location.y);
		glVertex2f(r.end.location.x, r.end.location.y);
	}
	glEnd();



	glColor3f(0.0,0.0,1.0); // blue intersections
	glPointSize(10);

	glBegin(GL_POINTS);
	//for(roadNode n : allNodes){
	for(int i = allNodes.size()-1;i>=0;i--){
		if(allNodes[i].ID > 0){
			roadNode n = allNodes[i];
			//if(n.ID > 10 &&  n.ID < 13){
			glColor3f(n.ID/(float)allNodes.size(),n.ID/(float)allNodes.size(),n.ID/(float)allNodes.size());

			glVertex2f(n.location.x, n.location.y);
		}
	}
	glEnd();



}

void RoadNetwork::findMinimumCycles(){
	cout << "--------------------" << endl;
	cout << "Finding min cycles" << endl;
	vector<primitive> prims = extractPrimitives();
	for(primitive p : prims){
		if(p.type == 2){ // if cycle
			cycles.push_back(p);
		}
	}
	cout<<"Primitives " << prims.size()<<endl;
	cout<<"Cycles " << cycles.size()<<endl;
}


vector<primitive> RoadNetwork::extractPrimitives(){

	cout << "Extracting primitives" << endl;

	vector<primitive> primitives;
	vector<roadNode> heap = sortPoints(allNodes);

	for(int i = 0; i < heap.size(); i++){
		cout<< heap[i].ID <<endl;
	}

	map<int,vector<int>> adjacencies = adjacencyList;
	vector<road> roads = sortRoads(allRoads);

	cout << "All nodes " << (int)allNodes.size() << endl;
	cout << "Heap size" << (int)heap.size() << endl;
	while((int)heap.size()!=0){
		if((int)roads.size()==0){
			cout<<"BREAKING BECAUSE NO MORE ROADS"<<endl;
			cout << "HEAP size" << (int)heap.size() << endl;
			cout<<"TOP OF HEAP "<<heap[0].ID<<endl;
			cout << "Adjacency List" << endl;
			for(int i = 0; i < adjacencyList.size(); i++){
				cout << " key " << i << ": ";
				for(int j = 0; j < adjacencyList[i].size(); j++){
					cout << " " << adjacencyList[i][j];
				}
				cout << endl;
			}
			break;
		}

		roadNode vertex = heap[0];
		vector<int> adjs = adjacencies[vertex.ID];
		int noAdj = (int)adjs.size();

		if(noAdj == 0){
			cout << "Extracting isolated vertex" << endl;
			// extract isolated vertex
			cout<<"IV Heap Before"<<(int)heap.size()<<endl;
			extractIsolatedVertex(&primitives, &heap, &adjacencies);
			cout<<"IV Heap After"<<(int)heap.size()<<endl;
		}else if ( noAdj == 1){
			cout << "Extracting filament vertex" << endl;
			// extract filament
			cout<<"EF Heap Before"<<(int)heap.size()<<endl;
			extractFilament(vertex.ID, adjacencies[vertex.ID][0] ,&primitives, &heap, &adjacencies, &roads);
			cout<<"EF Heap After"<<(int)heap.size()<<endl;
		}else{
			// extract filament or cycle
			cout << "Extracting cycle" << endl;
			extractPrimitive(&primitives, &heap, &adjacencies, &roads);
		}
		cout<< " HEAP SIZE "<<(int)heap.size()<<endl;

	}

	return primitives;
}

void RoadNetwork::extractIsolatedVertex(vector<primitive> * primitives, vector<roadNode> * heap, map<int,vector<int>> * adjs){
	vector<roadNode> vertices;
	vertices.push_back(heap->front()); // removes roadNode from heap
	heap->erase(heap->begin());
	cout << "Heap size " << heap->size() << endl;
	//removeFromHeap(heap,startID);// remove from heap
	primitive p = {vertices,0}; // adds vertex to primitive and sets it as isolated vertex
	adjs->erase(vertices[0].ID); // removes from adjacency list
	primitives->push_back(p);
}

// Note: visiting a vertex should remove it from adjacency list

void RoadNetwork::extractFilament(int startID, int endID, vector<primitive> * primitives,
		vector<roadNode> * heap, map<int,vector<int>> * adjs, vector<road> * roads){
	cout << "Extracting a filament road size: "<<(int)(*roads).size() << endl;
	if((*roads)[(findRoadIndex((*roads),startID,endID))].isCycleEdge){// the edge between startID (heap[0]) and an adjacent vertex of ID endID
		if((int)(*adjs)[startID].size() >= 3){

			int toRemove = findRoadIndex((*roads),startID,endID); //remove edge v0 to v1
			roads->erase(roads->begin() + toRemove);

			startID = endID;
			if((int)(*adjs)[startID].size() == 1){
				endID = (*adjs)[startID][0];
			}
		}

		while((int)(*adjs)[startID].size() == 1){
			cout<<"start id is " <<startID<<endl;
			endID = (*adjs)[startID][0];
			if((*roads)[findRoadIndex((*roads),startID,endID)].isCycleEdge){//  if cycle edge
				removeFromHeap(heap,startID);							//remove from heap
				int toRemove = findRoadIndex((*roads),startID,endID); 	//remove edge v0 to v1
				roads->erase(roads->begin() + toRemove);				//remove edge
				removeAdjacencyLinks(startID,adjs); 					//remove vertex
				startID = endID;
				cout<<"SIZE "<<(int)(*adjs)[startID].size()<<endl;
			}	else{
				cout<<"Breaking because his road isn't a cycle edge"<<endl;
				break;
			}
		}
		cout<<"We then ask this ol if what up"<<endl;
		if((int)(*adjs)[startID].size() == 0){
			removeFromHeap(heap,startID);		//remove from heap
			removeAdjacencyLinks(startID,adjs); //remove vertex
		}
	}else{
		cout << "Not a cycle edge " << endl; // Here safe
		primitive p;
		p.type = 1; // sets to filament

		if((int)(*adjs)[startID].size() >= 3){
			cout << "Has 3+ edges out " << endl;

			//			for(int i = 0; i < (*adjs)[startID].size(); i++){
			//				cout << (*adjs)[startID][i]<< endl;
			//			}
			//			cout<< endl;

			p.vertices.push_back(allNodes[startID]);
			cout << "Removing " <<startID << " " <<endID<<endl;
			int toRemove = findRoadIndex((*roads),startID,endID); //remove edge v0 to v1
			cout << "ROAD SIZE AT F IS "<< (int)(*roads).size()<< endl;
			roads->erase(roads->begin() + toRemove);//remove edge
			cout << "ROAD AFTR AT F IS "<< (int)(*roads).size()<< endl;
			startID = endID;
			if((int)(*adjs)[startID].size() == 1){
				endID = (*adjs)[startID][0];
			}
		}

		while((int)(*adjs)[startID].size() == 1){
			cout << "Has only 1 adj in while loop" << endl;
			p.vertices.push_back(allNodes[startID]);
			endID = (*adjs)[startID][0];
			removeFromHeap(heap,startID);//remove from heap (all verts)

			int toRemove = findRoadIndex((*roads),startID,endID); //remove edge v0 to v1
			cout<< " Output "<<toRemove<<endl;
			roads->erase(roads->begin() + toRemove);//remove edge (find road and remove road)
			removeAdjacencyLinks(startID,adjs); //remove vertex (remove all links to this and itself from the adj list)
			startID = endID;
		}
		cout << "Now doesn't have 1 adj" << endl;
		p.vertices.push_back(allNodes[startID]);
		if((int)(*adjs)[startID].size() == 0){
			removeFromHeap(heap,startID);//remove from heap
			removeAdjacencyLinks(startID,adjs); //remove vertex
		}
		primitives->push_back(p);
	}
	cout << "Got filament" <<endl;
}
//when I erase a road I just want to remove the two links
void RoadNetwork::removeEdge(vector<road> * roads, map<int,vector<int>> * adjs, int toRemove){
	//we probably want to get the actual start and end points ut of the this road to remove the links from list
	// remove end from start and vice versa in adjacency list
	road r = (*roads)[toRemove];
	cout<<"REMOVE EDGE: Road to remove "<<toRemove<<endl;
	for(int i = 0; i < (int)(*adjs)[r.start.ID].size(); i++){
		if((*adjs)[r.start.ID][i] == r.end.ID){
			cout<<"REMOVE EDGE: Remove start from end"<<endl;
			(*adjs)[r.start.ID].erase((*adjs)[r.start.ID].begin() + i);
			break;
		}
	}

	for(int i = 0; i < (int)(*adjs)[r.end.ID].size(); i++){
		if((*adjs)[r.end.ID][i] == r.start.ID){
			cout<<"REMOVE EDGE: Remove end from start"<<endl;
			(*adjs)[r.end.ID].erase((*adjs)[r.end.ID].begin() + i);
			break;
		}
	}
	roads->erase(roads->begin() + toRemove);//remove edge
}


void RoadNetwork::extractPrimitive(vector<primitive> * primitives, vector<roadNode> * heap, map<int,vector<int>> * adjs, vector<road> * roads){
	vector<roadNode> visited = vector<roadNode>();
	vector<roadNode> sequence;
	cout<<"Afdfdfdfed " << (int)roads->size()<<endl;
	roadNode start = (*heap)[0]; // insert v0
	roadNode v1 = getClockwiseMost(start, (*adjs)[start.ID]);	// v1 := most clockwise of v0

	sequence.push_back(start);									//add v0

	roadNode vprev = start;			//vp = v0

	roadNode vcurr = v1;			//vc = v1

	cout << "Start: " << start.ID;
	cout << " v1: " << v1.ID;
	cout<<endl;

	while(vcurr.ID!=-1&&vcurr.ID != start.ID && !contains(visited,vcurr)){//while vc != null && vc != v0 and vc != visited
		cout << "In cycle while loop" <<endl;

		sequence.push_back(vcurr);	//TODO why do we do this before break?
		visited.push_back(vcurr);	//We break after because its an early break condition

		//		if((int)(*adjs)[vcurr.ID].size() == 1){ //if vc(the next vc) == null, then end
		//			cout << "Break " << endl;
		//			break;
		//		}

		roadNode vnext = getAntiClockwiseMost(vprev,vcurr,(*adjs)[vcurr.ID]);//vnext = anti clockwise between vp and vc
		vprev = vcurr;				//vp := vc
		vcurr = vnext;				//vc := vn
	}
	///curr is now equal to start, if cycle.
	cout << "Passed while loop" << endl;

	if(vcurr.ID==-1){	//if vc approximates to null
		//Filament found, may not actaully start at vprev, but it is a part of it
		cout << "Filament found" << endl;
		extractFilament(vprev.ID, (*adjs)[vprev.ID][0], primitives, heap, adjs, roads);//Use vc because vc would be null if possible
	}else if(vcurr.ID == start.ID){
		cout << "Minimal cycle found" << endl;
		// Minimal cycle found
		primitive p = {sequence, 2};
		primitives->push_back(p);

		// for each edge in the cycle
		int n = (int)sequence.size();

		for(int i = 0; i < n;i++){
			setCycleEdge(roads,i,(i+1)%n);
		}


		cout<<"Input " << start.ID << " " << v1.ID;
		int toRemove = findRoadIndex((*roads),start.ID,v1.ID); //remove edge v0 to v1
		cout<<" output " << toRemove<<endl;
		cout<< "Start adjs. Removing  "<<v1.ID <<" "<< (int)(*adjs)[start.ID].size()<<endl;
		removeEdge(roads, adjs, toRemove);
		cout<< "Start adjs end. Start ID "<<start.ID<<" "<< (int)(*adjs)[start.ID].size()<<endl;
		cout<<"And edge has actually been removed " << roads->size()<<endl;
		if((int)(*adjs)[start.ID].size() == 1){
			cout<<"Exracted filament Start.ID : ADJ: "<<(*adjs)[start.ID][0]<<endl;
			extractFilament(start.ID, (*adjs)[start.ID][0], primitives, heap, adjs, roads);

		}

		if((int)(*adjs)[v1.ID].size() == 1){
			cout<<"Exracted filament v1.ID"<<endl;
			extractFilament(v1.ID, (*adjs)[v1.ID][0], primitives, heap, adjs, roads);
		}
	}else{
		while((int)(*adjs)[start.ID].size() == 2){
			//cout << "Stuck in else while loop" << endl;
			if((*adjs)[start.ID].at(0) != v1.ID){
				v1 = start;
				start = allNodes[(*adjs)[start.ID][0]];
			}else{
				v1 = start;
				start = allNodes[(*adjs)[start.ID][1]];
			}
		}
		cout<<"Extracting a thingy but not a cycle edge"<<endl;
		extractFilament(start.ID, v1.ID, primitives, heap, adjs, roads);
	}

}

void RoadNetwork::setCycleEdge(vector<road> * roads, int startID, int endID){
	int toSet = findRoadIndex((*roads),startID, endID);
	cout << "found road index" << endl;
	(*roads)[toSet].isCycleEdge = true;
}

road RoadNetwork::findClosestRoads(vec3 position) {
	road min = allRoads[0];
	vec2 pos = vec2(position.x,position.z);
	for(int i = 1; i < allRoads.size();i++){
		if(abs(length(min.end.location-pos))>abs(length(allRoads[i].end.location-pos))){
			min = allRoads[i];
		}
	}
	return min;
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
	if((int)adj.size()==1){return {vec2(0,0),-1};}
	vec2 dCurr = current.location - vprev.location; //support line
	roadNode vnext = allNodes[adj[0]]; // sets up first adjacent vertex

	if(adj[0] == vprev.ID){
		if((int)adj.size() == 1){
			return vprev;
		}
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
/* This kills a vertex   */
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

void RoadNetwork::testIsolatedVertex(){
	addNode(vec2(200,200));
}

void RoadNetwork::testFilamentVertex(){
	roadNode r = addNode(vec2(200,200));
	roadNode s = addNode(vec2(400,200));
	addRoad(r,s);

}

void RoadNetwork::testCycle(){
	vec2 r = (vec2(100,100));
	vec2 s = (vec2(100,400));
	vec2 t = (vec2(400,400));
	vec2 u = (vec2(400,100));

	roadNode a = addNode(r);
	roadNode b = addNode(s);
	roadNode c = addNode(t);
	roadNode d = addNode(u);

	addRoad(a,b);
	addRoad(b,c);
	//	addRoad(a,c);
	addRoad(c,d);
	addRoad(d,a);


}
