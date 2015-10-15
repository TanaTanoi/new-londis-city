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

	bool operator<(const fringeNode& other) const {
		return costToHere > other.costToHere;
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
			m_allNodes(allNodes), m_adjacencyList(adjacencyList) {}

	std::vector<cycle::roadNode> getPath(cycle::roadNode*);

	void aStarSearch(cycle::roadNode*, cycle::roadNode*);

	float estimate(cycle::roadNode*, cycle::roadNode*);
};

#endif /* SRC_ASTARSEARCHALGO_HPP_ */
