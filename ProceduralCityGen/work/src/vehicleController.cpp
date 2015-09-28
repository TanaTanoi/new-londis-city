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
#include "imageLoader.hpp"

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
void VehicleController::renderVehicles() {
	for (Vehicle v : m_vehicles) {

		// Transforamtions
		vec3 trans = v.getPos();
		vec3 rot = v.getRot();

		// TODO translate vehicle, don't worry about collision detection
		// NEED nearest intersection as vec3

		// TODO rotate IFF turning
		renderVehicle(&v, vec3(), vec3(), vec3(0.1, 0.1, 0.1), -1);
	}
}

void VehicleController::renderVehicle(Vehicle* vehicle, vec3 translate,
		vec3 rotate, vec3 scale, int texture) {

	vehicle->setPos(translate);
	vehicle->setRot(rotate);

	glPushMatrix();
	glTranslatef(translate.x, translate.y, translate.z);
	glRotatef(rotate.x, rotate.y, rotate.z, 1);
	vehicle->renderVehicle();
	glPopMatrix();
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

void VehicleController::interpolate(comp308::vec3* from, comp308::vec3* goal) {

	time_t now = time(0);
	float currentTime = (float) now;
	float previousTime = (float) previous_time;


	// Calculate change in distance according to speed and acceleration
}

void VehicleController::cleanUp() {

}
