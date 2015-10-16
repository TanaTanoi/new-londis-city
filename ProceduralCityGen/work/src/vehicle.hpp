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
#include "spline.hpp"
#include "cycleUtil.hpp"

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

class Spline;

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
	comp308::vec3 m_pos, m_pos_previous, m_rot, m_scale;

	// Direction that the vehicle is facing
	Direction m_direction = NORTH;

	// Goal that the vehicle is heading to
	cycle::roadNode m_goal, m_target, m_startPos;

	// Needed for vehicle to turn corners
	Spline *m_spline = nullptr;

	// Path that the vehicle takes
	std::vector<cycle::roadNode> m_path;
	int m_pathIndex = 0;

	bool m_isMoving = false, m_isTurning = false;
	float m_velocity = 0;

	void init(std::string);
	void readOBJ(std::string);

	void createNormals();
	void createDisplayListPoly();

public:

	Vehicle(std::string);
	Vehicle(std::string, comp308::vec3, comp308::vec3, comp308::vec3);
	virtual ~Vehicle();

	bool hasReachedGoal(comp308::vec3 *goal);
	void renderVehicle();

	cycle::roadNode nextTarget();

	cycle::roadNode getCurrentTarget();

	cycle::roadNode getPreviousTarget();

	void setPath(std::vector<cycle::roadNode>);

	void setSpline(std::vector<comp308::vec3>);
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

	inline const bool isTurning() const {
		return m_isTurning;
	}

	inline void setTurning(bool isTurning = false) {
		m_isTurning = isTurning;
	}

	inline const cycle::roadNode& getGoal() const {
		return m_goal;
	}

	inline void setGoal(const cycle::roadNode& goal) {
		m_goal = goal;
	}

	inline const cycle::roadNode& getStartPos() const {
		return m_startPos;
	}

	inline void setStartPos(const cycle::roadNode& pos) {
		m_startPos = pos;
	}

	inline const comp308::vec3& getPos() const {
		return m_pos;
	}

	inline void setPos(const comp308::vec3& pos) {
		m_pos = pos;
	}

	inline const std::vector<cycle::roadNode>& getPath() const {
		return m_path;
	}

	inline const comp308::vec3& getPosPrevious() const {
		return m_pos_previous;
	}

	inline void setPosPrevious(const comp308::vec3& posPrevious) {
		m_pos_previous = posPrevious;
	}
};

#endif /* SRC_VEHICLE_HPP_ */
