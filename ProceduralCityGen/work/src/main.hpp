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

/*Skybox texture*/
GLuint skyboxID = 0;
/*Skybox shader*/
GLuint skybox_shader = 0;

/*Mouse position*/
comp308::vec2 m_pos = comp308::vec2(0, 0);
float pitch = 0;
float yaw = -90.0f;
comp308::vec3 p_pos = comp308::vec3(0,1,2);
comp308::vec3 p_front = comp308::vec3(0, 0, -1);
comp308::vec3 p_up= comp308::vec3(0, 1, 0);
comp308::vec3 p_tar = comp308::vec3(0, 1, 0);
comp308::vec3 p_dir = comp308::normalize(p_pos - p_tar);

/*Controler Left Stick/Right Stick Axes position
Values between -1 and 1*/
comp308::vec2 c_LSpos = comp308::vec2(0, 0);
comp308::vec2 c_RSpos = comp308::vec2(0, 0);
float zoom = 1;

comp308::vec2 rotation = comp308::vec2(0,0);

/*If a joystick is present*/
int joystick;
int cam_mode = 0;//0 for model view, 1 for FPS

Building building;
Generator generator;

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
