/*
 * VEdge.hpp
 *
 *  Created on: 19/10/2015
 *      Author: craighhann
 */

#pragma once

#include <iostream>
#include <functional>
#include "cycleUtil.hpp"
#include "utility.hpp"
#include "comp308.hpp"

/*
	A class that stores an edge in Voronoi diagram

	start		: pointer to start point
	end			: pointer to end point
	left		: pointer to Voronoi place on the left side of edge
	right		: pointer to Voronoi place on the right side of edge

	neighbour	: some edges consist of two parts, so we add the pointer to another part to connect them at the end of an algorithm

	direction	: directional vector, from "start", points to "end", normal of |left, right|
	f, g		: directional coeffitients satisfying equation y = f*x + g (edge lies on this line)
*/

class VEdge
{
public:
	cycle::road edge;
	comp308::vec2 direction;
	comp308::vec2 left;
	comp308::vec2 right;
	int ID;

	double		f;
	double		g;

	int neighbourID;

	/*
		Constructor of the class

		s		: pointer to start
		a		: pointer to left place
		b		: pointer to right place
	*/

	VEdge(){}

	VEdge(cycle::roadNode s, comp308::vec2 a, comp308::vec2 b, int id){
		edge = cycle::road();
		edge.start	= s;
		left		= a;
		right		= b;
		neighbourID	= -1;
		edge.end.ID = -1;
		ID = id;

		f = (b.x - a.x) / (a.y - b.y) ;
		g = s.location.y - f * s.location.x ;
		direction = vec2(b.y - a.y, -(b.x - a.x));
	}


};
