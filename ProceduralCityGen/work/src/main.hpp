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

/*Fields */
double g_winWidth 	= 640;
double g_winHeight 	= 480;
comp308::vec2 rotation = comp308::vec2(0,0);

/*Method declarations*/
void drawGrid(double,double);
void setupCamera(void);
static void mouseMotionCallbackFPS(GLFWwindow*, double, double);
