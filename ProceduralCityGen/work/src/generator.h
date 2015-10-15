#pragma once

/*
* building.hpp
*
*  Created on: 15/09/2015
*      Author: tanoitana
*/

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "utility.hpp"
#include "comp308.hpp"
#include "building.hpp"
class Generator {

private:

public:

	static float SECTION_TO_POINT_SCALE(void);
	static std::string generateRandomBuildingString(int);
	static std::string generateResdidentialBuildingString(int);
	static std::vector<comp308::vec2> generateFloorPlan(comp308::vec2,float,int);
	static std::vector<comp308::vec2> generateModernFloorPlan(comp308::vec2, float);
	static std::vector<comp308::vec2> cutEdges(std::vector<comp308::vec2> points);
	static std::vector<comp308::vec2> shrinkPoints(std::vector<comp308::vec2>);
	static std::vector<std::vector<comp308::vec2>> subdivide(std::vector<comp308::vec2> points);
	static std::vector<comp308::vec2> combinePlans(std::vector<comp308::vec2>, std::vector<comp308::vec2>);
	static comp308::vec2 centerPoint(std::vector<comp308::vec2> points);
	static util::section createRandomSection(void);
	static std::vector<buildingParams> sectionsToParams(std::vector<util::section> sections,std::vector<comp308::vec2>);
	static std::vector<comp308::vec2> getBoundingBox(std::vector<comp308::vec2> floor);
	static std::vector<comp308::vec2> sectionToPoints(util::section);
	static util::section pointsToSections(std::vector<comp308::vec2>);

};
