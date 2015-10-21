/*
 * main.hpp
 *
 *  Created on: 15/09/2015
 *      Author: tanoitana
 */

#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "comp308.hpp"
#include "building.hpp"
#include "generator.h"
#include "vehicle.hpp"
#include "vehicleController.hpp"

/*Fields */
double g_winWidth 	= 640;
double g_winHeight 	= 480;

bool m_LeftButton = false;

GLFWwindow* window;

/*These control the flags of the program*/
enum Options{
	op_fullscreen =	0x01,
	op_heightmap  =	0x02,
	op_joystick   =	0x04,
	op_modelview  = 0x08,
	op_fullbright = 0x10,
};
unsigned char options = 0;

/*What mode the program is in*/
int mode = 0;
/*Different mode constants*/
const int FULL_MODE = 0;
const int SHOWCASE_MODE = 2;
const int CAR_MODE = 3;
const int NETWORK_MODE = 4;
const int SECTION_MODE = 5;
/*Showcase mode options*/
int showcase_mode_type = 0;
float showcase_mode_size = 1.0f;
int showcase_mode_angle = 0;
/* Car mode options*/
int car_mode_number = 10;

/*Network mode options*/
int network_mode_type = 0;
int network_mode_size = 100;
int network_mode_cycles = 0;

/*Skybox texture*/
GLuint skyboxID = 0;
/*Skybox shader*/
GLuint skybox_shader = 0;



/*Mouse position*/
comp308::vec2 m_pos = comp308::vec2(0, 0);
float pitch = 0;
float yaw = -90.0f;
comp308::vec3 p_pos = comp308::vec3(0,4,2);
comp308::vec3 p_front = comp308::vec3(0, 0, -1);
comp308::vec3 p_up= comp308::vec3(0, 1, 0);
comp308::vec3 p_tar = comp308::vec3(0, 1, 0);
comp308::vec3 p_dir = comp308::normalize(p_pos - p_tar);

/*Controler Left Stick/Right Stick Axes position
Values between -1 and 1*/
comp308::vec2 c_LSpos = comp308::vec2(0, 0);
comp308::vec2 c_RSpos = comp308::vec2(0, 0);
float zoom = 1;

float triggerDiff = 2.0f;


comp308::vec2 rotation = comp308::vec2(0,0);

Building building;
Generator generator;

/*-----------------
Method declarations
-----------------*/
/*Initalization declarations*/
void setupCamera(void);
void initLighting(void);
void initBuildingGenerator(void);
void generateBuildings(void);
void renderLoop(void);
/*Rendering delcarations*/
void drawGrid(double,double);
int generateBuildingFromString(std::string);
void initSkybox(std::string);
void drawSkycube(float);
//int generateHexagonBuilding(float,float);

/*Callback delcarations*/
void mouseButtonCallback(GLFWwindow* , int , int , int );
void mouseMotionCallbackFPS(GLFWwindow*, double, double);
void mouseMotionCallbackModelView(GLFWwindow*, double, double);
void mouseMotionCallback2D(GLFWwindow*, double, double);
void mouseScrollCallback(GLFWwindow*, double, double);
void windowSizeCallback(GLFWwindow*, int , int);
void keyCallback(GLFWwindow* , int , int , int , int );
void joystickEventsPoll(void);
