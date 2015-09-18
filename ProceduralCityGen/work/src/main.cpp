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
using namespace std;
using namespace comp308;


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
	window = glfwCreateWindow(g_winWidth, g_winHeight, "Procedural City Generator", nullptr, nullptr);
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

	int testList = generateRandomBuildings();
//	int testList = generateHexagonBuilding(0.0f,0.0f);
	/* Loop until the user closes the window */
	initLighting();
	while (!glfwWindowShouldClose(window)) {

		/*## Render here ##*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

		setupCamera();
		initLighting();
		drawGrid(10, 1);
		glCallList(testList);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
		joystickEventsPoll();
		// woo poll events
	}


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

	/*Create a new building object*/
	building = Building();
}

void initLighting() {
	float direction[] = { 0.0f, 0.0f, 1.0f, 0.5f };
	float diffintensity[] = { 0.7f, 0.7f, 0.7f, 0.1f };
	float ambient[] = { 0.9f, 0.9f, 0.9f, 0.1f };
	float specular[] = { 0.5, 0.5, 1.0, 0.1 };
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable(GL_LIGHT0);

	float light_position[] = { 1.0f, 10.0f, 0.0f,1.0f};
	vec3 diff = (vec3(0,0,0)-vec3(light_position[0],light_position[1],light_position[2]));
	float spotlight_position[] = { diff.x, diff.y, diff.z};
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 46.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 4.0);
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spotlight_position);

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffintensity);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);

//    glEnable(GL_LIGHTING);
}

void setupCamera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(zoom, zoom, zoom);
	glRotatef(rotation.y, 1.0f, 0.0f, 0.0f);
	glRotatef(rotation.x, 0.0f, 1.0f, 0.0f);

}

/*Returns a display list */
int generateRandomBuildings() {
	/*Generate a random bunch of floor plans*/
	int toReturn = glGenLists(1);
	float size = 2.4f;
	float disp = 1.2f;
	float building_size = 1.2f;
	glNewList(toReturn, GL_COMPILE);
	vector<vec2> points;
	for (float i = -size; i <= size; i += disp) {
		for (float j = -size; j <= i; j += disp) {
			points.clear();
			points.push_back(vec2(i, j));
			points.push_back(vec2(i + building_size, j));
			points.push_back(vec2(i + building_size, j + building_size));
			points.push_back(vec2(i, j + building_size));
			glColor3f(i + 1, j + 1, (i + j) / 2 + 1);
			glColor3f((i + size) / 2, (i + size) / 2, (i + size) / 2);
			building.generateBlock(points,-1.0f);
		}
	}
	glEndList();
	return toReturn;
}

/*Returns a display list of a hexagon shaped building */
int generateHexagonBuilding(float x, float y) {
	/*Generate a random bunch of floor plans*/
	int toReturn = glGenLists(1);
	vector<vec2> points;

	points.push_back(vec2(x, y) - vec2(1.0f,2.0f));
	points.push_back(vec2(x, y) - vec2(0.0f, 2.0f));
	points.push_back(vec2(x, y) - vec2(0.0f, 0.0f));
	points.push_back(vec2(x, y) - vec2(1.0f, 0.0f));
	points.push_back(vec2(x, y) - vec2(1.0f, 2.0f));


	glNewList(toReturn, GL_COMPILE);
	building.generateBlock(points,0.0f);
	glEndList();
	return toReturn;
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
	cout << button << " " << action << " " << mods << endl;

	if (button == GLFW_MOUSE_BUTTON_1) {
		m_LeftButton = action;
	}
}

void mouseMotionCallbackFPS(GLFWwindow* window, double xpos, double ypos) {
	if (m_LeftButton) {
		rotation.x += (m_pos.x-xpos);
		rotation.y += (m_pos.y-ypos );
	}
	m_pos = vec2(xpos, ypos);
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	if (yoffset > 0) {
		zoom *= 1.1;
	}
	else {
		zoom /= 1.1;
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
		c_LSpos = vec2((float)((int)(axes[0]*100))/100.0f, (float)((int)(axes[1] * 100)) / 100.0f);
		rotation.x += (0.0f - c_LSpos.x);
		rotation.y += (0.0f - c_LSpos.y);


	}
}
