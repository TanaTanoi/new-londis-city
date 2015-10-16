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

VehicleController::VehicleController(string vehicles, string textures,
		vector<vec3> limits, vec3 bounds) {

	readConfig(vehicles);
	readTextures(textures);
}

VehicleController::~VehicleController() {
	if (m_network)
		delete m_network;
	if (m_aStar)
		delete m_aStar;
}

void VehicleController::parseRoadNetwork(RoadNetwork *network) {
	m_network = network;
	m_aStar = new AStarSearchAlgo(m_network->getAllNodes(),
			m_network->getAdjacencyList());
	initVehicles();
}

void VehicleController::initVehicles() {

	// TODO will have to change this according to the size of the map
	int size = 1, index;

	for (int i = 0; i < size; i++) {
		index = rand() % m_filenames_car.size();
		m_vehicles.push_back(Vehicle(m_filenames_car[index]));

		// Set a random starting position
		vector<roadNode> goals = m_network->getAllNodes();
		int randomNumber = rand() % goals.size();

		// Choose position at random
		// vec2 location = goals[randomNumber].location;
		// vec3 startPos = vec3(location.x * SECTION_POINT_TO_SCALE, 0,
		//		location.y * SECTION_POINT_TO_SCALE);

		roadNode start = goals[randomNumber];
		m_vehicles[i].setStartPos(start);

		cout << "Starting position " << m_vehicles[i].getStartPos().location
				<< endl;
		m_vehicles[i].setPos(vec3(start.location.x, 0, start.location.y));
		m_vehicles[i].setPosPrevious(
				vec3(start.location.x, 0, start.location.y));
		// cout << m_vehicles[i].getDirection() << endl;

		// Rotate the vehicle towards the goal
		Direction inital = turnToTake(&m_vehicles[i]);
		m_vehicles[i].setDirection(inital);

		cout << "Vehicle " << i << " is travelling ";
		switch (inital) {
		case NORTH:
			cout << "north" << endl;
			break;
		case EAST:
			cout << "east" << endl;
			break;
		case SOUTH:
			cout << "south" << endl;
			break;
		case WEST:
			cout << "west" << endl;
			break;
		}

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

/**
 * Go through each vehicle and update their positions. Little bit of jargon,
 * GOAL is the final position that the vehicle is aiming for,
 * TARGET is the end of the road that the vehicle is currently on.
 */
void VehicleController::tick() {

	for (int i = 0; i < (int) m_vehicles.size(); ++i) {

		vec3 rot = m_vehicles[i].getRot();

		// Current position that the vehicle is at
		// vec3 start = m_vehicles[i].getPos();

		// Generate a goal for the vehicle
		if (reachedGoal(&m_vehicles[i])) {

			// Generate a goal
			roadNode goal = generateGoal(&m_vehicles[i]);
			roadNode start = m_vehicles[i].getStartPos();

			// Update the goal
			m_vehicles[i].setGoal(goal);

			// Generate path
			m_aStar->aStarSearch(&start, &goal);
			cout << "Found path" << endl;

			// Update the path
			m_vehicles[i].setPath(m_aStar->getPath(&goal));

			// Set the starting position to be the previous goal
			m_vehicles[i].setStartPos(m_vehicles[i].getGoal());

			// Skip the first target
			m_vehicles[i].nextTarget();
		}

		roadNode target = m_vehicles[i].getCurrentTarget();

		if (reachedTarget(&m_vehicles[i], &target)) {

			cout << "Target reached" << endl;

			// Change the target
			m_vehicles[i].nextTarget();
		}

		// The target that the vehicle is heading towards
		// vec3 target = findTarget(&m_vehicles[i]);
		// vec3 target = m_vehicles[i].getNextTarget();

		// Render the vehicles' target as a point
		glPushMatrix();
		glPointSize(10);
		glTranslatef(0, 0.1, 0);
		glBegin(GL_POINTS);
		glColor3f(1.0, 0.1, 0.0);
		roadNode t;

		for (int j = 0; j < (int) m_vehicles[i].getPath().size(); ++j) {
			t = m_vehicles[i].getPath()[j];
			// cout << "target " << t.ID << ", " << t.location << endl;
			glVertex3f(t.location.x * Generator::SECTION_TO_POINT_SCALE(), 0,
					t.location.y * Generator::SECTION_TO_POINT_SCALE());
		}
		glEnd();
		glPopMatrix();
//
//		cout << "Vehicle " << i << " target: " << target << endl;

		Direction turnDir;

//		// Check if vehicle has reached the intersection
//		if (reachedTarget(&m_vehicles[i], &m_vehicles[i].getCurrentTarget())) {
//
//			cout << "Reached Target" << endl;
//
//			// Find the turning direction
//			turnDir = turnToTake(&m_vehicles[i]);
//
//			if (m_vehicles[i].getDirection() != turnDir) {
//				m_vehicles[i].setDirection(turnDir);
//				m_vehicles[i].setTurning(true);
//			}
//		}

		vec3 new_pos = vec3(m_vehicles[i].getStartPos().location.x, 0,
				m_vehicles[i].getStartPos().location.y);

		vec3 currentPos = m_vehicles[i].getPos();
		vec3 previousPos = m_vehicles[i].getPosPrevious();

		roadNode prev = m_vehicles[i].getPreviousTarget();
		roadNode cur = m_vehicles[i].getCurrentTarget();

		rot = turnSimp(&currentPos, &previousPos, &prev, &cur);

		m_vehicles[i].setRot(rot);
		new_pos = interpolate_straight(&m_vehicles[i], &currentPos, &target);

//		// If the vehicle is turning
//		if (m_vehicles[i].isTurning()) {
//			cout << "Vehicle " << i << " turning" << endl;
//			turn t = interpolate_curve(&m_vehicles[i], &start, &target,
//					turnDir);
//			new_pos = t.trans;
//			rot = t.rot;
//		}
//		// If the vehicle is traveling straight
//		else {
//			cout << "Vehicle " << i << " going straight" << endl;
//			new_pos = interpolate_straight(&m_vehicles[i], &start, &target);
//		}
//
//		cout << "Vehicle " << i << " new pos: " << new_pos << endl;
//
//		// Update vehicle's position
//		m_vehicles[i].translatePos(new_pos);
//
//		// Finally render the vehicle
		cout << "Rendering vehicle " << i << " at" << new_pos << endl;

		// target = m_vehicles[i].getCurrentTarget();
//		new_pos = vec3(target.location.x, 0, target.location.y);
		renderVehicle(&m_vehicles[i], new_pos, rot, scale);
	}
}

void VehicleController::testRender() {
	Vehicle v = Vehicle(m_filenames_car[0]);
	renderVehicle(&v, vec3(10), vec3(), vec3(5));
}

vec3 VehicleController::turnSimp(vec3 *pos, vec3 *prev, roadNode *v1,
		roadNode *v2) {

	int mode = 0;

	// Find which axis the thing is traveling on
	if (pos->x == v2->location.x && pos->x == v1->location.x
			&& pos->z > v1->location.y)
		mode = 1;
	if (pos->x != v2->location.x && pos->z == v2->location.y)
		mode = 2;

	// if (pos->z == v1->location.z && pos->z == v2->location.z && pos)

	// Find the angle between the two vectors
	vec3 targ1 = vec3(v1->location.x, 0, v1->location.y);
	vec3 targ2 = vec3(v2->location.x, 0, v2->location.y);

	cout << "targ1 " << targ1 << endl;
	cout << "targ2 " << targ2 << endl;

	float angle;

	angle = atan2(targ2.z - targ1.z, targ2.x - targ1.x);

	// Check if the length is increasing
	if (distance(*prev, targ2) < distance(*pos, targ2)) {
		cerr << "      Holy shit holy shit" << endl;
		// return vec3(0, 2 * comp308::pi() - abs(angle), 0);
	}

	if (angle < 1) {
		cout << "Angle " << angle << endl;
		return vec3(0, 2 * comp308::pi() + angle, 0);
	}
	return vec3(0, angle, 0);

//	switch (mode) {
//	case 0:
//	default:
//		cout << "Angle " << angle << endl;
//		return vec3(0, angle, 0);
//	case 1:
//		cout << "Angle " << degrees(angle) << endl;
//		cout << "MODE 1 " << degrees(angle) + 180 << endl;
//		angle = -angle;
//		return vec3(0, angle, 0);
//	}

	// if (angle > )

//	if (targ2.z > targ1.z) {
//		cout << "Alpha ";
//		angle = atan2(targ2.z - targ1.z, targ2.x - targ1.x);
//	} else if (targ1.z > targ2.z) {
//		angle = atan2(targ2.z - targ1.z, targ1.x - targ2.x);
//		cout << "Bravo ";
//	} else if (targ2.x < targ1.x) {
//		angle = atan2(targ1.z - targ2.z, targ2.x - targ1.x);
//		cout << "Charlie ";
//	} else if (targ2.x > targ1.x) {
//		angle = atan2(targ2.z - targ1.z, targ2.x - targ1.x);
//		cout << "Delta ";
//	}
//	angle = acos(angle);
//	// angle = degrees(angle);
//
//	cout << "Angle: " << degrees(angle) << endl;
//
//	if (util::isNAN(angle))
//		angle = 0;
//
//	return vec3(0, angle, 0);

//	vec3 rot;
//	if (angle >= 1.0)
//		rot = vec3(0.0);
//	else if (angle <= -1.0)
//		rot = vec3(0, comp308::pi(), 0);
//	else
//		rot = vec3(0, acos(angle), 0); // 0..PI

	cout << "Angle: " << degrees(angle) << endl;

//	float len1 = sqrt(v1->location.x * v1->location.x + v1->location.y * v1->location.y);
//	float len2 = sqrt(v2->location.x * v2->location.x + v2->location.y * v2->location.y);
//
//	float dot = v1->location.x * v2->location.x + v1->location.y * v2->location.y;
//
//	float a = dot / (len1 * len2);
//
//	cout << "angle: " << degrees(a) << endl;
//
//	if (a >= 1.0)
//		return vec3(0.0);
//	else if (a <= -1.0)
//		return vec3(0, comp308::pi(), 0);
//	else
//		return vec3(0, acos(a), 0); // 0..PI
}

/**
 * Render a vehicle given a transformation and a texture
 */
void VehicleController::renderVehicle(Vehicle* vehicle, vec3 translate,
		vec3 rotate, vec3 scale, int texture) {

	// Set transformations
	vehicle->setPosPrevious(vehicle->getPos());
	vehicle->setPos(translate);
	vehicle->setRot(rotate);

	// translate.x =  translate.x - vehicle->getCurrentTarget().location.x;
	// translate.z =  translate.z - vehicle->getCurrentTarget().location.y;

	cout << "Rendering here: " << translate << endl;

	// Add scale
	translate.x *= Generator::SECTION_TO_POINT_SCALE();
	translate.z *= Generator::SECTION_TO_POINT_SCALE();

	// Render the vehicle
	glPushMatrix();
	// glRotatef(rotate.y, 0, 1, 0);
	glTranslatef(translate.x, translate.y, translate.z);
	cout << "Rotated at " << rotate.y << endl;

	glScalef(scale.x, scale.y, scale.z);
	vehicle->renderVehicle();
	glPopMatrix();

	// Render the vehicles' position as a point
	glPushMatrix();
	// glRotatef(rotate.y, 0, 1, 0);
	glTranslatef(0, 0.2, 0);
	glPointSize(10);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex3f(translate.x, translate.y, translate.z);
	glEnd();
	glPopMatrix();

	// Render the vehicles' goal as a point
	glPushMatrix();
	vec2 goal = vehicle->getGoal().location;
	glTranslatef(0, -0.5, 0);
	glPointSize(15);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex3f(goal.x * Generator::SECTION_TO_POINT_SCALE(), 0,
			goal.y * Generator::SECTION_TO_POINT_SCALE());
	glEnd();
	glPopMatrix();
}

roadNode VehicleController::generateGoal(Vehicle* vehicle) {

	// Vector of all of the possible goals
	vector<roadNode> goals = m_network->getAllNodes();
	int randomNumber = rand() % goals.size();

	// Choose a goal at random
//	vec2 location = goals[randomNumber].location;
//	vec3 goal = vec3(location.x * SECTION_POINT_TO_SCALE, 0,
//			location.y * SECTION_POINT_TO_SCALE);
//
//	cout << "New goal " << goal << endl;

	roadNode goal = goals[randomNumber];
	return goal;
}

/**
 * Has the vehicle reached it's goal?
 */
bool VehicleController::reachedGoal(Vehicle* vehicle) {

	// If a goal has not been set
	if (vehicle->getGoal().location.x == -1)
		return true;

	roadNode goal = vehicle->getGoal();
	vec2 currentPos = vec2(vehicle->getPos().x, vehicle->getPos().z);

	util::checkZone check = util::checkZone(goal.location.x / 2.0,
			goal.location.y / 2.0, GIVE);

	if (floor(currentPos.x) == goal.location.x
			&& floor(currentPos.y) == goal.location.y) {
		cout << "Aww gummon " << endl;
		return true;
	}

	return check.contains(&currentPos);
}

/**
 * Has the vehicle reached it's target?
 */
bool VehicleController::reachedTarget(Vehicle *vehicle, roadNode *target) {

	// return vec2(vehicle->getPos().x, 0, vehicle->getPos().x) == target->location;
	vec2 currentPos = vec2(vehicle->getPos().x, vehicle->getPos().z);

	util::checkZone check = util::checkZone(target->location.x / 2.0,
			target->location.y / 2.0, GIVE);

	cout << "currentPos " << currentPos << endl;
	cout << "target " << target->location << endl;
	cout << "currentPos floor " << floor(currentPos.x) << ", "
			<< floor(currentPos.y) << endl;
	if (check.contains(&currentPos))
		cout << "FUCK YOU" << endl;

	if (floor(currentPos.x) == target->location.x
			&& floor(currentPos.y) == target->location.y) {
		cout << "Aww gummon " << endl;
		return true;
	}

	return check.contains(&currentPos);
}

// Find a target for a vehicle
vec3 VehicleController::findTarget(Vehicle *vehicle) {

//	// Find the road the vehicle is on
//	road closest = m_network->findClosestRoads(vehicle->getPos());
//
//	vec3 currentPos = vehicle->getPos();
//
//	// Road start and end
//	vec3 start = vec3(closest.start.location.x * SECTION_POINT_TO_SCALE, 0,
//			closest.start.location.y * SECTION_POINT_TO_SCALE);
//	vec3 end = vec3(closest.end.location.x * SECTION_POINT_TO_SCALE, 0,
//			closest.end.location.y * SECTION_POINT_TO_SCALE);
//
//	cout << "Closest: " << closest.start.location << ", "
//			<< closest.end.location << endl;
//	cout << "Start: " << start << endl;
//	cout << "End: " << end << endl;
//
//	// Find the vector that it is heading to
//	switch (vehicle->getDirection()) {
//
//	case NORTH:
//		if (end.y > currentPos.z)
//			return end;
//		else
//			return start;
//
//	case EAST:
//		if (end.x > currentPos.x)
//			return end;
//		return start;
//
//	case SOUTH:
//		if (end.y < currentPos.z)
//			return end;
//		return start;
//
//	case WEST:
//		if (end.x < currentPos.x)
//			return end;
//		return start;
//	}
//
//	return currentPos;

	return vec3();
}

/**
 * Calculate the stopping distance so a vehicle can deccelerate
 */
float VehicleController::calculateStoppingDistance(Vehicle* vehicle,
		vec3* target) {
	return -1;
}

// Find the distance to the nearest vehicle to a given vehicle
//
float VehicleController::disToNextVehicle(Vehicle* from) {
//
//	float min = -1;
//
//	for (Vehicle v : m_vehicles) {
//		if (v.getDirection() == from->getDirection()) {
//
//			float distance = 100000000000;
//
//			switch (from->getDirection()) {
//			case NORTH:
//			case SOUTH:
//				distance = abs(v.getPos().z - from->getPos().z);
//				break;
//			case EAST:
//			case WEST:
//				distance = abs(v.getPos().x - from->getPos().x);
//				break;
//			default:
//				break;
//			}
//
//			// If the distance calculated is less than the original min, replace it
//			min = (distance < min) ? distance : min;
//		}
//	}
//
//	return min;

	return -1;
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

//	vec3 currentPos = vehicle->getPos();
//	vec3 goal = vehicle->getGoal();
//
//	// Calculate the angle between the two vectors
//	// cosθ = (u⃗ · v⃗) / (||u⃗|| · ||v ⃗||)
//
//	float angle = (dot(currentPos, goal) / (length(currentPos) * length(goal)));
////	angle = cos(angle);
//
//	// angle = degrees(angle);
//	cout << "Angle: " << acos(angle) << endl;
//
//	angle = acos(angle);
//	angle = degrees(angle);
//
//	// Calculate the turn to take
//	switch (vehicle->getDirection()) {
//
//	case NORTH:
//		return calculateTurn(NORTH, EAST, SOUTH, WEST, angle);
//
//	case EAST:
//		return calculateTurn(EAST, SOUTH, WEST, NORTH, angle);
//
//	case SOUTH:
//		return calculateTurn(SOUTH, WEST, NORTH, EAST, angle);
//
//	case WEST:
//		return calculateTurn(WEST, NORTH, EAST, SOUTH, angle);
//	}

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
	if ((315 < angle && angle < 360) || (0 < angle && angle < 45))
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
vec3 VehicleController::interpolate_straight(Vehicle *vehicle, vec3 *from,
		roadNode *target) {

	time_t now = time(0);
	float currentTime = (float) now;
	float previousTime = (float) previous_time;

	// Calculate change in time
	float delta_time = currentTime - previousTime;

	// Calculating distance traveled
	float v_f = vehicle->getVelocity() + (ACCEL * delta_time);
	float distance = (v_f + vehicle->getVelocity() / 2) * delta_time;
	vehicle->setVelocity(v_f);

	// XXX: For now i'll set it to be a constant speed
	distance = 0.1;
	// cout << "Distance: " << distance << endl;

//	// Apply transformation
//	vec3 new_pos;
//	switch (vehicle->getDirection()) {
//	case NORTH:
//		new_pos = vec3(0, 0, distance);
//		break;
//	case SOUTH:
//		new_pos = vec3(0, 0, -distance);
//		break;
//	case EAST:
//		new_pos = vec3(distance, 0, 0);
//		break;
//	case WEST:
//		new_pos = vec3(-distance, 0, 0);
//		break;
//	default:
//		new_pos = vec3();
//		break;
//	}

//	clock_t begin = clock();

	// code_to_time();

	clock_t end = clock();
	float elapsed_secs = float(end - begin_time) / CLOCKS_PER_SEC;

	cout << "time: " << elapsed_secs << endl;

	const float speed = 1;
	vec3 velocity, direction, position;

	cout << " rot here: " << vehicle->getRot() << endl;

	direction.x = cos(vehicle->getRot().y);
	direction.z = sin(vehicle->getRot().y);

	cout << "direction: " << direction << endl;

	velocity.x = direction.x * speed;
	velocity.z = direction.z * speed;

	cout << " velocity: " << velocity << endl;

	position = vec3(from->x, 0, from->z);

	cout << " position: " << position << endl;

	position.x += velocity.x * elapsed_secs;
	position.z += velocity.z * elapsed_secs;

	cout << " new pos" << position << endl;

//	position.x += 2;
//	position.z += 2;
	position.x = (position.x != from->x) ? position.x + 0.5 : position.x;
	position.z = (position.z != from->z) ? position.z + 0.5 : position.z;

	begin_time = clock();
	return position;
}

turn VehicleController::interpolate_curve(Vehicle *vehicle, vec3 *from,
		vec3 *target, Direction turnDirection) {

//	turn tur;
//	tur.trans = *from;
//	tur.rot = vec3(0, 45, 0);
//	// cout << "RENDERING SPLINE..." << endl;
//	// cout << numPts << endl;
//
	turn tur;
	return tur;
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

