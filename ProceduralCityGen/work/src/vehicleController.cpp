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
		int size) {

	m_size = size;
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
//	delete[] m_textures;
}

void VehicleController::parseRoadNetwork(RoadNetwork *network) {
	m_network = network;
	m_aStar = new AStarSearchAlgo(m_network->getAllNodes(),
			m_network->getAdjacencyList());
	m_spline = new Spline();
	initVehicles();
}

void VehicleController::initTexture(string filename, GLuint index) {

	image tex(filename);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_textures[index]); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, m_textures[index]); // Bind it as a 2D texture

	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finally, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(),
	GL_UNSIGNED_BYTE, tex.dataPointer());

	cout << tex.w << endl;
}

void VehicleController::initVehicles() {

	// TODO will have to change this according to the size of the map
	int size = m_size, index;

	for (int i = 0; i < size; i++) {
		index = rand() % m_filenames_car.size();
		srand(rand());
		m_vehicles.push_back(Vehicle(m_filenames_car[index], i));

		// Set a texture
		index = rand() % m_filenames_tex.size() + 1;
		srand(rand());
		m_vehicles[i].setTexture(index);

		// Set a random starting position
		vector<roadNode> goals = m_network->getAllNodes();
		srand(rand());
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

		// Generate a goal for the vehicle
		if (reachedGoal(&m_vehicles[i])) {

			// Generate a goal
			roadNode goal = generateGoal(&m_vehicles[i]);
			roadNode start = m_vehicles[i].getStartPos();

			// Generate path
			m_aStar->aStarSearch(&start, &goal);
			// cout << "Found path" << endl;

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
			m_vehicles[i].addPath(m_aStar->getPath(&goal));

			// Set goal
			m_vehicles[i].setGoal(
					m_vehicles[i].getPath()[(int) m_vehicles[i].getPath().size()
							- 2]);

			// Set the starting position to be the previous goal
//			m_vehicles[i].setStartPos(
//					m_vehicles[i].getPath()[(int) m_vehicles[i].getPath().size()
//							- 1]);
			m_vehicles[i].setStartPos(goal);

			// Skip the first target
			m_vehicles[i].nextTarget();

			m_vehicles[i].resetSplineTime();
		}

		roadNode target = m_vehicles[i].getCurrentTarget();

		if (m_vehicles[i].getSplineTime() >= 1) {

			// cout << "Target reached" << endl;

			// Change the target
			m_vehicles[i].nextTarget();
			m_vehicles[i].resetSplineTime();
		}

		// renderTargets(&m_vehicles[i]);

		// Calculate transformation
		vec3 trans = interpolate(&m_vehicles[i], m_vehicles[i].getPathIndex());

		vec3 rot;
		if (length(trans) == length(m_vehicles[i].getPos()))
			rot = m_vehicles[i].getRot();
		else
			rot = yaw(&m_vehicles[i], &trans);

		vec3 scale = vec3(0.08);

		renderVehicle(&m_vehicles[i], &trans, &rot, &scale,
				m_vehicles[i].getTexture());
	}
}

/*
 * Render the vehicles path (Debugging)
 */
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

/*
 * Render a vehicle given a transformation and texture
 */
void VehicleController::renderVehicle(Vehicle *vehicle, vec3 *trans, vec3 *rot,
		vec3 *scale, int texture) {

	// cout << "Rendering at " << *trans << ", " << rot->y << endl;

	vehicle->setPos(*trans);
	vehicle->setRot(*rot);

	// cout << "Texture " << texture << endl;

	// -1 means no texture
	if (texture > 0) {

		// Enable Drawing texures
		glEnable(GL_TEXTURE_2D);

		// Use Texture as the color
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// Set the location for binding the texture
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, m_textures[texture]);
	}

	// Render the vehicle
	glPushMatrix();

	float world_scale = 2.0f;

	// Translate it to grid coordinates
	glTranslatef(trans->x * Generator::SECTION_TO_POINT_SCALE() * world_scale,
			trans->y * Generator::SECTION_TO_POINT_SCALE() * world_scale,
			trans->z * Generator::SECTION_TO_POINT_SCALE() * world_scale);

	// Rotate vehicle in the direction it is facing
	glRotatef(rot->y, 0, 1, 0);

	// Shift the vehicle to the left side of the road
	glTranslatef(0, 0, world_scale * -0.2);
	glScalef(scale->x, scale->y, scale->z);
	vehicle->renderVehicle();
	glPopMatrix();

	// Clean up
	glBindTexture(GL_TEXTURE_2D, 0);
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
	const float pointDist = 0.9;
	vec2 turn1 = targ1 + ((length * pointDist) * normalize(targ2 - targ1));

	length = distance(targ2, targ3);
	vec2 turn2 = targ2 + ((length * pointDist) * normalize(targ3 - targ2));

	// Calculate translation
	vec2 translate = Spline::calculatePoint(targ1, turn1, turn2, targ3,
			v->getSplineTime());

	if (canTravel(v)) {
		v->incrementSplineTime();
		// cout << "Spline time " << v->getSplineTime() << endl;
		return vec3(translate.x, 0, translate.y);
	}

	return v->getPos();
}

/*
 * Calculate the angle between two points (y axis)
 */
vec3 VehicleController::yaw(Vehicle *v, vec3 *newPos) {
	vec3 oldPos = v->getPos();
	float angle = atan2(newPos->z - oldPos.z, newPos->x - oldPos.x);
	return vec3(0, degrees(-angle), 0);
}
/**
 * Has the vehicle reached it's goal?
 */
bool VehicleController::reachedGoal(Vehicle* vehicle) {

	// If a goal has not been set
	if (vehicle->getGoal().location.x == -1)
		return true;

	return vehicle->getPathIndex() == (int) vehicle->getPath().size() - 1;
}

roadNode VehicleController::generateGoal(Vehicle* vehicle) {

	// TODO make the goal be in the direction of the player

	// Vector of all of the possible goals
	vector<roadNode> goals = m_network->getAllNodes();

	int randomNumber = rand() % goals.size();
	srand(rand());
	roadNode goal = goals[randomNumber];

	if (vehicle->getGoal().location.x == -1)
		return goal;

	// vec3 controlPoint = vehicle->getPath()[vehicle->getPath().size() - 1];
	// vec3 rot = yaw(vehicle, &controlPoint);

//	// If direction is to the right
//	if (0 < rot.y && rot.y < 180) {
//
//	}
//	// If the direction is to the left
//	else if (-180 < rot.y && rot.y < 0) {
//
//	}

	return goal;
}

/*
 * Has the vehicle reached it's target?
 */
bool VehicleController::reachedTarget(Vehicle *vehicle) {
	return vehicle->getSplineTime() >= 1;
}

/*
 * Collision detection
 */
bool VehicleController::canTravel(Vehicle *v) {

	for (int i = 0; i < (int) m_vehicles.size(); ++i) {

//		// We don't want to check itself
//		if (v->getId() != i) {
//
//			// Find if they are in the same direction
////			if (length(cross(v->getPos(), m_vehicles[i].getPos())) == 0) {
//
//				// Calculate the distance between the vehicles and its neigbours
//				float dist = distance(v->getPos(), m_vehicles[i].getPos());
//
//				cerr << v->getId() << " and " << i << " are colliding" << "	Distance " << dist << endl;
//
//				// If the vehicle is too close to another one stop it.
////				if (length(v->getPos()) < length(m_vehicles[i].getPos())
////						&& dist <= 10) {srand(rand());
//
//				// Find the direction vector of v
//				vec2 target = v->getPath()[v->getPathIndex()].location;
//				vec2 dir = normalize(vec2(target.x - v->getPos().x, target.y - v->getPos().z));
//
//				// Find which vehicle is in front
//				vec3 diff3d = v->getPos() - m_vehicles[i].getPos();
//				vec2 diff = vec2(diff3d.x,diff3d.z);
//				float front = dot(dir, normalize(diff));
//
//				cout << "Current Vehicle " << v->getPos() << endl;
//				cout << "Other vehicle " << m_vehicles[i].getPos() << endl;
//				cout << "DIR  " << dir << endl;
//				cout << "DIFF " << diff << endl;
//				cout << "FRONT " << front << endl;
//				if (dist <= 0.9f && front > 0.99f) {
//					cerr << "Oi, get off my ass cunt " << endl;
//					// return false;
//				}
////			}

		// If they are about to intersect and not traveling in the same direction
		if (!v->getPath().empty() && !m_vehicles[i].getPath().empty()) {

			// Current target of current vehicle
			roadNode targ1 = v->getPath()[v->getPathIndex()];

			// Current target of other vehicle
			roadNode targ2 =
					m_vehicles[i].getPath()[m_vehicles[i].getPathIndex()];

			// Previous target of other vehicle
			roadNode targ3 =
					m_vehicles[i].getPath()[m_vehicles[i].getPathIndex() - 1];
			if (targ1.ID == targ2.ID || targ1.ID == targ3.ID) {

				cout << "Vehicle " << v->getId() << " is at: " << v->getPos()
						<< endl;
				cout << "Vehicle " << m_vehicles[i].getId() << " is at: "
						<< m_vehicles[i].getPos() << endl;

				// Calculate the distance between the vehicles and its neigbours
				float dist = distance(v->getPos(), m_vehicles[i].getPos());
				cout << "	Distance " << dist << endl;

				if (dist == 0)
					cerr << "Holy shit holy shit bad things" << endl;

				// Find the direction vector of v
				vec2 target = v->getPath()[v->getPathIndex()].location;
				vec2 dir = normalize(
						vec2(target.x - v->getPos().x,
								target.y - v->getPos().z));

				// Find which vehicle is in front
				vec3 diff3d = v->getPos() - m_vehicles[i].getPos();
				vec2 diff = vec2(diff3d.x, diff3d.z);
				float front = dot(dir, normalize(diff));

				// If the vehicle is too close to another one stop it.
				if (length(v->getPos()) > length(m_vehicles[i].getPos())
						&& dist <= 30) {
					cerr << "Too close tumeke " << endl;
					return false;
				}
			}
		}
// 	}
	}
	return true;
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

	for (string s : m_filenames_tex)
		cout << s << endl;

	m_textures = new GLuint[m_filenames_tex.size() + 1];

	for (int i = 0; i < (int) m_filenames_tex.size(); i++)
		initTexture(m_filenames_tex[i], i + 1);

	cout << "Finished reading texture files" << endl;
}
