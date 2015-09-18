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

class Building {
private:
	void initTexture(void);//not currently working
	void parseChar(char);

public:
	float generateBlock(std::vector<comp308::vec2>, float);
	std::vector<comp308::vec2> shrinkPoints(std::vector<comp308::vec2>);
	comp308::vec2 centerPoint(std::vector<comp308::vec2>);
	void generateFromString(std::vector<comp308::vec2>, std::string);
	void generatePointRoof(std::vector<comp308::vec2>,float);
};
