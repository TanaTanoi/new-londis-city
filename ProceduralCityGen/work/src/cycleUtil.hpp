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

namespace cycle {

struct roadNode {


	vec2 location;
	int ID = -1;
	bool visited = false;

	// AStar
	bool isVisited_aStar = false;
	roadNode *from = NULL;
	float cost = -1;

	roadNode(vec2 loc, int id) {
		location = loc;
		ID = id;
	}
	roadNode() {

	}
};

struct road {
	cycle::roadNode start;
	cycle::roadNode end;
	int ID;
	bool isCycleEdge = false;

	road(cycle::roadNode s, cycle::roadNode e, int id) {
		start = s;
		end = e;
		ID = id;
	}
	// Will have a quad to represent for drawing
};

struct primitive {
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
inline vector<roadNode> sortPoints(vector<roadNode> points) {
	vector<roadNode> sorted;

	for (int i = 0; i < (int) points.size(); i++) { // for each point in points
		roadNode current = points[i];
		bool added = false;

		for (int j = 0; j < (int) sorted.size(); j++) {
			if (current.location.x < sorted[j].location.x) { // adds if smaller
				sorted.insert(sorted.begin() + j, current);
				added = true;
				break;
			} else if (current.location.x == sorted[j].location.x) { // check here for equating floats
				if (current.location.y < sorted[j].location.y) { //adds if equal in x value but smaller in y
					sorted.insert(sorted.begin() + j, current);
					added = true;
					break;
				}
			}
		}
		if (!added) {
			sorted.push_back(current);
		}

	}
	return sorted;

}
inline int findRoadIndex(vector<road> roads, int sID, int eID) {

	for (int i = 0; i < roads.size(); i++) {
		if (eID == roads[i].start.ID && sID == roads[i].end.ID) {
			return i;
		}
		if (sID == roads[i].start.ID && eID == roads[i].end.ID) {
			return i;
		}

	}
	return 0;
	/*
	 //	cout << "finding a road index" << endl;
	 int low = 0; int high = (int)roads.size()-1;
	 while(low <= high){
	 int mid = (low+high+1)/2;
	 //		cout<<mid<<" MID " << (int)roads.size() <<endl;
	 //		cout << "Looking for " << sID << " " <<eID << " Mid is  " <<  roads[mid].start.ID << " " << roads[mid].end.ID << endl;
	 //		cout << "Mid is " << mid <<endl;

	 if((sID == roads[mid].start.ID) && (eID == roads[mid].end.ID)){ // Found object
	 //cout << "equal to" << endl;
	 return mid;
	 }
	 else if((sID == roads[mid].start.ID && eID < roads[mid].end.ID) || (sID < roads[mid].start.ID)){ // Less than
	 //			cout<< "Less than" <<endl;
	 high = mid - 1;
	 }
	 else{ // greater than
	 //cout<< "Greater than" <<endl;
	 low = mid + 1;
	 }
	 }

	 //cout << "Did not find in loop" << endl;

	 if(sID == roads[low].start.ID && eID == roads[low].end.ID){ // Found object
	 return low;
	 }

	 return findRoadIndex(roads,eID,sID);*/
}

/*
 * Sorts roads using insertion sort first by starting node ID and then
 * by ending node ID
 */
inline vector<road> sortRoads(vector<road> roads) {
	vector<road> sorted;

	for (int i = 0; i < (int) roads.size(); i++) {
		road current = roads[i];
		bool added = false;
		for (int j = 0; j < (int) sorted.size(); j++) {
			if (current.start.ID < sorted[j].start.ID) {
				sorted.insert(sorted.begin() + j, current);
				added = true;
				break;
			} else if (current.start.ID == sorted[j].start.ID) {
				if (current.end.ID < sorted[j].end.ID) {
					sorted.insert(sorted.begin() + j, current);
					added = true;
					break;
				}
			}
		}
		if (!added) {
			sorted.push_back(current);
		}
	}
	cout << "Printing sorted :";
	for (road r : sorted) {
		cout << r.start.ID << "|" << r.end.ID << " ";
	}
	cout << endl;
	return sorted;

}

//FIXME: check this, not confident
inline float DotPerp(vec2 start, vec2 end) {
	return start.x * end.y - end.x * start.y;
}

inline void removeFromHeap(vector<roadNode> * heap, int ID) {
	for (int i = 0; i < (int) heap->size(); i++) {
		if (ID == (*heap)[i].ID) {
			heap->erase(heap->begin() + i);
			return;
		}
	}
}

/**
 * Checks if a vector of roadNodes contains a given node
 */
inline bool contains(vector<roadNode> visited, roadNode node) {
	int n = (int) visited.size();
	cout << "In contains " << n << endl;
	if (n == 0) {
		cout << "is zero" << endl;
		return false;
	}
	for (int i = 0; i < n; i++) {
		cout << "There is no error here" << endl;
		if (visited[i].ID == node.ID) {
			return true;
		}
	}
	return false;
}

inline void testAntiClockwise() {
	roadNode start = { vec2(100, 100), 0 };

	roadNode a = { vec2(300, 100), 1 };
	roadNode b = { vec2(250, 75), 2 };
	roadNode c = { vec2(50, 50), 3 };
	roadNode d = { vec2(150, 50), 4 };

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
