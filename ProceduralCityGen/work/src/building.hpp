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

const float BLOCK_SIZE = 0.5f;

class Building {
private:
	GLuint g_shader = 0;
	void initShader(void);//not currently working
	void initTexture(void);//not currently working

public:
	float extendBuilding(std::vector<comp308::vec2>, float);
	std::vector<comp308::vec2> shrinkPoints(std::vector<comp308::vec2>);
	comp308::vec2 centerPoint(std::vector<comp308::vec2>);
	int generateRandomBuildings(void);
	int generateBuildingFromString(std::string input);
	void generateFromString(std::vector<comp308::vec2>, std::string);
	void generatePointRoof(std::vector<comp308::vec2>,float);
	void generateFlatPointRoof(std::vector<comp308::vec2>,float);
	Building(void);
};
