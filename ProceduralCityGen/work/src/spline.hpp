/*
 * spline.hpp
 *
 *  Created on: 9/10/2015
 *      Author: bryerscame
 */
#pragma once
#ifndef SRC_SPLINE_HPP_
#define SRC_SPLINE_HPP_

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "comp308.hpp"


class Spline {

private:

	std::vector<comp308::vec3> m_keyframes;
	std::vector<comp308::vec3> m_points;

public:

	virtual ~Spline();
	static comp308::vec2 calculatePoint(comp308::vec2,comp308::vec2,comp308::vec2,comp308::vec2,float);
	static float calculateYValue(std::vector<comp308::vec2> points,float x_dist);
};

#endif /* SRC_SPLINE_HPP_ */
