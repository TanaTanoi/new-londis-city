#pragma once
/*
 * vehicle.hpp
 *
 *  Created on: 19/09/2015
 *      Author: bryerscame
 */
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "comp308.hpp"

struct vertex {
	int p = 0; // index for point in m_points
	int t = 0; // index for uv in m_uvs
	int n = 0; // index for normal in m_normals
};

struct triangle {
	vertex v[3]; //requires 3 verticies
};

// This class will expand on the geometry class
class Vehicle {

private:

	// Fields for storing raw obj information
	std::string m_filename;
	std::vector<comp308::vec3> m_points;	// Point list
	std::vector<comp308::vec2> m_uvs;		// Texture Coordinate list
	std::vector<comp308::vec3> m_normals;	// Normal list
	std::vector<triangle> m_triangles;		// Triangle/Face list

	// IDs for the display list to render
	GLuint m_displayListPoly = 0; // DisplayList for Polygon

	// Current transformations
	comp308::vec3 m_pos, m_rot, m_scale;

	void init(std::string);
	void readOBJ(std::string);

	void createNormals();
	void createDisplayListPoly();

public:

	Vehicle(std::string);
	Vehicle(std::string, comp308::vec3, comp308::vec3, comp308::vec3);

	void renderVehicle();
	void handleCommand(int);

	// Getters and setters
	const comp308::vec3& getPos() const;
	void setPos(const comp308::vec3& pos);
	const comp308::vec3& getRot() const;
	void setRot(const comp308::vec3& rot);
	const comp308::vec3& getScale() const;
	void setScale(const comp308::vec3& scale);
};

// Controller for vehicles
class VehicleController {

private:

	// Points that vehicles cannot occupy
	std::vector<comp308::vec3> m_outOfBounds;

	// Size of the map
	comp308::vec3 m_mapSize;

	// textures and cars
	std::vector<std::string> m_filenames_car;
	std::vector<std::string> m_filenames_tex;

	std::vector<Vehicle> m_vehicles;

	// Textures for vehicles
	GLuint *m_textures;

	void readConfig(std::string);
	void readTextures(std::string);
	void initVehicles();
	void initTexture(std::string, int);

public:

	VehicleController(std::string, std::string, std::vector<comp308::vec3>, comp308::vec3);

	// Render all of the vehicles
	void renderVehicles();

	// Render a specific vehicle given a transformation and a texture
	void renderVehicle(Vehicle*, comp308::vec3, comp308::vec3, comp308::vec3, int);

	// Delete all pointers
	void cleanUp();

};

// #endif /* SRC_VEHICLE_HPP_ */
