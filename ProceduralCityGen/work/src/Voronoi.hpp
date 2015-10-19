/*
 * Voronoi.hpp
 *
 *  Created on: 19/10/2015
 *      Author: craighhann
 */


#pragma once
#include <list>
#include <queue>
#include <set>
#include <vector>

//#include "VPoint.h"
#include "VEdge.hpp"
#include "Parabola.hpp"
#include "comp308.hpp"
#include "utility.hpp"
#include "cycleUtil.hpp"
#include "VEvent.hpp"
#include <functional>


namespace vor
{
/*
		Useful data containers for Vertices (places) and Edges of Voronoi diagram
 */

typedef std::list<comp308::vec2> vertices;
typedef std::list<VEdge> edges;

/*
		Class for generating the Voronoi diagram
 */

class Voronoi
{
public:

	/*
			Constructor - without any parameters
	 */

	Voronoi();

	/*
			The only public function for generating a diagram

			input:
				v		: Vertices - places for drawing a diagram
				w		: width  of the result (top left corner is (0, 0)) // Note will need to adapt this
				h		: height of the result
			output:
				pointer to list of edges

			All the data structures are managed by this class
	 */

	edges GetEdges(vertices v, int w, int h);

private:

	/*
						places		: container of places with which we work
						edges		: container of edges which will be teh result
						width		: width of the diagram
						height		: height of the diagram
						root		: the root of the tree, that represents a beachline sequence
						ly			: current "y" position of the line (see Fortune's algorithm)
	 */

	int parabolaID = 0;
	int count = 0;
	int nodeID = 0;
	int edgeID = 0;
	int eventID = 0;
	vertices places;
	edges edgeList;
	double	width = 0, height = 0;
	std::vector<Parabola> parabolas;
	int rootID = 0;
	double ly = 0;

	/*
						deleted		: set  of deleted (false) Events (since we can not delete from PriorityQueue
						points		: list of all new points that were created during the algorithm
						queue		: priority queue with events to process
	 */

	std::set<VEvent> deleted;
	std::vector<cycle::roadNode> points;
	std::priority_queue<VEvent, std::vector<VEvent>, VEvent::CompareEvent> queue;
	//std::priority_queue<VEvent> queue;
	/*
						InsertParabola		: processing the place event
						RemoveParabola		: processing the circle event
						FinishEdge			: recursively finishes all infinite edges in the tree
						GetXOfEdge			: returns the current x position of an intersection point of left and right parabolas
						GetParabolaByX		: returns the Parabola that is under this "x" position in the current beachline
						CheckCircle			: checks the circle event (disappearing) of this parabola
						GetEdgeInterse
	 */

	void		InsertParabola(comp308::vec2 p);
	void		RemoveParabola(VEvent  e);
	void		FinishEdge(Parabola  n);
	double		GetXOfEdge(Parabola par, double y);
	Parabola GetParabolaByX(double xx);
	double	GetY(comp308::vec2 p, double x);
	void CheckCircle(Parabola b);
	cycle::roadNode GetEdgeIntersection(cycle::road a, cycle::road b);

	Parabola  GetLeft(Parabola p);
	Parabola  GetRight(Parabola p);
	Parabola  GetLeftParent(Parabola p);
	Parabola  GetRightParent(Parabola p);
	Parabola  GetLeftChild(Parabola p);
	Parabola  GetRightChild(Parabola p);
};
}

