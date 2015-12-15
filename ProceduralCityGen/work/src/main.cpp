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
#include "utility.hpp"
#include "cycleUtil.hpp"
#include "shaderLoader.hpp"
#include "imageLoader.hpp"
using namespace std;
using namespace comp308;

int single_list;
SectionDivider *g_sections = nullptr;
RoadNetwork *g_network = nullptr;

// Controls the vehicles in the city
VehicleController *g_vehicleCtrl = nullptr;

/*Supplied seed for generation*/
int user_seed = 42;

// City size
int city_size;
int devianceLevel = -1;

//Main program
//
int main(int argc, char **argv) {

	cout << "Procedural City Generator v 1.1" << endl;

	/*Running some checks*/

	options = options & 0;

	/*Read command line arguments and augment options. This only sets options, it doesn't run any code*/
	for (int i = 1; i < argc; i++) {
		string argument = string(argv[i]);
		if (argument.compare("showcase") == 0) {
			//Showcase mode "showcase [type] [size]" and can take optional seed
			if (i + 2 >= argc) {
				cout
				<< "Showcase mode requires two parameters, [type] and [size]"
				<< "\n";
				cout << "    (e.g. 'showcase 3 15')" << endl;
				return -1;
			} else if (i != 1) {
				cout
				<< "Showcase mode is a mode, not a parameter. It must be the first argument"
				<< endl;
				return -1;
			} else {
				mode = SHOWCASE_MODE;
				showcase_mode_type = std::atoi(argv[i + 1]);
				showcase_mode_size = std::atoi(argv[i + 2]) / 10.0f;
				i = i + 2;
			}
		} else if (argument.compare("car") == 0) {
			if (i != 1) {
				cout
				<< "Car mode is a mode, not a parameter, It must be the first argument"
				<< endl;
				return -1;
			}
			options = options | op_modelview;
			if (i + 1 < argc) { //check for optional parameter
				// add optional parameter
				if (argv[i + 1][0] != '-') {
					car_mode_number = std::atoi(argv[i + 1]);
					i++;
				}
			}
			mode = CAR_MODE;
		}else if (argument.compare("network") == 0) {
			if (i + 3 >= argc) {
				cout
				<< "Network mode requires three parameters, [type], [size], and [cycle bool]"
				<< endl;
				return -1;
			} else if (i != 1) {
				cout
				<< "Network mode is a mode, not a parameter, It must be the first argument"
				<< endl;
				return -1;
			} else {
				//if valid
				network_mode_type = std::atoi(argv[i + 1]);
				network_mode_size = std::atoi(argv[i + 2]);
				network_mode_cycles = std::atoi(argv[i + 3]);
				city_size = network_mode_size;
				i = i + 3;
				options = options|op_customsize;
			}
			mode = NETWORK_MODE;
		}else if(argument.compare("section")==0){
			if (i != 1) {
				cout<< "Section mode is a mode, not a parameter, It must be the first argument"<< endl;
				return -1;
			}
			mode = SECTION_MODE;
		}else if(argument.compare("-seed")==0){
			if(i+1>=argc){//if we don't have an additional argument
				cout<<"Seed requires a parameter (example usage. '-seed COMP308')"<<endl;
				return -1;
			}else{
				user_seed = Building::basicHashcode(std::string(argv[i+1]));
				cout<<"Using seed "<< argv[i+1]<<endl;
				options = options|op_customseed;
				i+=1;
			}
		}else if(argument.compare("-size")==0){
			if(i+1>=argc){//if we don't have an additional argument
				cout<<"size requires a parameter (example usage. '-seed COMP308')"<<endl;
				return -1;
			}else{
				city_size = std::atoi(argv[i + 1]);
				options = options|op_customsize;
				i+=1;
			}
		}else if(argument.compare("-totalcars")==0){
			if(i+1>=argc){//if we don't have an additional argument
				cout<<"Total Cars requires a parameter (example usage. '-totalcars 70')"<<endl;
				return -1;
			}else{
				car_mode_number = std::atoi(argv[i + 1]);
				i+=1;
			}
		} else if (argument.compare("-heightmap") == 0) {
			//if the user wants a heightmap to pop up
			cout << "Heightmap Enabled" << endl;
			options = options | op_heightmap;

		} else if (argument.compare("-fullscreen") == 0) {
			//enable fullscreen
			options = options | op_fullscreen;
			cout << "Fullscreen Enabled" << endl;

		} else if (argument.compare("-joystick") == 0) {
			//enable joystick if it is present
			options = options| op_joystick;

		}else if (argument.compare("-modelview") == 0) {
			options = options|op_modelview;
			p_dir = normalize(-p_pos);//Set spot light position to look at center point
		}else if(argument.compare("-fullbright")==0){
			options = options|op_fullbright;
		}else if(argument.compare("-experimental")==0){
			options = options|op_experimental;
		} else if(argument.compare("-deviance")==0){
			if(i+1>=argc){//if we don't have an additional argument
				cout<<"Deviance requires a parameter (example usage. '-deviance 0')"<<endl;
				return -1;
			}else{
				devianceLevel = (std::atoi(argv[i+1]));
				cout<<"Using set deviance "<< argv[i+1]<<endl;
				i+=1;
			}
		}else {
			cout << "Unrecognized argument |" << argv[i] << "|" << "\n";
			cout << "Refer to README for information on command line arguments"
					<< endl;
			return -1;
		}
	}
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	if (options & op_fullscreen) {
		window = glfwCreateWindow(g_winWidth, g_winHeight,
				"Procedural City Generator", glfwGetPrimaryMonitor(), nullptr);
	} else {
		window = glfwCreateWindow(g_winWidth, g_winHeight,
				"Procedural City Generator", nullptr, nullptr);
	}
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

	/*Setup callback functions*/
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
	glfwSetKeyCallback(window, keyCallback);

	/*Set up depths and perspective*/
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	if((options&op_customseed)){
		srand(user_seed);
	}else{
		srand(time(NULL));
	}

	if(!(options&op_customsize)){
		city_size = rand()%3;
	}

	if (mode == FULL_MODE) {
		initBuildingGenerator();
		initLighting();


		if (!(options & op_heightmap)) {
			cout << "No heightmap, generating buildings now" << endl;
			//if we don't want a height map, just generate the buildings
			generateBuildings();
		}

	} else if (mode == SHOWCASE_MODE) {
		initBuildingGenerator();
		initLighting();
		single_list = building.generateSingleBuilding(showcase_mode_type,
				showcase_mode_size);

	} else if (mode == CAR_MODE) {
		cout << "Car mode" << endl;
		glfwSetCursorPosCallback(window, mouseMotionCallbackModelView);
		// Load the vehicle files and textures
		g_vehicleCtrl = new VehicleController(
				"../work/res/assets/vehicle_config.txt",
				"../work/res/assets/tex_config.txt", car_mode_number);
		// Creates road network
		g_network = new RoadNetwork();
		g_network->testNetwork();

		// Parse the road network
		g_vehicleCtrl->parseRoadNetwork(g_network);
		initBuildingGenerator();

	} else if (mode == NETWORK_MODE) {
		cout << "Road mode" << endl;
		g_network = new RoadNetwork();
		if(devianceLevel != -1){
			g_network->setDevianceLevel(devianceLevel);
		}
		g_network->setNetMode();
		g_network->networkModeGen(network_mode_type, network_mode_size, network_mode_cycles);

		//
		if(options&op_experimental){
			while( ((g_network->getCycleSize() <= 40) && (city_size == 1)) || ((g_network->getCycleSize() <= 6) && (city_size == 0)) || ((g_network->getCycleSize() <= 150) && (city_size == 2))){
				delete g_network;
				g_network = new RoadNetwork();
				if(devianceLevel != -1){
					g_network->setDevianceLevel(devianceLevel);
				}
				g_network->setNetMode();
				g_network->networkModeGen(network_mode_type, network_mode_size, network_mode_cycles);
			}
		}


	} else if (mode == SECTION_MODE) {
		cout << "Section mode" << endl;
		g_sections = new SectionDivider();
		g_sections->testSection();

	}

	/*Setup mouse stuff here*/
	if ((options & op_modelview)) { //model view overwrites other types of
		glfwSetCursorPosCallback(window, mouseMotionCallbackModelView);
	} else if (mode == NETWORK_MODE || mode == SECTION_MODE) {
		glfwSetCursorPosCallback(window, mouseMotionCallback2D);
	} else {
		glfwSetCursorPosCallback(window, mouseMotionCallbackFPS);
	}
	glEnable(GL_SMOOTH);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		renderLoop();
	}
	// Delete pointers
	delete g_sections;
	delete g_network;
	delete g_vehicleCtrl;
	glfwTerminate();
}

void renderLoop() {
	/*## Render here ##*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	if (options & op_heightmap) {
		//Spline map mode
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, g_winWidth, 0, g_winHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_PROGRAM_POINT_SIZE);
		glPointSize(10.0f);
		glColor3f(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		for (int i = 2; i < building.heightmap_points.size() - 1; i++) {
			for (float time = 0.0f; time <= 1.0f; time += 0.1f) {
				vec2 splinePoint = Spline::calculatePoint(
						building.heightmap_points[i - 2],
						building.heightmap_points[i - 1],
						building.heightmap_points[i],
						building.heightmap_points[i + 1], time);
				glVertex2f(splinePoint.x, g_winHeight - splinePoint.y);
			}
		}
		glEnd();
	} else if (mode == FULL_MODE) {
		setupCamera();
		initLighting();
		glTranslatef(0, -2, 0);
		/*Draw buildings*/
		for (lot l : g_sections->getLots()) {
			glPushMatrix();
			glCallList(l.buildings.high);
			glPopMatrix();
			building.generateBlock(l.boundingBox, 0.0f);
		}

		/*Draw roads*/
		float scale = 2.0f * Generator::SECTION_TO_POINT_SCALE();
		for (cycle::road r : g_network->getAllRoads()) {
			building.generateRoad(r.start.location * scale,
					r.end.location * scale, 0.2f);
		}

		g_vehicleCtrl->tick();

		/*Draw world*/
		building.drawGround(100.0f);
		drawSkycube(100.0f);	//the further away it is the better it looks

	} else if (mode == SHOWCASE_MODE) {
		setupCamera();
		initLighting();
		glTranslatef(0, -1, 0);

		glPushMatrix();
		glRotatef(showcase_mode_angle, 0, 1, 0);
		glCallList(single_list);
		glPopMatrix();
		if (showcase_mode_angle > 0) {
			showcase_mode_angle++;
			showcase_mode_angle %= 360;
		}
		building.drawGround(10.0f);
		drawSkycube(100.0f);

	} else if (mode == NETWORK_MODE) {
		glClearColor(0.0, 0.0, 0.0, 0.0); //Set the cleared screen colour to black
		glViewport(0, 0, g_winWidth, g_winHeight); //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, g_winWidth, 0, g_winHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPushMatrix();
		glScalef(zoom, zoom, 0);
		glTranslatef(p_pos.x, p_pos.y, 0);
		g_network->renderRoads();
		glPopMatrix();

	} else if (mode == SECTION_MODE) {

		glClearColor(0.0, 0.0, 0.0, 0.0); //Set the cleared screen colour to black
		glViewport(0, 0, g_winWidth, g_winHeight); //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, g_winWidth, 0, g_winHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		g_sections->renderTest();

	} else if (mode == CAR_MODE) {
		setupCamera();
		initLighting();
		glTranslatef(-20, -2, 20);
		// drawGrid(40, 1);
		// Render the vehicles
		vector<cycle::road> roads = g_network->getAllRoads();
		for (int i = 0; i < (int) roads.size(); ++i) {
			building.generateRoad(
					roads[i].start.location
					* Generator::SECTION_TO_POINT_SCALE(),
					roads[i].end.location * Generator::SECTION_TO_POINT_SCALE(),
					0.5f);
		}

		g_vehicleCtrl->tick();
		// g_vehicleCtrl->testRender();
	}
	/* Swap front and back buffers */
	glfwSwapBuffers(window);
	/* Poll for and process events */
	glfwPollEvents();
	if (options & op_joystick)
		joystickEventsPoll();
}

/*Generates the roads and finds the cycles, creates lots,
 *assigns buildings to the loads, and stores them in the
 *section code.*/
void generateBuildings() {

	cout << "Generating network.." << endl;
	g_network = new RoadNetwork();
	if(devianceLevel != -1){
		g_network->setDevianceLevel(devianceLevel);
	}

	g_network->createRoads(options&op_experimental,city_size);

	if(options&op_experimental){
		while( ((g_network->getCycleSize() <= 40) && (city_size == 1)) || ((g_network->getCycleSize() <= 6) && (city_size == 0)) || ((g_network->getCycleSize() <= 150) && (city_size == 2))){
			delete g_network;
			g_network = new RoadNetwork();
			if(devianceLevel != -1){
				g_network->setDevianceLevel(devianceLevel);
			}
			g_network->createRoads(options&op_experimental,city_size);
		}
	}

	// Finds lot outlines
	vector<util::section> lotOutlines;
	for (cycle::primitive prim : g_network->getCycles()) {
		vector<vec2> points;
		for (cycle::roadNode rn : prim.vertices) {
			points.push_back(rn.location);
		}
		lotOutlines.push_back(Generator::pointsToSections(points));
	}

	cout << "Lot outlines :" << lotOutlines.size() << endl;
	cout << "Dividing Sections.." << endl;
	// Divides sections
	g_sections = new SectionDivider();
	g_sections->divideAllLots(lotOutlines);
	cout << "Generating buildings.." << endl;
	// Generate building display list
	vector<lot> allLots = g_sections->getLots();
	vec2 min = vec2(10000,10000);
	vec2 max = vec2(0,0);
	vec2 zero = vec2(0,0);
	for(lot l: allLots){

		vector<vec2> bounds = Generator::getBoundingBox(Generator::sectionToPoints(l.boundingBox));
		if(distance(bounds[0],zero)<distance(min,zero)){
			min = bounds[0];
		}
		if(distance(bounds[1],zero)>distance(max,zero)){
			max = bounds[1];
		}
	}
	float range = distance(max,min);


	for(lot l: allLots){
		srand(rand());
		l.buildings.high = building.generateBuildingsFromSections(l.sections,range,min);
		g_sections->addBuildingToLot(l);
	}
	g_vehicleCtrl = new VehicleController(
			"../work/res/assets/vehicle_config.txt",
			"../work/res/assets/tex_config.txt", car_mode_number);

	g_vehicleCtrl->parseRoadNetwork(g_network);
	cout << "Done! " << g_sections->getLots().size() << " lots created" << " from  "<< g_network->getCycleSize() << " cycles "<< endl;
}

/*Inits the building class, loads the shaders, and sets up the heightmap*/
void initBuildingGenerator() {
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

	/*Initialize shader and heightmap points*/
	initSkybox("../work/res/textures/cubeMap.jpg");
	skybox_shader = makeShaderProgram("../work/res/shaders/skybox_shader.vert",
			"../work/res/shaders/skybox_shader.frag");
	building.heightmap_points.push_back(vec2(0, g_winHeight / 2));
	building.heightmap_points.push_back(vec2(0, g_winHeight / 2));
	building.heightmap_points.push_back(vec2(g_winWidth, g_winHeight / 2));
	building.heightmap_points.push_back(vec2(g_winWidth, g_winHeight / 2));
	building.initTexture();
}

void initLighting() {
	float ambient_full[] ={ 0.99f, 0.99f, 0.99f, 1.0f };
	float ambient[]= { 0.2f, 0.2f, 0.2f, 1.0f };

	float direction[] = { 0.0f, -0.5f, -0.5f, 0.0f };
	float diffintensity[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	float specular[] = { 1, 1, 1, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	if(options&op_fullbright){
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_full);
	}else{
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	}

	glEnable(GL_LIGHT0);

	float light_ambient[] = { triggerDiff / 2.0f, triggerDiff / 2.0f,
			triggerDiff / 2.0f, 1.0f };
	float light_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float light_position[] = { p_pos.x, p_pos.y + 2, p_pos.z, 1.0f };
	vec3 difference = (vec3(0, 0, 0)
			- vec3(light_position[0], light_position[1], light_position[2]));
	float spotlight_direction[] = { p_dir.x, p_dir.y, p_dir.z, 0.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, (2.1f) - triggerDiff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 200);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlight_direction);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
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

/*Sets up the camera for 3D and supports model view or FPS*/
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	perspectiveGL(20.0, float(g_winWidth) / float(g_winHeight), 0.1f, 1000.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	perspectiveGL(45.0, float(g_winWidth) / float(g_winHeight), 0.1f, 1000.0f);
	if (options & op_modelview) {
		//if using model view
		glTranslatef(0, 0, -50 * zoom);
		glRotatef(-rotation.y, 1.0f, 0.0f, 0.0f);
		glRotatef(-rotation.x, 0.0f, 1.0f, 0.0f);
	} else {
		//if using FPS control
		lookAt(p_pos, p_pos + p_front, p_up);
	}
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
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	cout << "Key: " << key << endl;
	if((options&op_heightmap) && key == 257&&action){
		generateBuildings();
		options = options^op_heightmap;//disable heightmap
		cout<<"Disabling heightmap, entering render mode "<<(options&op_modelview)<<endl;
		if(options&op_joystick){
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		if((options&op_modelview)){
			glfwSetCursorPosCallback(window, mouseMotionCallbackModelView);
			cout<<"Setting modelview "<<endl;
		}else{
			glfwSetCursorPosCallback(window, mouseMotionCallbackFPS);
		}
	}else if(mode == SHOWCASE_MODE && key == 257&&action){
		showcase_mode_angle = 1;
	} else if (key == 87 && action) {
		p_pos += 0.05f * p_front * 1;
	} else if (key == 65 && action) {
		vec3 p_right = cross(p_up, p_front);
		p_pos += 0.05f * p_right * 1;
	} else if (key == 68 && action) {
		vec3 p_right = cross(p_up, p_front);
		p_pos -= 0.05f * p_right * 1;
	} else if (key == 63 && action) {
		p_pos -= 0.05f * p_front * 1;
	}else if(mode == SECTION_MODE && key == 78 && action){
		g_sections->testSection();
	}else if(mode == SECTION_MODE && key == 83 && action){
		g_sections->changeFill();
	}


}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	//cout << button << " " << action << " " << mods << endl;
	if ((options & op_heightmap) && action && button == GLFW_MOUSE_BUTTON_1) {
		if (building.heightmap_points.size() < 2) {
			building.heightmap_points.push_back(m_pos);
			return;
		}
		float x = building.heightmap_points[1].x;//set first point for comparison ([0] is 0,0)
		int i = 2;
		while (x < m_pos.x && i <= building.heightmap_points.size()) {
			x = building.heightmap_points[i].x;
			++i;
		}
		i--;
		building.heightmap_points.insert(building.heightmap_points.begin() + i,
				m_pos);
		return;
	}
	if (button == GLFW_MOUSE_BUTTON_1) {
		m_LeftButton = action;
	} /*else if (button == GLFW_MOUSE_BUTTON_2 && action) {
		string input;
		cout << "Enter an input seed: ";
		cin >> input;
		//single_list = building.generateBuildingFromString(input);
		single_list = building.generateBuildingsFromSections(input,
				g_sections->testSection().sections);
		mode = 0;
		glfwSetCursorPosCallback(window, mouseMotionCallbackModelView);
	}*/
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
bool c_xbox_button_down = false;
void joystickEventsPoll() {
	int axes_count;
	int button_count;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1,
			&button_count);
	//LINUX
	//0,1-Left Stick 2 is trigger (postive left, negative, right) 3,4 is Right Stick y,x
	/* 0/1- Left Stick
	 * 2  - Left Trigger
	 * 3/4- Right Stick
	 * 5  - RightTrigger
	 * 6/7- D-pad
	 * */
	if (axes_count > 0) {
		/* OSX
			0 - L-S X
			1 - L-S Y
			2 - R-S X
			3 - R-S Y


		*/
		c_LSpos = vec2((float) ((int) (axes[0] * 100)) / 100.0f,
				(float) ((int) (axes[1] * 100)) / 100.0f);
		vec2 c_RSpos = vec2((float) ((int) (axes[3] * 100)) / 100.0f,
				(float) ((int) (axes[2] * 100)) / 100.0f);

		float trigger = axes[4];
		triggerDiff = axes[5] + 1;
		float threshold = 0.2f;	//stops drifting when the user isn't touching the controller
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

		p_pos -= 0.05f * p_front * c_LSpos.y * (2 + trigger);

		vec3 p_right = cross(p_up, p_front);
		p_pos -= 0.05f * p_right * c_LSpos.x * (2 + trigger);

	}
	/* 0 - A
	 * 1 - B
	 * 2 - X
	 * 3 - Y
	 * 4 - LB
	 * 5 - RB
	 * 6 - Back
	 * 7 - Start
	 * 8 - Xbox button
	 * 9 - Left Stick Press
	 * 10 - Right Stick Press
	 */
	if (button_count > 0) {
		/*if (buttons[8] == GLFW_PRESS && !c_xbox_button_down) {

			c_xbox_button_down = true;
			//If Xbox button pressed
			single_list = building.generateBuildingsFromSections("XBOX Button",
					g_sections->testSection().sections);
			cout << "Regenerating city" << endl;
		} else if (buttons[8] == GLFW_RELEASE) {
			c_xbox_button_down = false;
		}*/
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
