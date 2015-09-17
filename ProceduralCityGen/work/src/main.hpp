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
/*Fields */
double g_winWidth 	= 640;
double g_winHeight 	= 480;

bool m_LeftButton = false;
comp308::vec2 m_pos = comp308::vec2(0, 0);

float zoom = 1;

comp308::vec2 rotation = comp308::vec2(0,0);

Building building;

/*Method declarations*/
void drawGrid(double,double);
int generateRandomBuildings(void);
void setupCamera(void);
void initLighting(void);
void init(void);
void mouseButtonCallback(GLFWwindow* , int , int , int );
void mouseMotionCallbackFPS(GLFWwindow*, double, double);
void mouseScrollCallback(GLFWwindow*, double, double);
void windowSizeCallback(GLFWwindow*, int , int);
