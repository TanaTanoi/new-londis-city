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
inline vec2 getClockwiseMost(vec2 current, vector<vec2> adj){
	vec2 dCurr = vec2(0,-1); //support line
	vec2 vnext = adj[0]; // sets up first adjacent vertex
	vec2 dnext = vnext -current;
	bool vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;

	for(int i = 1; i < (int)adj.size(); i++){
		vec2 dirAdj = adj[i] - current; // gets direction of adjacent vertex
		if(vcurrIsConvex){
			if(DotPerp(dCurr,dirAdj) < 0 || DotPerp(dnext,dirAdj) < 0){
				vnext = adj[i];
				dnext = dirAdj;
				vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;
			}
		}
		else{
			if(DotPerp(dCurr,dirAdj) < 0 && DotPerp(dnext,dirAdj) < 0){
				vnext = adj[i];
				dnext = dirAdj;
				vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;
			}

		}
	}
	return vnext;
}

// Gets the clockwiseMost adjacent vertex
// Returns the ID of it
inline vec2 getAntiClockwiseMost(roadNode vprev, roadNode current, vector<roadNode> adj){
	vec2 dCurr = current.location - vprev.location; //support line
	vec2 vnext = adj[0].location; // sets up first adjacent vertex

	if(adj[0].ID == vprev.ID){
		vnext = adj[1].location;
	}

	vec2 dnext = vnext -current.location;
	bool vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;

	for(int i = 1; i < (int)adj.size(); i++){
		if(adj[i].ID != vprev.ID){
			vec2 dirAdj = adj[i].location - current.location; // gets direction of adjacent vertex
			if(vcurrIsConvex){
				if(DotPerp(dCurr,dirAdj) > 0 && DotPerp(dnext,dirAdj) > 0){
					vnext = adj[i].location;
					dnext = dirAdj;
					vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;
				}
			}
			else{
				if(DotPerp(dCurr,dirAdj) > 0 || DotPerp(dnext,dirAdj) > 0){
					vnext = adj[i].location;
					dnext = dirAdj;
					vcurrIsConvex = (int)round(DotPerp(dnext,dCurr)) <=0;
				}

			}
		}
	}
	return vnext;
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

	vec2 result = getAntiClockwiseMost(c,start,adj);

	cout << "Result" << endl;
	cout<< result.x << " " << result.y << endl;

}

}
