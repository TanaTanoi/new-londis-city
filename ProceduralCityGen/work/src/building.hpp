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
enum building_type { SKYSCRAPER = 0, RESIDENTIAL = 1, INDUSTRIAL = 2};
struct buildingParams {
	std::vector<comp308::vec2> boundingArea;	//the area in which this building is limited
	unsigned int seed = 0;								//the seed for generation
	building_type b_type = SKYSCRAPER;// = building_type.SKYSCRAPER;
	int height = 3;

};

/*Level of detail structure containing two displaylists*/
struct buildingLOD {
	int low;
	int high;
};



const float BLOCK_SIZE = 0.3f;
const float WINDOW_WIDTH = 0.1f;
const float EXTRUDE_THRESHOLD = 0.1f;//default 0.3
const int TOTAL_WALL_TEXTURES = 3;
const int TOTAL_WINDOW_TEXTURES = 5;

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
	std::vector<comp308::vec2> heightmap_points= std::vector<comp308::vec2>();

	void initTexture(void);//not currently working

	float extendBuilding(std::vector<comp308::vec2>, float);

	void renderWindows(std::vector<comp308::vec2> floor, float elevation);

	int generateRandomBuildings(void);
	static int basicHashcode(std::string);
	int generateBuildingFromString(std::string,float,int);	

	int generateBuildingsFromSections(std::string, std::vector<util::section>);	//Test method
	int generateBuildingsFromSections(std::vector<util::section>);

	void generateFromString(std::vector<comp308::vec2>, std::string);
	void generatePointRoof(std::vector<comp308::vec2>,float);
	void generateWindows(comp308::vec2,comp308::vec2 , float , comp308::vec3 );
	void generateDoor(comp308::vec2 a, comp308::vec2 b,float, comp308::vec3 normal);
	void generateFlatPointRoof(std::vector<comp308::vec2>,float);
	Building(void);
	void drawGround(float);
	void generateBuilding(buildingParams*, buildingLOD*);
	void generateBlock(util::section,float);
	void generateResdientialBuilding(std::vector<comp308::vec2>,int);
	void generateModernBuilding(std::vector<comp308::vec2> points,comp308::vec2,float);

	void generatePark(std::vector<comp308::vec2> floor);
	void generateParkWall(comp308::vec2 a, comp308::vec2 b, comp308::vec2 mid);

	void generateRoad(comp308::vec2,comp308::vec2,float);

};
