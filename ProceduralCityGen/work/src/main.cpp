//---------------------------------------------------------------------------
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
#include "geometry.hpp"
#include "main.hpp"
#include "building.hpp"
using namespace std;
using namespace comp308;



//Main program
//
int main(int argc, char **argv) {

	cout << "CityGen v 2.0" << endl;

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(g_winWidth, g_winHeight, "Procedural City Generator", nullptr, nullptr);
	if (!window){
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

	/*Setup callback functions*/
	glfwSetCursorPosCallback(window,mouseMotionCallbackFPS);
	glfwSetScrollCallback(window, mouseScrollCallback);

	/*Setting up other stuff*/

	building = Building();
	int testList = generateRandomBuildings();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)){

		/*## Render here ##*/
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.7f,0.7f,0.7f,1.0f);
		initLighting();
		setupCamera();
		
		drawGrid(10,1);
		glCallList(testList);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}


	glfwTerminate();

}

void initLighting() {
	float direction[] = { 0.0f, 0.0f, 1.0f, 0.5f };
	float diffintensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float ambient[] = { 0.6f, 0.2f, 0.2f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void setupCamera(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScalef(zoom, zoom, zoom);
	glRotatef(rotation.y,1.0f,0.0f,0.0f);
	glRotatef(rotation.x,0.0f,1.0f,0.0f);

}
/*Returns a display list */
int generateRandomBuildings() {
	/*Generate a random bunch of floor plans*/
	int toReturn = glGenLists(1);
	float size = 1.0f;
	float disp = 0.1f;

	glNewList(toReturn, GL_COMPILE);
	vector<vec2> points;
	for (float i = -size; i<size; i += disp) {
		for (float j = -size; j<i; j += disp) {
			points.clear();
			points.push_back(vec2(i, j));
			points.push_back(vec2(i + 0.1f, j));
			points.push_back(vec2(i + 0.1f, j + 0.1f));
			points.push_back(vec2(i, j + 0.1f));
			glColor3f(i + 1, j + 1, (i + j) / 2 + 1);
			glColor3f((i+size)/2, (i + size) / 2, (i + size) / 2);
			building.generateRandomBuilding(points);
		}
	}
	glEndList();
	return toReturn;
}

/**
 * Basic method that draws a grid specified by grid_sie and square_size
 */
void drawGrid(double grid_size, double square_size){
	glBegin(GL_LINES);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3d(grid_size,0,0);
	glVertex3d(-grid_size,0,0);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3d(0,5,0);
	glVertex3d(0,0,0);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3d(0,0,grid_size);
	glVertex3d(0,0,-grid_size);
	glColor3f(0.4,0.4,0.4);
	for(int i = -grid_size;i<grid_size+square_size;i+=square_size){
		glVertex3d(-grid_size,0,i);
		glVertex3d(grid_size,0,i);
	}
	for(int j = -grid_size;j<grid_size+square_size;j+=square_size){
		glVertex3d(j,0,-grid_size);
		glVertex3d(j,0,grid_size);
	}
	glEnd();
}



static void mouseMotionCallbackFPS(GLFWwindow* window, double xpos, double ypos){
	//The times two is to account for the movement back from setCursorPos
	rotation.x+=(xpos-rotation.x);
	rotation.y+=(ypos-rotation.y);
	// glfwSetCursorPos(window,g_winWidth/2,g_winHeight/2);
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset){

	if (yoffset > 0) {
		zoom *= 1.1;
	}
	else {
		zoom /= 1.1;
	}
}
