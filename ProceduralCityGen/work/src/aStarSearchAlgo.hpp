/*
 * aStarSearchAlgo.hpp
 *
 *  Created on: 15/10/2015
 *      Author: bryerscame
 */

#pragma once
#ifndef SRC_ASTARSEARCHALGO_HPP_
#define SRC_ASTARSEARCHALGO_HPP_

#include <functional>
#include <queue>
#include <map>
#include <vector>
#include "cycleUtil.hpp"

struct fringeNode {
	cycle::roadNode *start, *from;
	float costToHere, totalCostToGoal;

//	/* Copy constructor */
//	fringeNode(const fringeNode& other) :
//			start(other.start), from(other.from), costToHere(other.costToHere), totalCostToGoal(
//					other.totalCostToGoal) {}

	/* Comparator */
	bool operator<(const fringeNode& other) const {
		return costToHere > other.costToHere;
	}
};

/*
 * Used for this algorithm, Hannah was complaining about putting these values in the original
 * struct, don't know what she was complaining about but whatever.
 */
struct roadNodeAStar: cycle::roadNode {
	bool isVisited_aStar = false;
	roadNode *from;
	float cost;
//
	// Copies values from parent
	roadNodeAStar(const roadNode& parent) {

		// From parent
		location = parent.location;
		ID = parent.ID;
		visited = parent.visited;

		// AStar stuff
		isVisited_aStar = false;
		from = NULL;
		cost = 0;
	}
};

class AStarSearchAlgo {
private:

	int m_size = 0;
	cycle::roadNode m_start, m_goal;
	std::vector<cycle::roadNode> m_visited, m_path, m_allNodes;
	std::map<int, std::vector<int>> m_adjacencyList;

public:

	AStarSearchAlgo(std::vector<cycle::roadNode> allNodes,
			std::map<int, std::vector<int>> adjacencyList) :
			m_allNodes(allNodes), m_adjacencyList(adjacencyList) {
	}

//	AStarSearchAlgo(std::vector<cycle::roadNode> allNodes,
//			std::map<int, std::vector<int>> adjacencyList);

	std::vector<cycle::roadNode> getPath(cycle::roadNode*);

	void aStarSearch(cycle::roadNode*, cycle::roadNode*);

	float estimate(cycle::roadNode*, cycle::roadNode*);
};

#endif /* SRC_ASTARSEARCHALGO_HPP_ */
