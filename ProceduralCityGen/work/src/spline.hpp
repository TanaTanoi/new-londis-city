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

	Spline(std::vector<comp308::vec3> keyframes) : m_keyframes(keyframes){}
	virtual ~Spline();
	void renderSpline();

};

#endif /* SRC_SPLINE_HPP_ */
