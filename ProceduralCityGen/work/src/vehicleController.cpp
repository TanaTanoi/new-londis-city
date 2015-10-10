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
//#include <unistd.h> ??

#include "comp308.hpp"
#include "vehicle.hpp"
#include "vehicleController.hpp"
#include "imageLoader.hpp"
#include "roadNetwork.hpp"

using namespace std;
using namespace comp308;

const float ACCEL = 0.5;
time_t previous_time;

VehicleController::VehicleController(string vehicles, string textures,
		vector<vec3> limits, vec3 bounds) {

	readConfig(vehicles);
	readTextures(textures);
	initVehicles();
}

VehicleController::~VehicleController() {
	delete m_network;
}

void VehicleController::parseRoadNetwork(RoadNetwork *network) {
	m_network = network;
}

void VehicleController::initVehicles() {

	// TODO will have to change this according to the size of the map
	int size = 1, index;

	for (int i = 0; i < size; i++) {
		index = rand() % m_filenames_car.size();
		m_vehicles.push_back(Vehicle(m_filenames_car[index]));
	}

}

void VehicleController::initTexture(string filename, int index) {

	/**
	 image tex(filename);

	 glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	 glGenTextures(1, &m_textures[index]); // Generate texture ID
	 glBindTexture(GL_TEXTURE_2D, m_textures[index]); // Bind it as a 2D texture

	 // Setup sampling strategies
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	 GL_LINEAR_MIPMAP_LINEAR);
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	 // Finnaly, actually fill the data into our texture
	 gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(),
	 GL_UNSIGNED_BYTE, tex.dataPointer());

	 cout << tex.w << endl;
	 **/
}

// Move a vehicle from one intersection to another.
//	- It should accelerate towards the intersection then
// decelerate to a stop when it arrives at the intersection
void VehicleController::tick() {
	for (Vehicle v : m_vehicles) {

		vec3 rot = v.getRot();

		// Current position that the vehicle is at
		vec3 start = v.getPos();

		// The goal that the vehicle is heading towards
		vec3 target = findTarget(&v);

		// If the vehicle is traveling straight
		// TODO make a condition to check if the vehicle needs to turn
		interpolate_straight(&v, &start, &target);

		// Finally render the vehicle
		renderVehicle(&v, start, rot, vec3(0.1, 0.1, 0.1), -1);
	}
}

void VehicleController::renderVehicle(Vehicle* vehicle, vec3 translate,
		vec3 rotate, vec3 scale, int texture) {

	// Set transformations
	vehicle->setPos(translate);
	vehicle->setRot(rotate);

	// Render the vehicle
	glPushMatrix();
	glTranslatef(translate.x, translate.y, translate.z);
	glRotatef(rotate.x, rotate.y, rotate.z, 1);
	vehicle->renderVehicle();
	glPopMatrix();
}

// Find a target for a vehicle
vec3 VehicleController::findTarget(Vehicle *vehicle) {

	// Find the road the vehicle is on
	road closest = m_network->findClosestRoads(vehicle->getPos());

	vec3 currentPos = vehicle->getPos();

	// Road start and end
	vec3 start = vec3(closest.start.location.x, 0, closest.start.location.y);
	vec3 end = vec3(closest.end.location.x, 0, closest.end.location.y);

	// Find the vector that it is heading to
	switch (vehicle->getDirection()) {

	case NORTH:
		if (end.y > currentPos.z)
			return end;
		else
			return start;

	case EAST:
		if (end.x > currentPos.x)
			return end;
		return start;

	case SOUTH:
		if (end.y < currentPos.z)
			return end;
		return start;

	case WEST:
		if (end.x < currentPos.x)
			return end;
		return start;
	}

	return currentPos;
}

/**
 * Calculate the stopping distance so a vehicle can deccelerate
 */
float VehicleController::calculateStoppingDistance(Vehicle* vehicle, vec3* target) {
	return -1;
}

// Find the distance to the nearest vehicle to a given vehicle
//
float VehicleController::disToNextVehicle(Vehicle* from) {

	float min = -1;

	for (Vehicle v : m_vehicles) {
		if (v.getDirection() == from->getDirection()) {

			float distance = 100000000000;

			switch (from->getDirection()) {
			case NORTH:
			case SOUTH:
				distance = abs(v.getPos().z - from->getPos().z);
				break;
			case EAST:
			case WEST:
				distance = abs(v.getPos().x - from->getPos().x);
				break;
			default:
				break;
			}

			// If the distance calculated is less than the original min, replace it
			min = (distance < min) ? distance : min;
		}
	}

	return min;
}

/**
 * Check the corresponding roads at an intersection to see if they're occupied
 */
intersection VehicleController::checkIntersections(vec3* position) {
	// TODO: implement this
	intersection sect;
	return sect;
}

/**
 * Determine the turn to take when an intersection is reached.
 */
Direction VehicleController::turnToTake(Vehicle* vehicle) {

	vec3 currentPos = vehicle->getPos();
	vec3 goal = vehicle->getGoal();

	// Calculate the angle between the two vectors
	// cosθ = (u⃗ · v⃗) / (||u⃗|| · ||v ⃗||)

	float angle = (dot(currentPos, goal) / (length(currentPos) * length(goal)));
	angle = acos(angle);

	angle = degrees(angle);

	// Calculate the turn to take
	switch (vehicle->getDirection()) {

	case NORTH:
		return calculateTurn(NORTH, EAST, SOUTH, WEST, angle);

	case EAST:
		return calculateTurn(EAST, SOUTH, WEST, NORTH, angle);

	case SOUTH:
		return calculateTurn(SOUTH, WEST, NORTH, EAST, angle);

	case WEST:
		return calculateTurn(WEST, NORTH, EAST, SOUTH, angle);
	}

	return vehicle->getDirection();
}

/**
 * One Turn Zones (315 - 45, 45 - 135, 135 - 225, 225 - 315) Vehicle can choose one direction to turn.
 * Two Turn Zones (0 - 90, 90 - 180, 180 - 270, 270 - 360) Vehicle can choose two directions to turn.
 */
Direction VehicleController::calculateTurn(Direction dir1, Direction dir2,
		Direction dir3, Direction dir4, float angle) {

	int r = ((double) rand() / (RAND_MAX)) + 1;

	// If goal is in a one turn zone
	if (angle > 315 || (0 < angle && angle < 45))
		return dir1;
	else if (45 < angle && angle < 135)
		return dir2;
	else if (135 < angle && angle < 225)
		return dir3;
	else if (225 < angle && angle < 315)
		return dir4;

	// If the goal is in a two turn zone choose a random turn
	else if (0 < angle && angle < 90) {
		if (r == 1)
			return dir1;
		else
			return dir2;
	} else if (90 < angle && angle < 180) {
		if (r == 1)
			return dir2;
		else
			return dir3;
	} else if (180 < angle && angle < 270) {
		if (r == 1)
			return dir3;
		else
			return dir4;
	} else if (270 < angle && angle < 360) {
		if (r == 1)
			return dir4;
		else
			return dir1;
	}

	return dir1;
}
/**
 * Interpolate a vehicle between a start point and an end point.
 * TODO slow the car down when it reaches the end of the road.
 */
void VehicleController::interpolate_straight(Vehicle *vehicle, vec3 *from,
		vec3 *goal) {

	time_t now = time(0);
	float currentTime = (float) now;
	float previousTime = (float) previous_time;

	// Calculate change in time
	float delta_time = currentTime - previousTime;

	// Calculating distance traveled
	float v_f = vehicle->getVelocity() + (ACCEL * delta_time);
	float distance = (v_f + vehicle->getVelocity() / 2) * delta_time;
	vehicle->setVelocity(v_f);

	// Apply transformation
	vec3 new_pos;
	switch (vehicle->getDirection()) {
	case NORTH:
	case SOUTH:
		new_pos = vec3(0, 0, distance);
		break;
	case EAST:
	case WEST:
		new_pos = vec3(distance, 0, 0);
		break;
	default:
		new_pos = vec3();
		break;
	}

	vehicle->translatePos(new_pos);
}

void VehicleController::interpolate_curve(Vehicle* vehicle, vec3* from,
		vec3* goal) {

	// cout << "RENDERING SPLINE..." << endl;
	// cout << numPts << endl;

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

	for (int i = 0; i < m_filenames_tex.size(); i++)
		initTexture(m_filenames_tex[i], i + 1);

	cout << "Finished reading texture files" << endl;
}

