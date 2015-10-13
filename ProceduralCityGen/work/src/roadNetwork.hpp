#pragma once
#ifndef SRC_ROADNETWORK_HPP_
#define SRC_ROADNETWORK_HPP_
/*
 * roadNetwork.hpp
 *
 *  Created on: 30/09/2015
 *      Author: craighhann
 */
#include <map>
#include <vector>
#include "utility.hpp"
#include "comp308.hpp"
#include "cycleUtil.hpp"




class RoadNetwork{
private:
	std::map<int,std::vector<int>> adjacencyList; // stores road ID to other roadID's it connects to
	std::vector<cycle::roadNode> allNodes; // has all nodes
	std::vector<cycle::road> allRoads; // has all roads
	int nodeID = 0;
	int roadID = 0;

	// Boundary variables
	util::section outline; // gives the outline of the city
	float maxHeight;
	float minHeight;
	float farLeft;
	float farRight;

	int insideWorld(comp308::vec2);
	int insideWorld(cycle::road);
	cycle::road truncate(cycle::road);
	void addIntersection(int,int);
	cycle::roadNode addNode(comp308::vec2);
	cycle::road addRoad(cycle::roadNode,cycle::roadNode);
	void updateAdjacencyList(cycle::road, cycle::roadNode);
	void calulateBoundary();
	void createNewYorkGrid(util::section s);
	void recDivideGrid(cycle::road,int,bool);
	std::vector<cycle::primitive> extractPrimitives();
	void extractIsolatedVertex(std::vector<cycle::primitive> * , std::vector<cycle::roadNode> *, std::map<int,std::vector<int>> *);
	void extractFilament(int, int, std::vector<cycle::primitive> * , std::vector<cycle::roadNode> *, std::map<int,std::vector<int>> * , std::vector<cycle::road> * );
	void extractPrimitive(std::vector<cycle::primitive> * , std::vector<cycle::roadNode> *, std::map<int,std::vector<int>> *, std::vector<cycle::road> *);
	cycle::roadNode getClockwiseMost(cycle::roadNode, std::vector<int>);
	cycle::roadNode getAntiClockwiseMost(cycle::roadNode,cycle::roadNode, std::vector<int>);
	void removeAdjacencyLinks(int, std::map<int,std::vector<int>> * );
	void setCycleEdge(std::vector<cycle::road> *, int, int);
	void findMinimumCycles();

public:
	RoadNetwork();
	void renderRoads();
	void createRoads(util::section);
	void recDivide(cycle::road);

	// Testing
		void testNetwork();

	// Cam's methods

	cycle::road findClosestRoads(vec3);

	inline const std::map<int, std::vector<int> >& getAdjacencyList() const {
		return adjacencyList;
	}

	inline const std::vector<cycle::roadNode>& getAllNodes() const {
		return allNodes;
	}

	inline const std::vector<cycle::road>& getAllRoads() const {
		return allRoads;
	}
};

#endif /* SRC_ROADNETWORK_HPP_ */
