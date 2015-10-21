/*
 * vehicleController.hpp
 *
 *  Created on: 6/10/2015
 *      Author: bryerscame
 */
#pragma once
#ifndef SRC_VEHICLECONTROLLER_HPP_
#define SRC_VEHICLECONTROLLER_HPP_

#include <map>
#include <vector>

/**
 * Used to show if there are cars at the surrounding roads at an intersection.
 */
struct intersection {
	bool foward, port, starboard;
};

/**
 * Used when a vehicle is turning.
 */
struct turn {
	comp308::vec3 trans, rot;
};

class RoadNetwork;

class AStarSearchAlgo;

class Spline;

// Controller for vehicles
class VehicleController {

private:

	std::vector<comp308::vec3> m_keyframes;
	std::vector<comp308::vec3> m_points;

	// Contains all of the roads for vehicles to travel on
	RoadNetwork *m_network = nullptr;

	// Used to find vehicle path
	AStarSearchAlgo *m_aStar = nullptr;

	// Used to make vehicles travel between points
	Spline *m_spline = nullptr;

	// Size of the map
	int m_size;

	// textures and cars
	std::vector<std::string> m_filenames_car;
	std::vector<std::string> m_filenames_tex;

	std::vector<Vehicle> m_vehicles;

	// Textures for vehicles
	GLuint *m_textures;

	void readConfig(std::string);
	void readTextures(std::string);
	void initVehicles();
	void initTexture(std::string, GLuint);

	float disToNextVehicle(Vehicle*);
	float calculateStoppingDistance(Vehicle*, comp308::vec3* target);

	/* Old methods, not used */
	Direction turnToTake(Vehicle*);
	Direction calculateTurn(Direction, Direction, Direction, Direction, float);

	intersection checkIntersections(comp308::vec3*);
	comp308::vec3 findTarget(Vehicle*);

	comp308::vec3 interpolate_straight(Vehicle*, comp308::vec3*,
			cycle::roadNode*);
	comp308::vec3 interpolate_straight(Vehicle*, comp308::vec3*);
	turn interpolate_curve(Vehicle*, comp308::vec3*, comp308::vec3*, Direction);

	comp308::vec3 turnSimp(comp308::vec3*, comp308::vec3*, cycle::roadNode*,
			cycle::roadNode*);

	/* New methods, currently used */
	comp308::vec3 interpolate(Vehicle*, cycle::roadNode*, cycle::roadNode*,
			cycle::roadNode*);
	comp308::vec3 interpolate(Vehicle*, int);

	comp308::vec3 yaw(Vehicle *v, comp308::vec3*);
	comp308::vec3 yaw(comp308::vec3*, comp308::vec3*);

	bool reachedTarget(Vehicle*);
	cycle::roadNode generateGoal(Vehicle*);
	bool reachedGoal(Vehicle*);

	bool canTravel(Vehicle*);

public:

	VehicleController(std::string, std::string, int size = 10);
	virtual ~VehicleController();

	void parseRoadNetwork(RoadNetwork*);

	// Render all of the vehicles
	void tick();

	// Not used
	void testRender();

	// Render a specific vehicle given a transformation and a texture
	void renderVehicle(Vehicle*, comp308::vec3*, comp308::vec3*, comp308::vec3*,
			int texture = -1);
};

#endif /* SRC_VEHICLECONTROLLER_HPP_ */
