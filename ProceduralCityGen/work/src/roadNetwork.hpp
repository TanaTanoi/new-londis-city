#pragma once
#ifndef SRC_ROADNETWORK_HPP_
#define SRC_ROADNETWORK_HPP_
/*
 * roadNetwork.hpp
 *
 *  Created on: 30/09/2015
 *      Author: craighhann
 */

#include <functional>
#include <queue>
#include <map>
#include <vector>
#include <set>
#include "utility.hpp"
#include "comp308.hpp"
#include "cycleUtil.hpp"
#include "Voronoi.hpp"


class RoadNetwork{
private:
	std::map<int,std::vector<int>> adjacencyList; // stores road ID to other roadID's it connects to
	std::vector<cycle::roadNode> allNodes; // has all nodes
	std::set<cycle::roadNode> canBranch;
	std::vector<cycle::road> allRoads; // has all roads
	std::vector<cycle::primitive> cycles;
	int nodeID = 0;
	int roadID = 0;

	// Boundary variables
	util::section outline; // gives the outline of the city
	float maxHeight;
	float minHeight;
	float farLeft;
	float farRight;

	// L system param
	int maxLength = 100;
	int minLength = 40;
	int minAngle = 0;
	int maxAngle = 360;
	int minRotateAngle = -15;
	int maxRotateAngle = 15;
	int snapDistance = 10;

	// Voronoi point generation
	std::vector<comp308::vec2> points;
	int gridSpace = 90;
	int radOut = 15;
	int circlePoints = 6;
	vor::Voronoi voro;

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
	void removeEdge(std::vector<cycle::road>*,  std::map<int,std::vector<int>> * ,int);

	void genBranchRoads(comp308::vec2);
	void branch(cycle::roadNode,std::vector<cycle::roadNode> * toAdd, std::vector<cycle::roadNode> * toRemove);
	comp308::vec2 direction(cycle::roadNode);
	void updateBranchList(cycle::roadNode, std::vector<cycle::roadNode> * toRemove);
	cycle::roadNode snapToIntersection(cycle::roadNode start, vec2 end);
	cycle::roadNode snapToExisting(vec2, cycle::road);
	void removeFilaments();

	void genGridPoints();
	void genRadialPoints();
	void createVoronoiRoads();

	void testIsolatedVertex();
	void testFilamentVertex();
	void testCycle();

public:
	RoadNetwork();
	void renderRoads();
	void createRoads(bool);
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

	inline const std::vector<cycle::primitive>& getCycles() const {
		return cycles;
	}
};

#endif /* SRC_ROADNETWORK_HPP_ */
