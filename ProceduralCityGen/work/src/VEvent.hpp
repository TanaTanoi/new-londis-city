/*
 * VEvent.hpp
 *
 *  Created on: 19/10/2015
 *      Author: craighhann
 */

#pragma once

#include <iostream>
#include <functional>
#include "cycleUtil.hpp"

/*
	The class for storing place / circle event in event queue.

	point		: the point at which current event occurs (top circle point for circle event, focus point for place event)
	pe			: whether it is a place event or not
	y			: y coordinate of "point", events are sorted by this "y"
	arch		: if "pe", it is an arch above which the event occurs
 */

class VEvent
{
public:
	comp308::vec2 point;
	bool pe = false;
	double y;
	int archIndex;
	int ID;

	/*
		Constructor for the class

		p		: point, at which the event occurs
		pev		: whether it is a place event or not
	 */
	VEvent(){}

	VEvent(comp308::vec2 p, bool pev, int id){
		point	= p;
		pe		= pev;
		y		= p.y;
		archIndex = -1;
		ID = id;
	}

	bool operator<(const VEvent& other) const {
		return (y < other.y);
	}

	/*
		function for comparing two events (by "y" property)
	 */


	struct CompareEvent	{
		bool operator()(VEvent l, VEvent r) { return (l.y > r.y); }
	};
};


