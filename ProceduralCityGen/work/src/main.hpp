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

/*Fields */
double g_winWidth 	= 640;
double g_winHeight 	= 480;

bool m_LeftButton = false;
/*Mouse position*/
comp308::vec2 m_pos = comp308::vec2(0, 0);
/*Controler Left Stick/Right Stick Axes position
Values between -1 and 1*/
comp308::vec2 c_LSpos = comp308::vec2(0, 0);
comp308::vec2 c_RSpos = comp308::vec2(0, 0);
float zoom = 1;

comp308::vec2 rotation = comp308::vec2(0,0);

/*If a joystick is present*/
int joystick;


Building building;
Generator generator;
VehicleController *g_vehicleCtrl = nullptr;

/*-----------------
Method declarations
-----------------*/
/*Initalization declarations*/
void setupCamera(void);
void initLighting(void);
void init(void);

/*Rendering delcarations*/
void drawGrid(double,double);
int generateBuildingFromString(std::string);

//int generateHexagonBuilding(float,float);

/*Callback delcarations*/
void mouseButtonCallback(GLFWwindow* , int , int , int );
void mouseMotionCallbackFPS(GLFWwindow*, double, double);
void mouseScrollCallback(GLFWwindow*, double, double);
void windowSizeCallback(GLFWwindow*, int , int);

void joystickEventsPoll(void);
