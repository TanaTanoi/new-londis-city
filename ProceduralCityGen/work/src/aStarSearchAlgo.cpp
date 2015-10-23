/*
 * aStarSearchAlgo.cpp
 *
 *  Created on: 15/10/2015
 *      Author: bryerscame
 */

#include "aStarSearchAlgo.hpp"
#include "comp308.hpp"

using namespace std;
using namespace cycle;
using namespace comp308;

//AStarSearchAlgo::AStarSearchAlgo(std::vector<cycle::roadNode> allNodes,
//		std::map<int, std::vector<int> > adjacencyList) {
//	for (int i = 0; i < (int)allNodes.size(); ++i) {
//		roadNodeAStar node = roadNodeAStar(allNodes[i]);
//		m_allNodes[i] = node;
//	}
//}

vector<roadNode> AStarSearchAlgo::getPath(roadNode *target) {

	m_path.clear();

	// //cout << "Reversing..." << endl;

	for (int i = target->ID; i < (int) m_allNodes.size();
			i = m_allNodes[i].from->ID) {

		//cout << i << endl;
		m_path.push_back(m_allNodes[i]);

		// //cout << "Accessing " << m_allNodes[i].from->ID << endl;
		// roadNode node = m_allNodes[m_allNodes[i].from->ID];
		if (m_allNodes[i].from == NULL)
			break;

	}

	reverse(m_path.begin(), m_path.end());

	// //cout << "Reversing Done, size: " << m_path.size() << endl;
	return m_path;
}

void AStarSearchAlgo::aStarSearch(roadNode *start, roadNode *goal) {

	// //cout << "Finding path" << endl;
	// //cout << "Number of nodes: " << m_allNodes.size() << endl;
	m_start = *start;
	m_goal = *goal;

	m_visited.clear();

	for (int i = 0; i < (int) m_allNodes.size(); ++i) {
		m_allNodes[i].from = NULL;
		// m_allNodes[i].hasFrom = false;
		m_allNodes[i].isVisited_aStar = false;
	}

	priority_queue<fringeNode> fringe;
	fringeNode head = { start, NULL, 0, estimate(start, goal) };
	fringe.push(head);

	while (!fringe.empty()) {

		fringeNode dequeue = fringe.top();
		roadNode *nodePtr = dequeue.start;
		int nodeID = nodePtr->ID;

		// Can't call fringeNode dequeue = fringe.pop();
		fringe.pop();

		if (!m_allNodes[nodeID].isVisited_aStar) {
			m_allNodes[nodeID].isVisited_aStar = true;
			// m_allNodes[nodeID].hasFrom = true;
			m_allNodes[nodeID].from = dequeue.from;
			m_allNodes[nodeID].cost = dequeue.costToHere;
			m_visited.push_back(m_allNodes[nodeID]);

			// //cout << "StartID: " << start->ID << endl;
			// //cout << "GoalID: " << goal->ID << endl;
			// //cout << "NodeID: " << node->ID << endl;
			// If the goal has been found
			if (m_allNodes[nodeID].ID == goal->ID) {
				// //cout << "NodeID found!!" << endl;
				return;
			}

			// Repeat this for all neighbouring nodes
			for (int neighID : m_adjacencyList[nodeID]) {

				// roadNode neigh = m_allNodes[index];
				// XXX: May not be able to check on first node
//				if (neigh.from != null) {
//
//				}

				// //cout << "NeighbourID: " << m_allNodes[index].ID << endl;

				if (!m_allNodes[neighID].isVisited_aStar) {
					float costToNeigh = dequeue.costToHere
							+ distance(m_allNodes[nodeID].location,
									m_allNodes[neighID].location);
					double estTotal = costToNeigh
							+ estimate(&m_allNodes[neighID], goal);

					// Add a new fringeNode to the queue
					fringeNode add = { &m_allNodes[neighID],
							&m_allNodes[nodeID], costToNeigh, estTotal };
					fringe.push(add);
				}
			}
		}
	}
}

float AStarSearchAlgo::estimate(roadNode *current, roadNode *goal) {

	vec2 cur = current->location;
	vec2 gol = current->location;

	return abs((cur.x - gol.x) + (cur.y - gol.y));
}
