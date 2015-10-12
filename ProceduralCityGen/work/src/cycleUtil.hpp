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
	comp308::vec2 location;
	int ID;
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

//FIXME: check this, not confident
inline float DotPerp(vec2 start, vec2 end){
	return start.x * end.y -  end.x * start.y;
}

// Gets the clockwiseMost adjacent vertex
// Returns the ID of it
inline int getClockWiseMost(vec2 current, vector<vec2> adj){
	vec2 dCurr = vec2(0,-1); //support line
	vec2 vnext = adj[0]; // sets up first adjacent vertex
	vec2 dnext = vnext -current;
	bool vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;

	for(int i = 1; i < (int)adj.size(); i++){
		vec2 dirAdj = adj[i] - current; // gets direction of adjacent vertex
		if(vcurrIsConvex){
			if(DotPerp(dCurr,dirAdj) < 0 || DotPerp(dnext,dirAdj)){

			}
		}
		else{


		}
	}
	return 0;
}

}
