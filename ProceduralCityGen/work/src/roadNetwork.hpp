/*
 * roadNetwork.hpp
 *
 *  Created on: 30/09/2015
 *      Author: craighhann
 */

#include <map>
#include <vector>
#include "section.hpp"

struct roadNode{
	comp308::vec2 location;
	int ID;
};

struct road{
	roadNode start;
	roadNode end;
	// Will have a quad to represent for drawing
};



class RoadNetwork{
private:
	std::map<int,std::vector<int>> adjacencyList; // stores road ID to other roadID's it connects to
	std::vector<roadNode> allNodes; // has all nodes
	std::vector<road> allRoads; // has all roads
	int nodeID = 0;

	// Boundary variables
	section outline; // gives the outline of the city
	float maxHeight;
	float minHeight;
	float farLeft;
	float farRight;

	int insideWorld(vec2);
	int insideWorld(road);
	road truncate(road);
	void addIntersection(int,int);
	void addNode(vec2);
	void addRoad(roadNode,roadNode);
	void calulateBoundary();

public:
	void renderRoads();
	void createRoads(section);

	// Cam's methods

	inline const std::map<int, std::vector<int> >& getAdjacencyList() const {
		return adjacencyList;
	}

	inline const std::vector<roadNode>& getAllNodes() const {
		return allNodes;
	}

	inline const std::vector<road>& getAllRoads() const {
		return allRoads;
	}
};
