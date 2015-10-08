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

#include "comp308.hpp"
#include "section.hpp"
class Generator {

private:

public:
	static std::string generateRandomBuildingString(int);
	static std::string generateResdidentialBuildingString(int);
	static std::vector<comp308::vec2> generateFloorPlan(comp308::vec2,float,int);
	static std::vector<comp308::vec2> generateModernFloorPlan(comp308::vec2, float);
	static std::vector<comp308::vec2> cutEdges(std::vector<comp308::vec2> points);
	static comp308::vec2 centerPoint(std::vector<comp308::vec2> points);
	static std::vector<comp308::vec2> shrinkPoints(std::vector<comp308::vec2>);
	static std::vector<comp308::vec2> combinePlans(std::vector<comp308::vec2>, std::vector<comp308::vec2>);
	static section createRandomSection(void);
	std::vector<buildingParams> sectionsToParams(std::vector<section> sections);
};
