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

using namespace std;
using namespace comp308;



//Main program
//
int main(int argc, char **argv) {

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




	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)){

		/*## Render here ##*/
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		setupCamera();
		drawGrid(10,1);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}


	glfwTerminate();

}

void setupCamera(){
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

 glRotatef(rotation.y,1.0f,0.0f,0.0f);
 glRotatef(rotation.x,0.0f,1.0f,0.0f);

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
	rotation.x+=(xpos-rotation.x)*2;
	rotation.y+=(ypos-rotation.y)*2;
	 glfwSetCursorPos(window,g_winWidth/2,g_winHeight/2);
}
