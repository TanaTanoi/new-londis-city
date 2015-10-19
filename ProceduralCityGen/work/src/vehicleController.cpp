/*
 * vehicleController.cpp
 *
 *  Created on: 19/09/2015
 *      Author: bryerscame
 */
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <map>
#include <vector>
#include <ctime>
//#include <unistd.h> ??

#include "comp308.hpp"
#include "vehicle.hpp"
#include "vehicleController.hpp"
#include "imageLoader.hpp"
#include "roadNetwork.hpp"
#include "cycleUtil.hpp"
#include "generator.h"
#include "aStarSearchAlgo.hpp"
#include "spline.hpp"

using namespace std;
using namespace comp308;
using namespace cycle;

const float ACCEL = 0.5;

// Scaling hannah's roads
const float SECTION_POINT_TO_SCALE = Generator::SECTION_TO_POINT_SCALE();

const comp308::vec3 scale = vec3(0.1);

// Amount of error when checking if a vehicle has reached the goal
const float GIVE = 10;
time_t previous_time;

clock_t begin_time = clock();

void renderTargets(Vehicle *);

VehicleController::VehicleController(string vehicles, string textures,
		vector<vec3> limits, vec3 bounds) {

	readConfig(vehicles);
	readTextures(textures);
}

VehicleController::~VehicleController() {

	// XXX: Some reason it does not like this, maybe because
	// this class itself is being destroyed? I don't know.

	//	if (m_network)
	//		delete m_network;
	//	if (m_aStar)
	//		delete m_aStar;
	//
	m_network = NULL;
	m_aStar = NULL;
	m_spline = NULL;
}

void VehicleController::parseRoadNetwork(RoadNetwork *network) {
	m_network = network;
	m_aStar = new AStarSearchAlgo(m_network->getAllNodes(),
			m_network->getAdjacencyList());
	m_spline = new Spline();
	initVehicles();
}

void VehicleController::initVehicles() {

	// TODO will have to change this according to the size of the map
	int size = 10, index;

	for (int i = 0; i < size; i++) {
		index = rand() % m_filenames_car.size();
		m_vehicles.push_back(Vehicle(m_filenames_car[index]));

		// Set a random starting position
		vector<roadNode> goals = m_network->getAllNodes();
		int randomNumber = rand() % goals.size();

		roadNode start = goals[randomNumber];
		m_vehicles[i].setStartPos(start);

		cout << "Starting position " << m_vehicles[i].getStartPos().location
				<< endl;
		m_vehicles[i].setPos(vec3(start.location.x, 0, start.location.y));
		m_vehicles[i].setPosPrevious(
				vec3(start.location.x, 0, start.location.y));
		// cout << m_vehicles[i].getDirection() << endl;
	}
}

void VehicleController::tick() {

	for (int i = 0; i < (int) m_vehicles.size(); ++i) {

		roadNode node = { vec2(), 0, true };
		cout << node.ID << ", " << node.location << ", " << node.visited
				<< endl;

		// Generate a goal for the vehicle
		if (reachedGoal(&m_vehicles[i])) {

			// Generate a goal
			roadNode goal = generateGoal(&m_vehicles[i]);
			roadNode start = m_vehicles[i].getStartPos();

			// Generate path
			m_aStar->aStarSearch(&start, &goal);
			cout << "Found path" << endl;

			// Path must be >= 3 otherwise the program won't work
			while ((int) m_aStar->getPath(&goal).size() < 3) {

				cerr << "Path is size three!!" << endl;

				goal = generateGoal(&m_vehicles[i]);

				// Generate path
				m_aStar->aStarSearch(&start, &goal);
				cout << "Found path" << endl;
			}

			// XXX: first and last points are skipped as they are control points

			// Update the path
			m_vehicles[i].setPath(m_aStar->getPath(&goal));

			// Set goal
			m_vehicles[i].setGoal(
					m_vehicles[i].getPath()[(int) m_vehicles[i].getPath().size()
							- 2]);

			// Set the starting position to be the previous goal
			m_vehicles[i].setStartPos(m_vehicles[i].getGoal());

			// Skip the first target
			m_vehicles[i].nextTarget();
		}

		roadNode target = m_vehicles[i].getCurrentTarget();

		if (m_vehicles[i].getSplineTime() >= 1) {

			cout << "Target reached" << endl;

			// Change the target
			m_vehicles[i].nextTarget();
			m_vehicles[i].resetSplineTime();
		}

		renderTargets(&m_vehicles[i]);

		// Calculate transformation
		vec3 trans = interpolate(&m_vehicles[i], m_vehicles[i].getPathIndex());
		vec3 rot = vec3(0);
		vec3 scale = vec3(0.1);

		renderVehicle(&m_vehicles[i], &trans, &rot, &scale);
	}
}

void renderTargets(Vehicle *v) {

	// Render the vehicles' target as a point
	glPushMatrix();
	glPointSize(10);
	glTranslatef(0, 0.1, 0);
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.1, 0.0);
	roadNode t;

	for (int j = 0; j < (int) v->getPath().size(); ++j) {
		t = v->getPath()[j];
		// cout << "target " << t.ID << ", " << t.location << endl;
		glVertex3f(t.location.x * Generator::SECTION_TO_POINT_SCALE(), 0,
				t.location.y * Generator::SECTION_TO_POINT_SCALE());
	}
	glEnd();
	glPopMatrix();
}

void VehicleController::renderVehicle(Vehicle *vehicle, vec3 *trans, vec3 *rot,
		vec3 *scale, int texture) {

	cout << "Rendering at " << *trans << endl;

	// Render the vehicle
	glPushMatrix();
	glTranslatef(trans->x * Generator::SECTION_TO_POINT_SCALE(),
			trans->y * Generator::SECTION_TO_POINT_SCALE(),
			trans->z * Generator::SECTION_TO_POINT_SCALE());
	glScalef(scale->x, scale->y, scale->z);
	vehicle->renderVehicle();
	glPopMatrix();

	// Render the vehicles' position as a point
//	glPushMatrix();
//	// glRotatef(rotate.y, 0, 1, 0);
//	glTranslatef(0, 0.2, 0);
//	glPointSize(20);
//	glColor3f(1.0, 0.0, 0.0);
//	glBegin(GL_POINTS);
//	glVertex3f(trans->x * Generator::SECTION_TO_POINT_SCALE(),
//			trans->y * Generator::SECTION_TO_POINT_SCALE(),
//			trans->z * Generator::SECTION_TO_POINT_SCALE());
//	glEnd();
//	glPopMatrix();
}

vec3 VehicleController::interpolate(Vehicle *v, int pathIndex) {

	roadNode prev = v->getPath()[pathIndex - 1];
	roadNode cur = v->getPath()[pathIndex];
	roadNode next = v->getPath()[pathIndex + 1];

	return interpolate(v, &prev, &cur, &next);
}
vec3 VehicleController::interpolate(Vehicle *v, roadNode *prev, roadNode *cur,
		roadNode *next) {

	// Target locations
	vec2 targ1 = prev->location;
	vec2 targ2 = cur->location;
	vec2 targ3 = next->location;

	// Turning points
	float length = distance(targ1, targ2);
	vec2 turn1 = targ1 + ((length * 0.9) * normalize(targ2 - targ1));

	length = distance(targ2, targ3);
	vec2 turn2 = targ2 + ((length * 0.9) * normalize(targ3 - targ2));

	// Calculate translation
	vec2 translate = Spline::calculatePoint(targ1, turn1, turn2, targ3,
			v->getSplineTime());

	v->incrementSplineTime();

	cout << "Spline time " << v->getSplineTime() << endl;

	return vec3(translate.x, 0, translate.y);
}
/**
 * Has the vehicle reached it's goal?
 */
bool VehicleController::reachedGoal(Vehicle* vehicle) {

//	// If a goal has not been set
//	if (vehicle->getGoal().location.x == -1)
//		return true;
//
//	roadNode goal = vehicle->getGoal();
//	vec2 currentPos = vec2(vehicle->getPos().x, vehicle->getPos().z);
//
//	util::checkZone check = util::checkZone(goal.location.x / 2.0,
//			goal.location.y / 2.0, GIVE);
//
//	if (floor(currentPos.x) == goal.location.x
//			&& floor(currentPos.y) == goal.location.y) {
//		cout << "Aww gummon " << endl;
//		return true;
//	}
//
//	return check.contains(&currentPos);
	cout << "path index " << vehicle->getPathIndex() << endl;

	// If a goal has not been set
	if (vehicle->getGoal().location.x == -1)
		return true;

	return vehicle->getPathIndex() == (int) vehicle->getPath().size() - 1;
}

roadNode VehicleController::generateGoal(Vehicle* vehicle) {

	// Vector of all of the possible goals
	vector<roadNode> goals = m_network->getAllNodes();

	int randomNumber = rand() % goals.size();
	return goals[randomNumber];
}

/**
 * Has the vehicle reached it's target?
 */
bool VehicleController::reachedTarget(Vehicle *vehicle, roadNode *target) {

	// return vec2(vehicle->getPos().x, 0, vehicle->getPos().x) == target->location;
	vec2 currentPos = vec2(vehicle->getPos().x, vehicle->getPos().z);

	util::checkZone check = util::checkZone(target->location.x / 2.0,
			target->location.y / 2.0, GIVE);

	// cout << "currentPos " << currentPos << endl;
	// cout << "target " << target->location << endl;
	// cout << "currentPos floor " << floor(currentPos.x) << ", "
	// << floor(currentPos.y) << endl;
	if (check.contains(&currentPos))
		cout << "FUCK YOU" << endl;

	if (floor(currentPos.x) == target->location.x
			&& floor(currentPos.y) == target->location.y) {
		cout << "Aww gummon " << endl;
		return true;
	}

	return check.contains(&currentPos);
}

/**
 * Reads the file which contains all of the vehicle models
 */
void VehicleController::readConfig(string filename) {

	ifstream file(filename);
	string line;

	if (!file.is_open()) {
		cerr << "Failed to open file " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}
	while (file.good()) {

		// Read the line
		getline(file, line);

		m_filenames_car.push_back(line);
	}

	cout << "Finished reading vehicle files" << endl;
}

/**
 * Reads the file that contains all of the vehicle textures
 */
void VehicleController::readTextures(string filename) {

	ifstream file(filename);
	string line;

	if (!file.is_open()) {
		cerr << "Failed to open file " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	while (file.good()) {

		// Read the line
		getline(file, line);
		m_filenames_tex.push_back(line);
	}

	m_textures = new GLuint[m_filenames_tex.size()];

	//	for (int i = 0; i < (int)m_filenames_tex.size(); i++)
	//		initTexture(m_filenames_tex[i], i + 1);

	cout << "Finished reading texture files" << endl;
}
