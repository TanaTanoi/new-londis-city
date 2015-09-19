/*
 * vehicle.cpp
 *
 *  Created on: 19/09/2015
 *      Author: bryerscame
 */

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "comp308.hpp"
#include "vehicle.hpp"

using namespace std;
using namespace comp308;

Vehicle::Vehicle(string filename) {
	m_filename = filename;
	readOBJ(filename);
	if (m_triangles.size() > 0) {
		createDisplayListPoly();
	}
}

void Vehicle::readOBJ(string filename) {

	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0, 0, 0));
	m_uvs.push_back(vec2(0, 0));
	m_normals.push_back(vec3(0, 0, 1));

	ifstream objFile(filename);

	if (!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << filename << endl;

	// good() means that failbit, badbit and eofbit are all not set
	while (objFile.good()) {

		// Pull out line from file
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		if (!objLine.fail()) {

			if (mode == "v") {
				vec3 v;
				objLine >> v.x >> v.y >> v.z;
				m_points.push_back(v);

			} else if (mode == "vn") {
				vec3 vn;
				objLine >> vn.x >> vn.y >> vn.z;
				m_normals.push_back(vn);

			} else if (mode == "vt") {
				vec2 vt;
				objLine >> vt.x >> vt.y;
				m_uvs.push_back(vt);

			} else if (mode == "f") {

				vector<vertex> verts;
				while (objLine.good()) {
					vertex v;

					// v/vt/vn file format
					if (m_uvs.size() > 1) {

						objLine >> v.p;		// Scan in position index
						objLine.ignore(1);	// Ignore the '/' character
						objLine >> v.t;		// Scan in uv (texture coord) index
						objLine.ignore(1);	// Ignore the '/' character
						objLine >> v.n;		// Scan in normal index
					}

					// v//vn file format
					else if (m_normals.size() > 1) {
						objLine >> v.p;
						objLine.ignore(2);
						objLine >> v.n;
					}

					// v file format
					else
						objLine >> v.p;

					verts.push_back(v);
				}

				// IFF we have 3 vertices, construct a triangle
				if (verts.size() == 3) {
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);

				}
			}
		}
	}

	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size() - 1 << " points" << endl;
	cout << m_uvs.size() - 1 << " uv coords" << endl;
	cout << m_normals.size() - 1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;

	// If we didn't have any normals, create them
	if (m_normals.size() <= 1)
		createNormals();
}

void Vehicle::createNormals() {

	// Initialize the normals vector
	for (int i = 0; i < m_points.size(); ++i)
		m_normals.push_back(vec3(0, 0, 0));

	for (int i = 0; i < m_triangles.size(); ++i) {

		vec3 v1 = (m_points[m_triangles[i].v[0].p]);
		vec3 v2 = (m_points[m_triangles[i].v[1].p]);
		vec3 v3 = (m_points[m_triangles[i].v[2].p]);

		vec3 surfaceNormal = cross(v2 - v1, v3 - v1);

		// float sin_alpha = length(surfaceNormal)
		//		/ (length(v2 - v1) * length(v3 - v1));

		for (int j = 0; j < 3; ++j) {

			// Create the vertex normal
			m_normals[m_triangles[i].v[j].p] += normalize(surfaceNormal);

			// Create reference to normal
			m_triangles[i].v[j].n = m_triangles[i].v[j].p;
		}
	}

	// Normalize the vertex normal
	for (vec3 vertexNormal : m_normals)
		if (vertexNormal.x != 0)
			normalize(vertexNormal);

	cout << m_normals.size() - 1 << " normals" << endl;

}

void Vehicle::createDisplayListPoly() {

	if (m_displayListPoly)
		glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	cout << "Creating Poly Geometry" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < m_triangles.size(); ++i) {

		// Create the vectors
		vec3 v0 = m_points[m_triangles[i].v[0].p];
		vec3 v1 = m_points[m_triangles[i].v[1].p];
		vec3 v2 = m_points[m_triangles[i].v[2].p];

		// Create the normals
		vec3 norm0 = m_normals[m_triangles[i].v[0].n];
		vec3 norm1 = m_normals[m_triangles[i].v[1].n];
		vec3 norm2 = m_normals[m_triangles[i].v[2].n];

		// Create the texture coords
		vec2 uv1 = m_uvs[m_triangles[i].v[0].t];
		vec2 uv2 = m_uvs[m_triangles[i].v[1].t];
		vec2 uv3 = m_uvs[m_triangles[i].v[2].t];

		glNormal3d(norm0.x, norm0.y, norm0.z);
		glTexCoord2f(uv1.x, uv1.y);
		glVertex3f(v0.x, v0.y, v0.z);

		glNormal3d(norm1.x, norm1.y, norm1.z);
		glTexCoord2f(uv2.x, uv2.y);
		glVertex3f(v1.x, v1.y, v1.z);

		glNormal3d(norm2.x, norm2.y, norm2.z);
		glTexCoord2f(uv3.x, uv3.y);
		glVertex3f(v2.x, v2.y, v2.z);
	}
	glEnd();

	glEndList();
	cout << "Finished creating Poly Geometry" << endl;
}

void Vehicle::renderVehicle() {
	glShadeModel(GL_SMOOTH);
	glCallList(m_displayListPoly);
}

void Vehicle::handleCommand(int command) {
}
