/*
 * spline.cpp
 *
 *  Created on: 9/10/2015
 *      Author: bryerscame
 */
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

#include "spline.hpp"
#include "comp308.hpp"

using namespace std;
using namespace comp308;

void Spline::renderSpline() {
	// cout << "RENDERING SPLINE..." << endl;
	// cout << numPts << endl;

	// Points on the spine
	float xcr, ycr;

	if ((int) m_points.size() > 3) {

		m_points.clear();

		for (int i = 1; i < (int) m_points.size() - 2; ++i) {
			float length = 0;

			for (int k = 0; k < 50; ++k) {
				float t = k * 0.02;

				// To the calculations
				xcr = m_keyframes[i].x
						+ 0.5 * t
								* (-m_keyframes[i - 1].x + m_keyframes[i + 1].x)
						+ t * t
								* (m_keyframes[i - 1].x - 2.5 * m_keyframes[i].x
										+ 2 * m_keyframes[i + 1].x
										- 0.5 * m_keyframes[i + 2].x)
						+ t * t * t
								* (-0.5 * m_keyframes[i - 1].x
										+ 1.5 * m_keyframes[i].x
										- 1.5 * m_keyframes[i + 1].x
										+ 0.5 * m_keyframes[i + 2].x);

				ycr = m_keyframes[i].y
						+ 0.5 * t
								* (-m_keyframes[i - 1].y + m_keyframes[i + 1].y)
						+ t * t
								* (m_keyframes[i - 1].y - 2.5 * m_keyframes[i].y
										+ 2 * m_keyframes[i + 1].y
										- 0.5 * m_keyframes[i + 2].y)
						+ t * t * t
								* (-0.5 * m_keyframes[i - 1].y
										+ 1.5 * m_keyframes[i].y
										- 1.5 * m_keyframes[i + 1].y
										+ 0.5 * m_keyframes[i + 2].y);

				m_points.push_back(vec3(xcr, ycr, 0));
			}
		}
	}
}

Spline::~Spline() {
	// TODO Auto-generated destructor stub
}

