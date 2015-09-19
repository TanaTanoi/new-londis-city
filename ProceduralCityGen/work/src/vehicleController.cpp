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
#include <unistd.h>

#include "comp308.hpp"
#include "vehicle.hpp"
#include "imageLoader.hpp"

using namespace std;
using namespace comp308;

VehicleController::VehicleController(string vehicles, string textures,
		vector<vec3> limits, vec3 bounds) {

	readConfig(vehicles);
	readTextures(textures);
	initVehicles();

}

void VehicleController::initVehicles() {

	// TODO will have to change this according to the size of the map
	int size = 10, index;

	for (int i = 0; i < size; i++) {
		index = rand() % m_filenames_car.size();
		// m_vehicles[i] = Vehicle(index);
	}

}

void VehicleController::initTexture(std::string filename, int index) {

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

void VehicleController::renderVehicles() {

}

void VehicleController::renderVehicle(Vehicle* vehicle, comp308::vec3 translate,
		comp308::vec3 rotate, int texture) {

}

void VehicleController::readConfig(std::string filename) {

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

void VehicleController::readTextures(std::string filename) {

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

	cout << "Finished reading vehicle files" << endl;

	m_textures = new GLuint[m_filenames_tex.size()];

	for (int i = 0; i < m_filenames_tex.size(); i++)
		initTexture(m_filenames_tex[i], i + 1);
}

void VehicleController::cleanUp() {

}
