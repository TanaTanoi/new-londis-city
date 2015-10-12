
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "roadNetwork.hpp"
#include "comp308.hpp"
#include "main.hpp"
#include "building.hpp"
#include "generator.h"
#include "section.hpp"
#include "shaderLoader.hpp"
#include "imageLoader.hpp"
using namespace std;
using namespace comp308;

int testList;
SectionDivider *g_sections = nullptr;
RoadNetwork *g_network = nullptr;

// Controls the vehicles in the city
VehicleController *g_vehicleCtrl = nullptr;

//0, bmode, 1, smode, 2 cmode, 3 rmode
int mode = 0;
//Main program
//
int main(int argc, char **argv) {

	cout << "Procedural City Generator v 1.1" << endl;

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
	glfwSetCursorPosCallback(window, mouseMotionCallbackModelView);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	/*Setting up other stuff*/
	if (joystick) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	/*Set up depths and perspective*/
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	string BMODE = "B";
	string SMODE = "S";
	string RMODE = "R";
	string CMODE = "C";

	if (argc > 1 && argv[1] == BMODE) {
		cout << "Building mode" << endl;
		init();
		string one = "1";
		if (argc >= 2 && std::string(argv[2]) == "1") {
			cout << "CAM MODE" << endl;
			cam_mode = 1;
			glfwSetCursorPosCallback(window, mouseMotionCallbackFPS);
		}
		//testList = building.generateBuildingFromString("testd");
		g_sections = new SectionDivider();
		testList = building.generateBuildingsFromSections("gfggghhhhtagagdg",
				g_sections->testSection().sections);
		mode = 0;
		initLighting();
	} else if (argv[1] == SMODE) {
		cout << "Section mode" << endl;
		g_sections = new SectionDivider();
		g_sections->testSection();
		mode = 1;
	} else if (argv[1] == RMODE) {
		glfwSetCursorPosCallback(window, mouseMotionCallback2D);
		cout << "Road mode" << endl;
		g_network = new RoadNetwork();
		g_network->testNetwork();
		mode = 3;
	} else if (argv[1] == CMODE) {
		cout << "Car mode" << endl;
		g_vehicleCtrl = new VehicleController(
				"../work/res/assets/vehicle_config.txt",
				"../work/res/assets/tex_config.txt", vector<vec3>(), vec3());
		g_vehicleCtrl->tick();
		mode = 2;
	} else {
		init();
		g_sections = new SectionDivider();
		glfwSetCursorPosCallback(window, mouseMotionCallback2D);
		//cout << "entered no loops" << endl;
		mode = 4;//because I'm very lazy
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
		//TODO clean up all of this gross code when we integrate
		if (mode == 0) {
			setupCamera();
			initLighting();
			glTranslatef(0, -2, 0);
			drawGrid(40, 1);
			glCallList(testList);
			drawSkycube(100.0f);//the further away it is the better it looks
		} else if (mode == 1 || mode == 3) {

			glClearColor(0.0, 0.0, 0.0, 0.0); //Set the cleared screen colour to black
			glViewport(0, 0, g_winWidth, g_winHeight); //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0, g_winWidth, 0, g_winHeight);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			if (mode == 1) {
				g_sections->renderTest();
			} else {
				glPushMatrix();
				glScalef(zoom, zoom, 0);
				glTranslatef(p_pos.x, p_pos.y, 0);
				g_network->renderRoads();
				glPopMatrix();
			}
		} else if (mode == 2) {
			// Draw vehicles
			//g_vehicleCtrl->renderVehicles();
		}else if (mode == 4) {
			//Spline map mode
			glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0, g_winWidth, 0, g_winHeight);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glEnable(GL_PROGRAM_POINT_SIZE);
			glPointSize(10.0f);


			glColor3f(1, 0, 0);
			for (int i = 1; i < heightmap_points.size();i++) {
				cout<< heightmap_points[i-1].x << endl;
				glBegin(GL_LINES);
				glVertex2f(heightmap_points[i-1].x, g_winHeight-heightmap_points[i-1].y);
				glVertex2f(heightmap_points[i].x, g_winHeight-heightmap_points[i].y);
				glEnd();
			}

		}
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
		joystickEventsPoll();
	}

	// Delete pointers
	delete g_sections;
	delete g_network;
	delete g_vehicleCtrl;
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
	initSkybox("../work/res/textures/cubeMap.jpg");
	skybox_shader = makeShaderProgram("../work/res/shaders/skybox_shader.vert",
			"../work/res/shaders/skybox_shader.frag");
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

//	float light_position[] = { 0.0f, 5.0f, 0.0f, 1.0f };
	float light_position[] = { p_pos.x, p_pos.y, p_pos.z, 1.0f };
	vec3 difference = (vec3(0, 0, 0)
			- vec3(light_position[0], light_position[1], light_position[2]));
	float spotlight_direction[] = { p_dir.x, p_dir.y, p_dir.z, 0.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
//	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 0.3f);
//	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);
//	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlight_direction);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
}

/*Method taken from the internet that replicates gluPerspective using glFrustum*/
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear,
		GLdouble zFar) {
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;

	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void lookAt(vec3 pos, vec3 center, vec3 up) {
	gluLookAt(pos.x, pos.y, pos.z, center.x, center.y, center.z, up.x, up.y,
			up.z);

}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	perspectiveGL(20.0, float(g_winWidth) / float(g_winHeight), 0.1f, 1000.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	perspectiveGL(45.0, float(g_winWidth) / float(g_winHeight), 0.1f, 1000.0f);
	if (cam_mode) {
		lookAt(p_pos, p_pos + p_front, p_up);
	} else {
		glTranslatef(0, 0, -50 * zoom);
		glRotatef(-rotation.y, 1.0f, 0.0f, 0.0f);
		glRotatef(-rotation.x, 0.0f, 1.0f, 0.0f);
	}
	//glTranslatef(0,0,-50*zoom);
	//vec3 up = vec3(0, 1, 0);
	//vec3 camRight = normalize(cross(up, p_front));
	//vec3 camUp = cross(p_front, camRight);

	//glRotatef(-p_dir.y,1.0f,0.0f,0.0f);
	//glRotatef(-p_dir.x,0.0f,1.0f,0.0f);
	//glTranslatef(-p_pos.x,-p_pos.y,-p_pos.z);

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
	if (mode == 4&&action&&button == GLFW_MOUSE_BUTTON_1) {
		if(heightmap_points.size()<2){
			heightmap_points.push_back(m_pos);
				return;
			}
			float x = heightmap_points[1].x;//set first point for comparison ([0] is 0,0)
			int i = 2;
			while(x<m_pos.x&&i<=heightmap_points.size()){
				x = heightmap_points[i].x;
				++i;
			}i--;
			heightmap_points.insert(heightmap_points.begin()+i,m_pos);


		heightmap_points.push_back(m_pos);
		cout << "adding point " << m_pos.x << " " << m_pos.y << endl;
		return;
	}
	if (button == GLFW_MOUSE_BUTTON_1) {
		m_LeftButton = action;
	} else if (button == GLFW_MOUSE_BUTTON_2 && action) {
		string input;
		cout << "Enter an input seed: ";
		cin >> input;
		//testList = building.generateBuildingFromString(input);
		testList = building.generateBuildingsFromSections(input,
				g_sections->testSection().sections);
		mode = 0;
	}
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	cout << "Key: " << key << endl;
	if (key == 87 && action) {
		p_pos += 0.05f * p_front * 1;
	} else if (key == 65 && action) {
		vec3 p_right = cross(p_up, p_front);
		p_pos += 0.05f * p_right * 1;
	} else if (key == 68 && action) {
		vec3 p_right = cross(p_up, p_front);
		p_pos -= 0.05f * p_right * 1;
	} else if (key == 63 && action) {
		p_pos -= 0.05f * p_front * 1;
	}

}
bool firstM = true;
void mouseMotionCallbackFPS(GLFWwindow* window, double xpos, double ypos) {
	if (firstM) {
		m_pos = vec2(xpos, ypos);
		firstM = false;
	}
	if (m_LeftButton) {
		//vector normalised to give a -1 to 1 value on the screen
		vec2 normalisedVec = vec2(
				-((g_winWidth / 2) - m_pos.x) / (g_winWidth / 2),
				((g_winHeight / 2) - m_pos.y) / (g_winHeight / 2));
		yaw += (normalisedVec.x - 0.0f) * 1.2f;
		pitch += (normalisedVec.y) * 1.2f;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		float pitchR = radians(pitch);
		float yawR = radians(yaw);
		p_dir.x = cos(yawR) * cos(pitchR);
		p_dir.y = sin(pitchR);
		p_dir.z = sin(yawR) * cos(pitchR);
		p_front = normalize(p_dir);
	}
	m_pos = vec2(xpos, ypos);
}

void mouseMotionCallbackModelView(GLFWwindow* window, double xpos,
		double ypos) {
	if (firstM) {
		m_pos = vec2(xpos, ypos);
		firstM = false;
	}

	if (m_LeftButton) {
		rotation.x += (m_pos.x - xpos);
		rotation.y += (m_pos.y - ypos);
	}
	m_pos = vec2(xpos, ypos);
}

void mouseMotionCallback2D(GLFWwindow* window, double xpos, double ypos) {
	if (firstM) {
		m_pos = vec2(xpos, ypos);
		firstM = false;
	}

	if (m_LeftButton) {
		p_pos.x -= (m_pos.x - xpos);
		p_pos.y += (m_pos.y - ypos);
	}
	m_pos = vec2(xpos, ypos);
}
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	if (yoffset > 0) {
		zoom /= 1.1;
	} else {
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
	//0,1-Left Stick 2 is trigger (postive left, negative, right) 3,4 is Right Stick y,x
	if (count > 0) {

		c_LSpos = vec2((float) ((int) (axes[0] * 100)) / 100.0f,
				(float) ((int) (axes[1] * 100)) / 100.0f);
		vec2 c_RSpos = vec2((float) ((int) (axes[4] * 100)) / 100.0f,
				(float) ((int) (axes[3] * 100)) / 100.0f);

		float threshold = 0.2f;	//stops drifting when the user isn't touching the controler
		if (abs(c_RSpos.y) >= threshold) {
			yaw += (c_RSpos.y - 0.0f) * 1.2f;
		}
		if (abs(c_RSpos.x) >= threshold) {
			pitch += (0.0f - c_RSpos.x) * 1.2f;
		}
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		float pitchR = radians(pitch);
		float yawR = radians(yaw);
		p_dir.x = cos(yawR) * cos(pitchR);
		p_dir.y = sin(pitchR);
		p_dir.z = sin(yawR) * cos(pitchR);
		p_front = normalize(p_dir);

		if (abs(c_LSpos.y) < threshold) {
			c_LSpos.y = 0;
		}
		if (abs(c_LSpos.x) < threshold) {
			c_LSpos.x = 0;
		}

		p_pos -= 0.05f * p_front * c_LSpos.y * 1.4;

		vec3 p_right = cross(p_up, p_front);
		p_pos -= 0.05f * p_right * c_LSpos.x * 1.4;

	}

}

void initSkybox(string filepath) {

	cout << "Loading cubemap " << filepath << endl;

	glGenTextures(1, &skyboxID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);

	image tex(filepath);
	int cubeWidth = ceil(tex.w / 4.0f);

	//images for each part of the cubemap
	image p_x = tex.subImage(cubeWidth * 2, cubeWidth * 1, cubeWidth,
			cubeWidth);	//right
	image n_x = tex.subImage(cubeWidth * 0, cubeWidth * 1, cubeWidth,
			cubeWidth);	//left
	image p_y = tex.subImage(cubeWidth * 1, cubeWidth * 0, cubeWidth,
			cubeWidth);	//top
	image n_y = tex.subImage(cubeWidth * 1, cubeWidth * 2, cubeWidth,
			cubeWidth);	//bot
	image n_z = tex.subImage(cubeWidth * 3, cubeWidth * 1, cubeWidth,
			cubeWidth);	//back
	image p_z = tex.subImage(cubeWidth * 1, cubeWidth * 1, cubeWidth,
			cubeWidth);	//front
	vector<image> faces = vector<image>();
	faces.push_back(p_x);
	faces.push_back(n_x);
	faces.push_back(p_y);
	faces.push_back(n_y);
	faces.push_back(p_z);
	faces.push_back(n_z);
	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, cubeWidth,
				cubeWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[i].dataPointer());
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, GL_RGB8,
				cubeWidth, cubeWidth, faces[i].glFormat(), GL_UNSIGNED_BYTE,
				faces[i].dataPointer());
	}

	//Specify cubemap paramters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glUniform1i(glGetUniformLocation(skybox_shader, "skybox"),
	GL_TEXTURE_CUBE_MAP);

}

void drawSkycube(float size) {
	const float floor = -1.0f;
	glUseProgram(skybox_shader);
	vector<vec2> box = vector<vec2>();
	box.push_back(vec2(-size, -size));
	box.push_back(vec2(-size, size));
	box.push_back(vec2(size, size));
	box.push_back(vec2(size, -size));

	glBegin(GL_QUADS);
	//Floor

	size /= 1.5;
	glNormal3f(0, 1, 0);
	for (int i = 0; i < 4; i++) {
		glVertex3f(box[i].x, floor, box[i].y);
	}
	glNormal3f(0, -1, 0);
	for (int i = 3; i >= 0; i--) {
		glVertex3f(box[i].x, size, box[i].y);
	}

	for (int i = 0; i < 4; i++) {
		vec3 normal = vec3(0, 0, 0) - vec3(box[i].x, 0, box[i].y);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f(0, 0);
		glVertex3f(box[i].x, floor, box[i].y);
		normal = vec3(0, 0, 0) - vec3(box[i].x, -size, box[i].y);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f(0, 1);
		glVertex3f(box[i].x, size, box[i].y);
		normal = vec3(0, 0, 0) - vec3(box[i].x, -size, box[i].y);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f(1, 1);
		glVertex3f(box[(i + 1) % 4].x, size, box[(i + 1) % 4].y);
		normal = vec3(0, 0, 0) - vec3(box[i].x, -size, box[i].y);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f(1, 0);
		glVertex3f(box[(i + 1) % 4].x, floor, box[(i + 1) % 4].y);
	}

	glEnd();

}
