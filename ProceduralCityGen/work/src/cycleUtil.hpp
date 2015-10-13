/*
 * cycleUtil.hpp
 *
 *  Created on: 12/10/2015
 *      Author: craighhann
 */

#pragma once


#include "comp308.hpp"
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>
using namespace comp308;
using namespace std;

namespace cycle{

struct roadNode{
	vec2 location;
	int ID;
	bool visited = false;
};

struct road{
	cycle::roadNode start;
	cycle::roadNode end;
	int ID;
	bool isCycleEdge = false;
	// Will have a quad to represent for drawing
};


struct primitive{
	vector<roadNode> vertices;
	int type; // 0 isolated vertex, 1 filament, 2 cycle/filament
};

/**
 * Sorts a set of road nodes so
 * that the smallest x values (i.e. leftmost) come first.
 * In the event that two nodes have the same x value,
 * put the lowest y value first
 *
 * Currently uses basic insertion sort
 */
inline vector<roadNode> sortPoints(vector<roadNode> points){
	vector<roadNode> sorted;

	for(int i = 0; i < (int)points.size(); i++){ // for each point in points
		roadNode current = points[i];
		bool added = false;

		for(int j = 0; j <(int) sorted.size(); j++){
			if(current.location.x < sorted[j].location.x){ // adds if smaller
				sorted.insert(sorted.begin() + j, current);
				added = true;
				break;
			}
			else if(current.location.x == sorted[j].location.x){ // check here for equating floats
				if(current.location.y < sorted[j].location.y){ //adds if equal in x value but smaller in y
					sorted.insert(sorted.begin() + j, current);
					added = true;
					break;
				}
			}
		}
		if(!added){
			sorted.push_back(current);
		}

	}

	return sorted;

}

inline int findRoadIndex(vector<road> roads, int sID, int eID){
	int low = 0; int high = (int)roads.size(); int mid = (low+high)/2;
	while(low < high){
		if(sID == roads[mid].start.ID && eID == roads[mid].end.ID){ // Found object
			return mid;
		}
		else if((sID == roads[mid].start.ID && eID < roads[mid].end.ID) || (sID < roads[mid].start.ID)){ // Less than
			high = mid;
			mid = (low+high)/2;
		}
		else{ // greater than
			low = mid + 1;
			mid = (low+high)/2;
		}
	}

	if(sID == roads[low].start.ID && eID == roads[low].end.ID){ // Found object
		return low;
	}

	return findRoadIndex(roads,eID,sID);
}

/*
 * Sorts roads using insertion sort first by starting node ID and then
 * by ending node ID
 */
inline vector<road> sortRoads(vector<road> roads){
	vector<road> sorted;

	for(int i = 0; i < (int)roads.size(); i++){
		road current = roads[i];
		bool added = false;
		for(int j = 0; j < (int)sorted.size(); j++){
			if(current.start.ID < sorted[j].start.ID){
				sorted.insert(sorted.begin() + j, current);
				added = true;
				break;
			}
			else if(current.start.ID == sorted[j].start.ID){
				if(current.end.ID < sorted[j].end.ID){
					sorted.insert(sorted.begin() + j, current);
					added = true;
					break;
				}
			}
		}
		if(!added){
			sorted.push_back(current);
		}
	}
	return sorted;

}

//FIXME: check this, not confident
inline float DotPerp(vec2 start, vec2 end){
	return start.x * end.y -  end.x * start.y;
}

inline void removeFromHeap(vector<roadNode> * heap, int ID){
	for(int i = 0; i < (int)heap->size(); i++){
		if(ID == (*heap)[i].ID){
			heap->erase(heap->begin() + i);
			return;
		}
	}
}

/**
 * Checks if a vector of roadNodes contains a given node
 */
inline bool contains(vector<roadNode> visited, roadNode node){
	for(roadNode n : visited){
		if(n.ID == node.ID){
			return true;
		}
	}
	return false;
}



inline void testAntiClockwise(){
	roadNode start = {vec2(100,100),0};

	roadNode a = {vec2(300,100),1};
	roadNode b = {vec2(250,75),2};
	roadNode c = {vec2(50,50),3};
	roadNode d = {vec2(150,50),4};

	vector<roadNode> adj;
	adj.push_back(a);
	adj.push_back(b);
	adj.push_back(c);
	adj.push_back(d);
	//
	//	roadNode result = getAntiClockwiseMost(c,start,adj);
	//
	//	cout << "Result" << endl;
	//	cout<< result.x << " " << result.y << endl;

}

}
