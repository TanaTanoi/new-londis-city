//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "comp308.hpp"
#include "main.hpp"
#include "building.hpp"
#include "generator.h"
#include "section.hpp"

using namespace std;
using namespace comp308;

int testList;
SectionDivider *g_sections = nullptr;

//0, bmode, 1, rmode, 2 cmode
int mode = 0;
//Main program
//
int main(int argc, char **argv) {

	cout << "CityGen v 0.1" << endl;

	/*Running some checks*/

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(g_winWidth, g_winHeight,
			"Procedural City Generator", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Initilise GLEW
	// must be done after creating GL context (glfwMakeContextCurrent in this case)
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}

	/*Test for joysticks/controllers */
	joystick = glfwJoystickPresent(GLFW_JOYSTICK_1);
	cout << joystick << " joystick.." << endl;

	/*Setup callback functions*/
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mouseMotionCallbackFPS);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
	/*Setting up other stuff*/
	/*Set up depths and perspective*/
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	string BMODE = "B";
	string RMODE = "R";
	string CMODE = "C";

	if(argc > 1 && argv[1] == BMODE ){
		init();
		testList = building.generateBuildingFromString("test");
		mode = 0;
		initLighting();
	}else if(argv[1] == RMODE){
		g_sections = new SectionDivider();
		g_sections->testSection();
		mode = 1;
	}else if(argv[1] == CMODE){
		mode = 2;
	}



	glEnable(GL_SMOOTH);
	//int testList = building.generateRandomBuildings();

	/* Loop until the user closes the window */

	// Create vehicle controller
	//g_vehicleCtrl = new VehicleController(
	//		"../work/res/assets/vehicle_config.txt",
	//		"../work/res/assets/tex_config.txt", vector<vec3>(), vec3());

	while (!glfwWindowShouldClose(window)) {

		/*## Render here ##*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

		if(mode == 0){

			setupCamera();
			initLighting();
			glTranslatef(0, -2, 0);
			drawGrid(40, 1);
			glCallList(testList);
		}else if(mode == 1){

			glClearColor(0.0, 0.0, 0.0, 0.0);  //Set the cleared screen colour to black
			glViewport(0, 0, g_winWidth, g_winHeight);   //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0, g_winWidth, 0, g_winHeight);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			g_sections->renderTest();
		}else if(mode == 2){
			// Draw vehicles
			//g_vehicleCtrl->renderVehicles();
		}
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
		joystickEventsPoll();
	}

	// Delete pointers
	//delete g_vehicleCtrl;
	g_sections->cleanUp();
	delete g_sections;
	glfwTerminate();

}
/*Any code that needs to get run just before the main loop, put it here*/
void init() {
	/*Set up depths and perspective*/
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	/*Create a new building object*/
	building = Building();
	building.initTexture();
}

void initLighting() {
	float direction[] = { 0.0f, 0.0f, -1.0f, 0.0f };
	float diffintensity[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float specular[] = { 1, 1, 1, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable(GL_LIGHT0);

	float light_position[] = { 0.0f, 5.0f, 0.0f, 1.0f };
	vec3 difference = (vec3(0, 0, 0)
			- vec3(light_position[0], light_position[1], light_position[2]));
	float spotlight_direction[] = { difference.x, difference.y, difference.z };

	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.5f);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlight_direction);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
}

/*Method taken from the internet that replicates gluPerspective using glFrustum*/
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar){
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;

	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	perspectiveGL(20.0, float(g_winWidth) / float(g_winHeight), 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -50*zoom);
	glRotatef(-rotation.y, 1.0f, 0.0f, 0.0f);
	glRotatef(-rotation.x, 0.0f, 1.0f, 0.0f);

}

/**
 * Basic method that draws a grid specified by grid_size and square_size
 */
void drawGrid(double grid_size, double square_size) {
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3d(grid_size, 0, 0);
	glVertex3d(-grid_size, 0, 0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3d(0, 5, 0);
	glVertex3d(0, 0, 0);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3d(0, 0, grid_size);
	glVertex3d(0, 0, -grid_size);
	glColor3f(0.4, 0.4, 0.4);
	for (int i = -grid_size; i < grid_size + square_size; i += square_size) {
		glVertex3d(-grid_size, 0, i);
		glVertex3d(grid_size, 0, i);
	}
	for (int j = -grid_size; j < grid_size + square_size; j += square_size) {
		glVertex3d(j, 0, -grid_size);
		glVertex3d(j, 0, grid_size);
	}
	glEnd();
}
//action = state, 1 is down, 0 is release
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	//cout << button << " " << action << " " << mods << endl;

	if (button == GLFW_MOUSE_BUTTON_1) {
		m_LeftButton = action;
	}else if (button == GLFW_MOUSE_BUTTON_2 && action){
		string input;
		cout << "Enter an input seed: ";
		cin >> input;
		testList = building.generateBuildingFromString(input);
	}
}

void mouseMotionCallbackFPS(GLFWwindow* window, double xpos, double ypos) {
	if (m_LeftButton) {
		rotation.x += (m_pos.x - xpos);
		rotation.y += (m_pos.y - ypos);
	}
	m_pos = vec2(xpos, ypos);
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	if (yoffset > 0) {
		zoom /= 1.1;
	}else{
		zoom *= 1.1;
	}
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
	g_winHeight = height;
	g_winWidth = width;
	glViewport(0, 0, width, height);

}

void joystickEventsPoll() {
	int count;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
	if (count > 0) {
		c_LSpos = vec2((float) ((int) (axes[0] * 100)) / 100.0f,
				(float) ((int) (axes[1] * 100)) / 100.0f);
		rotation.x += (0.0f - c_LSpos.x);
		rotation.y += (0.0f - c_LSpos.y);

	}
}
