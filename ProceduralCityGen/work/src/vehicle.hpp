#pragma once
#ifndef SRC_VEHICLE_HPP_
#define SRC_VEHICLE_HPP_
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

enum Direction {
	NORTH, EAST, SOUTH, WEST
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

	// Direction that the vehicle is facing
	Direction m_direction = NORTH;

	// Goal that the vehicle is heading to
	comp308::vec3 m_goal;

	bool m_isMoving = false;
	float m_velocity = 0;

	void init(std::string);
	void readOBJ(std::string);

	void createNormals();
	void createDisplayListPoly();

public:

	Vehicle(std::string);
	Vehicle(std::string, comp308::vec3, comp308::vec3, comp308::vec3);

	bool hasReachedGoal(comp308::vec3 *goal);
	void renderVehicle();
	void handleCommand(int);

	// Getters and setters

	inline Direction getDirection() const {
		return m_direction;
	}

	inline void setDirection(Direction direction = NORTH) {
		m_direction = direction;
	}

	inline bool isIsMoving() const {
		return m_isMoving;
	}

	inline void setIsMoving(bool isMoving = false) {
		m_isMoving = isMoving;
	}

	inline const comp308::vec3& getPos() const {
		return m_pos;
	}

	inline void setPos(const comp308::vec3& pos) {
		m_pos = pos;
	}

	inline void translatePos(const comp308::vec3& pos) {
		m_pos += pos;
	}

	inline const comp308::vec3& getRot() const {
		return m_rot;
	}

	inline void setRot(const comp308::vec3& rot) {
		m_rot = rot;
	}

	inline const comp308::vec3& getScale() const {
		return m_scale;
	}

	inline void setScale(const comp308::vec3& scale) {
		m_scale = scale;
	}

	inline float getVelocity() const {
		return m_velocity;
	}

	inline void setVelocity(float velocity = 0) {
		m_velocity = velocity;
	}

	inline const comp308::vec3& getGoal() const {
		return m_goal;
	}

	inline void setGoal(const comp308::vec3& goal) {
		m_goal = goal;
	}
};

#endif /* SRC_VEHICLE_HPP_ */
