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

struct road{
	cycle::roadNode start;
	cycle::roadNode end;
	int ID;
	// Will have a quad to represent for drawing
};



class RoadNetwork{
private:
	std::map<int,std::vector<int>> adjacencyList; // stores road ID to other roadID's it connects to
	std::vector<cycle::roadNode> allNodes; // has all nodes
	std::vector<road> allRoads; // has all roads
	int nodeID = 0;
	int roadID = 0;

	// Boundary variables
	util::section outline; // gives the outline of the city
	float maxHeight;
	float minHeight;
	float farLeft;
	float farRight;

	int insideWorld(comp308::vec2);
	int insideWorld(road);
	road truncate(road);
	void addIntersection(int,int);
	cycle::roadNode addNode(comp308::vec2);
	road addRoad(cycle::roadNode,cycle::roadNode);
	void updateAdjacencyList(road, cycle::roadNode);
	void calulateBoundary();
	void createNewYorkGrid(util::section s);
	void recDivideGrid(road,int,bool);



public:
	RoadNetwork();
	void renderRoads();
	void createRoads(util::section);
	void recDivide(road);

	// Testing
		void testNetwork();

	// Cam's methods

	road findClosestRoads(vec3);

	inline const std::map<int, std::vector<int> >& getAdjacencyList() const {
		return adjacencyList;
	}

	inline const std::vector<cycle::roadNode>& getAllNodes() const {
		return allNodes;
	}

	inline const std::vector<road>& getAllRoads() const {
		return allRoads;
	}
};
