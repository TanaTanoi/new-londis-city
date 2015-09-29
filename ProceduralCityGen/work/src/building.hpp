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
enum building_type { SKYSCRAPER = 0, RESIDENTIAL = 1, INDUSTRIAL = 2};
struct buildingParams {
	std::vector<comp308::vec2> boundingArea;	//the area in which this building is limited
	unsigned int seed = 0;								//the seed for generation
	building_type b_type = SKYSCRAPER;// = building_type.SKYSCRAPER;

};

/*Level of detail structure containing two displaylists*/
struct buildingLOD {
	int low;
	int high;
};



const float BLOCK_SIZE = 0.3f;
const float WINDOW_WIDTH = 0.1f;
const float EXTRUDE_THRESHOLD = 0.3f;//TODO make extrude_threshold a function of floorplan size

const int tex_wall_num = 2;
const int tex_window_num = 2;


class Building {

private:
	GLuint g_shader = 0;
	int cur_tex_wall = 0;	//what type of wall to use
	int cur_tex_wall_num = 1;//what wall texture to use

	float tex_wall_width = 0.5f;

	int cur_tex_win = 0;	//what type of window to use
	int cur_tex_win_num = 0;//what window texture to use
	void initShader(void);//not currently working

public:
	void initTexture(void);//not currently working

	float extendBuilding(std::vector<comp308::vec2>, float);
	std::vector<comp308::vec2> shrinkPoints(std::vector<comp308::vec2>);
	std::vector<std::vector<comp308::vec2>> subdivide(std::vector<comp308::vec2>);

	comp308::vec2 centerPoint(std::vector<comp308::vec2>);
	int generateRandomBuildings(void);
	static int basicHashcode(std::string);
	int generateBuildingFromString(std::string);
	void generateFromString(std::vector<comp308::vec2>, std::string);
	void generatePointRoof(std::vector<comp308::vec2>,float);
	void generateWindows(comp308::vec2,comp308::vec2 , float , comp308::vec3 );
	void generateFlatPointRoof(std::vector<comp308::vec2>,float);
	Building(void);

	void generateBuilding(buildingParams*, buildingLOD*);

	void generateResdientialBuilding(std::vector<comp308::vec2> points);



};
