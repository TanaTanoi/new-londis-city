//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// COMP308 Main header
//
// Includes the GLEW, GLUT, and GLU headers with support for OSX
//
// Vector math library that mimics api and behaviour of GLSL without being as
// complicated as GLM. Functionality includes vec2, vec3, vec4, mat2, mat3, mat4
// and their respective functions.
//
// Important things to note, vectors and matrices are mutable. Matrices
// are column major (stored with columns in consectutive order) and are
// accessed by mat[column][row].
//
// Features not avaliable include swizzling. Structures not avaliable include
// samplers, bvec2, bvec3, bvec4, ivec2, ivec3, ivec4.
//
//
// See the official GLSL docs for relevant documentation
// - https://cvs.khronos.org/svn/repos/ogl/trunk/ecosystem/public/sdk/docs/man4/html/
//
// Or these other links that may be more helpful
// - http://www.shaderific.com/glsl-functions/   (recommended)
// - https://web.eecs.umich.edu/~sugih/courses/eecs487/common/notes/APITables.xml
// - http://docs.gl/
//
//
// NOTE: This is a new library which hasn't be throughly tested. If you suspect
// any bugs that are reproducible, please let the tutors know, and they'll fix it.
//
//
//----------------------------------------------------------------------------

#pragma once

// include glew.h before (instead of) gl.h, or anything that includes gl.h
// glew.h replaces gl.h and sets up OpenGL functions in a cross-platform manner
#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <GLFW/glfw3.h>
#else
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#endif

#include <cassert>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>

namespace comp308 {

// pi
inline double pi() {
	return 3.1415926535897932384626433832795;
}

// natural log base
inline double e() {
	return 2.7182818284590452353602874713527;
}

// golden ratio
inline double phi() {
	return 1.61803398874989484820458683436563811;
}

template<typename T> inline T radians(T val) {
	return val * pi() / 180.0;
}

template<typename T> inline T degrees(T val) {
	return val / pi() * 180.0;
}

template<typename T> inline T log2(const T &a) {
	return std::log(a) * 1.4426950408889634073599246810019;
}

template<typename T> inline T exp2(const T &a) {
	return std::pow(2, a);
}

template<typename T> inline T atan(const T &y, const T &x) {
	return std::atan2(y, x);
}

// Will not work with vecX/matX types
//

template<typename T> inline int sign(T val) {
	return (T(0) < val) - (val < T(0));
}

template<typename T> inline T inf() {
	// use like: inf<float>()
	// only for floating point types
	return std::numeric_limits<T>::infinity();
}

template<typename T> inline bool isinf(T a) {
	return std::numeric_limits<T>::max() < std::abs(a);
}

template<typename T> inline T nan() {
	// use like: nan<float>()
	// only for floating point types
	return T(0) / T(0);
}

template<typename T> inline bool isnan(T a) {
	return a != a;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////    ___  _____   __      ________ _____ _______ ____  _____                                                        ////
////   |__ \|  __ \  \ \    / /  ____/ ____|__   __/ __ \|  __ \                                                     ////
////      ) | |  | |  \ \  / /| |__ | |       | | | |  | | |__) |                                                  ////
////     / /| |  | |   \ \/ / |  __|| |       | | | |  | |  _  /                                                 ////
////    / /_| |__| |    \  /  | |___| |____   | | | |__| | | \ \                                                   ////
////   |____|_____/      \/   |______\_____|  |_|  \____/|_|  \_\                                                    ////
////                                                                                                                   ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class vec2 {
public:
	float x, y;

	// float constructors
	vec2() :
			x(0), y(0) {
	}
	explicit vec2(float v) :
			x(v), y(v) {
	}
	vec2(float _x, float _y) :
			x(_x), y(_y) {
	}

	static vec2 i() {
		return vec2(1, 0);
	}
	static vec2 j() {
		return vec2(0, 1);
	}

	static vec2 checknan(const vec2 &v) {
		float sum = v.x + v.y;
		assert(sum == sum);
		return v;
	}

	explicit operator float *() {
		return &(x);
	}

	const float & operator[](size_t i) const {
		assert(i < 2);
		return *(&x + i);
	}

	float & operator[](size_t i) {
		assert(i < 2);
		return *(&x + i);
	}

	// stream insertion
	inline friend std::ostream & operator<<(std::ostream &out, const vec2 &v) {
		return out << '(' << v.x << ", " << v.y << ')';
	}

	// Operator overload - assign
	//

	// add-assign
	inline vec2 & operator+=(const vec2 &rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	// add-assign
	inline vec2 & operator+=(float rhs) {
		x += rhs;
		y += rhs;
		return *this;
	}

	// subtract-assign
	inline vec2 & operator-=(const vec2 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	// subtract-assign
	inline vec2 & operator-=(float rhs) {
		x -= rhs;
		y -= rhs;
		return *this;
	}

	// mulitply-assign
	inline vec2 & operator*=(const vec2 &rhs) {
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	// mulitply-assign
	inline vec2 & operator*=(float rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}

	// divide-assign
	inline vec2 & operator/=(const vec2 &rhs) {
		x /= rhs.x;
		y /= rhs.y;
		vec2::checknan(*this);
		return *this;
	}

	// divide-assign
	inline vec2 & operator/=(float rhs) {
		x /= rhs;
		y /= rhs;
		vec2::checknan(*this);
		return *this;
	}

};

//TODO complete when mat3 is finished
// inline mat3 outerProduct(const vec2 &lhs, const &rhs){

// }

// Vector / Vector Operator Overloads
//

// negate
inline vec2 operator-(const vec2 &rhs) {
	return vec2(-rhs.x, -rhs.y);
}

// add
inline vec2 operator+(const vec2 &lhs, const vec2 &rhs) {
	vec2 v = lhs;
	return v += rhs;
}

// subtract
inline vec2 operator-(const vec2 &lhs, const vec2 &rhs) {
	vec2 v = lhs;
	return v -= rhs;
}

// multiply
inline vec2 operator*(const vec2 &lhs, const vec2 &rhs) {
	vec2 v = lhs;
	return v *= rhs;
}

// divide
inline vec2 operator/(const vec2 &lhs, const vec2 &rhs) {
	vec2 v = lhs;
	return v /= rhs;
}

// Vector / Scalar Operator Overloads
//

// add right
inline vec2 operator+(const vec2 &lhs, float rhs) {
	vec2 v = lhs;
	return v += rhs;
}

// add left
inline vec2 operator+(float lhs, const vec2 &rhs) {
	vec2 v = rhs;
	return v += lhs;
}

// subtract right
inline vec2 operator-(const vec2 &lhs, float rhs) {
	vec2 v = lhs;
	return v -= rhs;
}

// subtract left
inline vec2 operator-(float lhs, const vec2 &rhs) {
	vec2 v(lhs);
	return v -= rhs;
}

// multiply right
inline vec2 operator*(const vec2 &lhs, float rhs) {
	vec2 v = lhs;
	return v *= rhs;
}

// multiply left
inline vec2 operator*(float lhs, const vec2 &rhs) {
	vec2 v = rhs;
	return v *= lhs;
}

// divide right
inline vec2 operator/(const vec2 &lhs, float rhs) {
	vec2 v = lhs;
	return v /= rhs;
}

// divide left
inline vec2 operator/(float lhs, const vec2 &rhs) {
	vec2 v(lhs);
	return v /= rhs;
}

// Angle and Trigonometry Functions
//

// degrees to radians
inline vec2 radians(const vec2 &d) {
	return vec2(radians(d.x), radians(d.y));
}

// radians to degrees
inline vec2 degrees(const vec2 &r) {
	return vec2(degrees(r.x), degrees(r.y));
}

// sine
inline vec2 sin(const vec2 &v) {
	return vec2(std::sin(v.x), std::sin(v.y));
}

// cosine
inline vec2 cos(const vec2 &v) {
	return vec2(std::cos(v.x), std::cos(v.y));
}

// tangent
inline vec2 tan(const vec2 &v) {
	return vec2(std::tan(v.x), std::tan(v.y));
}

// arc sine
inline vec2 asin(const vec2 &v) {
	return vec2(std::asin(v.x), std::asin(v.y));
}

// arc cosine
inline vec2 acos(const vec2 &v) {
	return vec2(std::acos(v.x), std::acos(v.y));
}

// arc tangent of y/x
inline vec2 atan(const vec2 &y, const vec2 &x) {
	return vec2(std::atan2(y.x, x.x), std::atan2(y.y, x.y));
}

// arc tangent
inline vec2 atan(const vec2 &v) {
	return vec2(std::atan(v.x), std::atan(v.y));
}

// Exponential Functions
//

// v raised to the power of e
inline vec2 pow(const vec2 &v, const vec2 &e) {
	return vec2(std::pow(v.x, e.x), std::pow(v.y, e.y));
}

// natural exponentiation of vector
inline vec2 exp(const vec2 &v) {
	return vec2(std::exp(v.x), std::exp(v.y));
}

// natural logarithm of vector
inline vec2 log(const vec2 &v) {
	return vec2(std::log(v.x), std::log(v.y));
}

// base 2 exponentiation of vector
inline vec2 exp2(const vec2 &v) {
	return vec2(exp2(v.x), exp2(v.y));
}

// base 2 logarithm of vector
inline vec2 log2(const vec2 &v) {
	return vec2(log2(v.x), log2(v.y));
}

// square root of vector
inline vec2 sqrt(const vec2 &v) {
	return vec2(std::sqrt(v.x), std::sqrt(v.y));
}

// inverse of the square root of vector
inline vec2 inversesqrt(const vec2 &v) {
	return vec2::checknan(vec2(1 / std::sqrt(v.x), 1 / std::sqrt(v.y)));
}

// Common Functions
//

// absolute value of vector
inline vec2 abs(const vec2 &v) {
	return vec2(std::abs(v.x), std::abs(v.y));
}

// sign (-1, 0, 1) of vector
inline vec2 sign(const vec2 &v) {
	return vec2(sign(v.x), sign(v.y));
}

// floor of vector
inline vec2 floor(const vec2 &v) {
	return vec2(std::floor(v.x), std::floor(v.y));
}

// ceil of vector
inline vec2 ceil(const vec2 &v) {
	return vec2(std::ceil(v.x), std::ceil(v.y));
}

// fractional part of vector : v-floor(v)
inline vec2 fract(const vec2 &v) {
	return v - floor(v);
}

// modulas of vector : v-m*floor(v/m)
inline vec2 mod(const vec2 &v, float m) {
	return v - m * floor(v / m);
}

// modulas of vector : v-m*floor(v/m)
inline vec2 mod(const vec2 &v, const vec2 &m) {
	return v - m * floor(v / m);
}

// minimum of vector components and float
inline vec2 min(const vec2 &lhs, float rhs) {
	return vec2(std::min(lhs.x, rhs), std::min(lhs.y, rhs));
}

// minimum of vector components
inline vec2 min(const vec2 &lhs, const vec2 &rhs) {
	return vec2(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y));
}

// maximum of vector components and float
inline vec2 max(const vec2 &lhs, float rhs) {
	return vec2(std::max(lhs.x, rhs), std::max(lhs.y, rhs));
}

// maximum of vector components
inline vec2 max(const vec2 &lhs, const vec2 &rhs) {
	return vec2(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y));
}

// clamp components of vector between minVal and maxVal
inline vec2 clamp(const vec2 &v, float minVal, float maxVal) {
	return min(max(v, minVal), maxVal);
}

// clamp components of vector between minVal and maxVal components
inline vec2 clamp(const vec2 &v, const vec2 &minVal, const vec2 &maxVal) {
	return min(max(v, minVal), maxVal);
}

// linear blend of vectors : x*(1-a) + y*a
inline vec2 mix(const vec2 &lhs, const vec2 &rhs, float a) {
	return lhs * (1 - a) + rhs * a;
}

// linear blend of vectors : x*(1-a) + y*a
inline vec2 mix(const vec2 &lhs, const vec2 &rhs, const vec2 &a) {
	return lhs * (1 - a) + rhs * a;
}

// 0.0 if edge < v, else 1.0
inline vec2 step(const vec2 &edge, const vec2 &v) {
	return vec2((edge.x < v.x) ? 0.0 : 1.0, (edge.y < v.y) ? 0.0 : 1.0);
}

// 0.0 if edge < v, else 1.0
inline vec2 step(float edge, const vec2 &v) {
	return vec2((edge < v.x) ? 0.0 : 1.0, (edge < v.y) ? 0.0 : 1.0);
}

// smooth hermit interpolation
inline vec2 smoothstep(const vec2 &edge0, const vec2 &edge1, float x) {
	vec2 t = clamp((x - edge0) / (edge1 - edge0), 0, 1);
	return t * t * (3 - 2 * t);
}

// smooth hermit interpolation
inline vec2 smoothstep(const vec2 &edge0, const vec2 &edge1, const vec2 &x) {
	vec2 t = clamp((x - edge0) / (edge1 - edge0), 0, 1);
	return t * t * (3 - 2 * t);
}

// Geometric Functions
//

// length/magnitude of vector
inline float length(const vec2 &v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}

// distance between vectors
inline float distance(const vec2 &lhs, const vec2 &rhs) {
	return length(lhs - rhs);
}

// dot product
inline float dot(const vec2 &lhs, const vec2 &rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

// returns unit vector
inline vec2 normalize(const vec2 &v) {
	if (length(v) == 0)
		return v;
	return v / length(v);
}

// if dot(nref, i) < 0 return n else return -n
inline vec2 faceforward(const vec2 &n, const vec2 &i, const vec2 &nref) {
	return (dot(nref, i) < 0) ? n : -n;
}

// for incident i and surface normal n, returns the reflection direction
inline vec2 reflect(const vec2 &i, const vec2 &n) {
	return i - 2 * dot(n, i) * n;
}

// for incident i, surface normal n, and refraction index eta, return refraction vector
inline vec2 refract(const vec2 &i, const vec2 &n, float eta) {
	float k = 1.0 - eta * eta * (1.0 - dot(n, i) * dot(n, i));
	if (k < 0.0) {
		return vec2();
	}
	return eta * i - (eta * dot(n, i) + std::sqrt(k)) * n;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////    ____  _____   __      ________ _____ _______ ____  _____                                                       ////
////   |___ \|  __ \  \ \    / /  ____/ ____|__   __/ __ \|  __ \                                                    ////
////     __) | |  | |  \ \  / /| |__ | |       | | | |  | | |__) |                                                 ////
////    |__ <| |  | |   \ \/ / |  __|| |       | | | |  | |  _  /                                                ////
////    ___) | |__| |    \  /  | |___| |____   | | | |__| | | \ \                                                  ////
////   |____/|_____/      \/   |______\_____|  |_|  \____/|_|  \_\                                                   ////
////                                                                                                                   ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class vec3 {
public:
	float x, y, z;

	// float constructors
	vec3() :
			x(0), y(0), z(0) {
	}
	explicit vec3(float v) :
			x(v), y(v), z(v) {
	}
	vec3(float _x, float _y, float _z) :
			x(_x), y(_y), z(_z) {
	}

	// vec2 constructors
	vec3(const vec2 &v, float _z) :
			x(v.x), y(v.y), z(_z) {
	}
	vec3(float _x, const vec2 &v) :
			x(_x), y(v.x), z(v.y) {
	}

	// vec2 down-cast consctructor
	explicit operator vec2() const {
		return vec2(x, y);
	}

	static vec3 i() {
		return vec3(1, 0, 0);
	}
	static vec3 j() {
		return vec3(0, 1, 0);
	}
	static vec3 k() {
		return vec3(0, 0, 1);
	}

	static vec3 checknan(const vec3 &v) {
		float sum = v.x + v.y + v.z;
		assert(sum == sum);
		return v;
	}

	explicit operator float *() {
		return &(x);
	}

	const float & operator[](size_t i) const {
		assert(i < 3);
		return *(&x + i);
	}

	float & operator[](size_t i) {
		assert(i < 3);
		return *(&x + i);
	}

	// stream insertion
	inline friend std::ostream & operator<<(std::ostream &out, const vec3 &v) {
		return out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
	}

	// Operator overload - assign
	//

	// add-assign
	inline vec3 & operator+=(const vec3 &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	// add-assign
	inline vec3 & operator+=(float rhs) {
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}

	// subtract-assign
	inline vec3 & operator-=(const vec3 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	// subtract-assign
	inline vec3 & operator-=(float rhs) {
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}

	// mulitply-assign
	inline vec3 & operator*=(const vec3 &rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	// mulitply-assign
	inline vec3 & operator*=(float rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	// divide-assign
	inline vec3 & operator/=(const vec3 &rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		vec3::checknan(*this);
		return *this;
	}

	// divide-assign
	inline vec3 & operator/=(float rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		vec3::checknan(*this);
		return *this;
	}
};

//TODO complete when mat3 is finished
// inline mat3 outerProduct(const vec3 &lhs, const &rhs){

// }

// Vector / Vector Operator Overloads
//

// negate
inline vec3 operator-(const vec3 &rhs) {
	return vec3(-rhs.x, -rhs.y, -rhs.z);
}

// add
inline vec3 operator+(const vec3 &lhs, const vec3 &rhs) {
	vec3 v = lhs;
	return v += rhs;
}

// subtract
inline vec3 operator-(const vec3 &lhs, const vec3 &rhs) {
	vec3 v = lhs;
	return v -= rhs;
}

// multiply
inline vec3 operator*(const vec3 &lhs, const vec3 &rhs) {
	vec3 v = lhs;
	return v *= rhs;
}

// divide
inline vec3 operator/(const vec3 &lhs, const vec3 &rhs) {
	vec3 v = lhs;
	return v /= rhs;
}

// Vector / Scalar Operator Overloads
//

// add right
inline vec3 operator+(const vec3 &lhs, float rhs) {
	vec3 v = lhs;
	return v += rhs;
}

// add left
inline vec3 operator+(float lhs, const vec3 &rhs) {
	vec3 v = rhs;
	return v += lhs;
}

// subtract right
inline vec3 operator-(const vec3 &lhs, float rhs) {
	vec3 v = lhs;
	return v -= rhs;
}

// subtract left
inline vec3 operator-(float lhs, const vec3 &rhs) {
	vec3 v(lhs);
	return v -= rhs;
}

// multiply right
inline vec3 operator*(const vec3 &lhs, float rhs) {
	vec3 v = lhs;
	return v *= rhs;
}

// multiply left
inline vec3 operator*(float lhs, const vec3 &rhs) {
	vec3 v = rhs;
	return v *= lhs;
}

// divide right
inline vec3 operator/(const vec3 &lhs, float rhs) {
	vec3 v = lhs;
	return v /= rhs;
}

// divide left
inline vec3 operator/(float lhs, const vec3 &rhs) {
	vec3 v(lhs);
	return v /= rhs;
}

// Angle and Trigonometry Functions
//

// degrees to radians
inline vec3 radians(const vec3 &d) {
	return vec3(radians(d.x), radians(d.y), radians(d.z));
}

// radians to degrees
inline vec3 degrees(const vec3 &r) {
	return vec3(degrees(r.x), degrees(r.y), degrees(r.z));
}

// sine
inline vec3 sin(const vec3 &v) {
	return vec3(std::sin(v.x), std::sin(v.y), std::sin(v.z));
}

// cosine
inline vec3 cos(const vec3 &v) {
	return vec3(std::cos(v.x), std::cos(v.y), std::cos(v.z));
}

// tangent
inline vec3 tan(const vec3 &v) {
	return vec3(std::tan(v.x), std::tan(v.y), std::tan(v.z));
}

// arc sine
inline vec3 asin(const vec3 &v) {
	return vec3(std::asin(v.x), std::asin(v.y), std::asin(v.z));
}

// arc cosine
inline vec3 acos(const vec3 &v) {
	return vec3(std::acos(v.x), std::acos(v.y), std::acos(v.z));
}

// arc tangent of y/x
inline vec3 atan(const vec3 &y, const vec3 &x) {
	return vec3(std::atan2(y.x, x.x), std::atan2(y.y, x.y),
			std::atan2(y.z, x.z));
}

// arc tangent
inline vec3 atan(const vec3 &v) {
	return vec3(std::atan(v.x), std::atan(v.y), std::atan(v.z));
}

// Exponential Functions
//

// v raised to the power of e
inline vec3 pow(const vec3 &v, const vec3 &e) {
	return vec3(std::pow(v.x, e.x), std::pow(v.y, e.y), std::pow(v.z, e.z));
}

// natural exponentiation of vector
inline vec3 exp(const vec3 &v) {
	return vec3(std::exp(v.x), std::exp(v.y), std::exp(v.z));
}

// natural logarithm of vector
inline vec3 log(const vec3 &v) {
	return vec3(std::log(v.x), std::log(v.y), std::log(v.z));
}

// base 2 exponentiation of vector
inline vec3 exp2(const vec3 &v) {
	return vec3(exp2(v.x), exp2(v.y), exp2(v.z));
}

// base 2 logarithm of vector
inline vec3 log2(const vec3 &v) {
	return vec3(log2(v.x), log2(v.y), log2(v.z));
}

// square root of vector
inline vec3 sqrt(const vec3 &v) {
	return vec3(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z));
}

// inverse of the square root of vector
inline vec3 inversesqrt(const vec3 &v) {
	return vec3::checknan(
			vec3(1 / std::sqrt(v.x), 1 / std::sqrt(v.y), 1 / std::sqrt(v.z)));
}

// Common Functions
//

// absolute value of vector
inline vec3 abs(const vec3 &v) {
	return vec3(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

// sign (-1, 0, 1) of vector
inline vec3 sign(const vec3 &v) {
	return vec3(sign(v.x), sign(v.y), sign(v.z));
}

// floor of vector
inline vec3 floor(const vec3 &v) {
	return vec3(std::floor(v.x), std::floor(v.y), std::floor(v.z));
}

// ceil of vector
inline vec3 ceil(const vec3 &v) {
	return vec3(std::ceil(v.x), std::ceil(v.y), std::ceil(v.z));
}

// fractional part of vector : v-floor(v)
inline vec3 fract(const vec3 &v) {
	return v - floor(v);
}

// modulas of vector : v-m*floor(v/m)
inline vec3 mod(const vec3 &v, float m) {
	return v - m * floor(v / m);
}

// modulas of vector : v-m*floor(v/m)
inline vec3 mod(const vec3 &v, const vec3 &m) {
	return v - m * floor(v / m);
}

// minimum of vector components and float
inline vec3 min(const vec3 &lhs, float rhs) {
	return vec3(std::min(lhs.x, rhs), std::min(lhs.y, rhs),
			std::min(lhs.z, rhs));
}

// minimum of vector components
inline vec3 min(const vec3 &lhs, const vec3 &rhs) {
	return vec3(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y),
			std::min(lhs.z, rhs.z));
}

// maximum of vector components and float
inline vec3 max(const vec3 &lhs, float rhs) {
	return vec3(std::max(lhs.x, rhs), std::max(lhs.y, rhs),
			std::max(lhs.z, rhs));
}

// maximum of vector components
inline vec3 max(const vec3 &lhs, const vec3 &rhs) {
	return vec3(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y),
			std::max(lhs.z, rhs.z));
}

// clamp components of vector between minVal and maxVal
inline vec3 clamp(const vec3 &v, float minVal, float maxVal) {
	return min(max(v, minVal), maxVal);
}

// clamp components of vector between minVal and maxVal components
inline vec3 clamp(const vec3 &v, const vec3 &minVal, const vec3 &maxVal) {
	return min(max(v, minVal), maxVal);
}

// linear blend of vectors : x*(1-a) + y*a
inline vec3 mix(const vec3 &lhs, const vec3 &rhs, float a) {
	return lhs * (1 - a) + rhs * a;
}

// linear blend of vectors : x*(1-a) + y*a
inline vec3 mix(const vec3 &lhs, const vec3 &rhs, const vec3 &a) {
	return lhs * (1 - a) + rhs * a;
}

// 0.0 if edge < v, else 1.0
inline vec3 step(const vec3 &edge, const vec3 &v) {
	return vec3((edge.x < v.x) ? 0.0 : 1.0, (edge.y < v.y) ? 0.0 : 1.0,
			(edge.z < v.z) ? 0.0 : 1.0);
}

// 0.0 if edge < v, else 1.0
inline vec3 step(float edge, const vec3 &v) {
	return vec3((edge < v.x) ? 0.0 : 1.0, (edge < v.y) ? 0.0 : 1.0,
			(edge < v.z) ? 0.0 : 1.0);
}

// smooth hermit interpolation
inline vec3 smoothstep(const vec3 &edge0, const vec3 &edge1, float x) {
	vec3 t = clamp((x - edge0) / (edge1 - edge0), 0, 1);
	return t * t * (3 - 2 * t);
}

// smooth hermit interpolation
inline vec3 smoothstep(const vec3 &edge0, const vec3 &edge1, const vec3 &x) {
	vec3 t = clamp((x - edge0) / (edge1 - edge0), 0, 1);
	return t * t * (3 - 2 * t);
}

// Geometric Functions
//

// length/magnitude of vector
inline float length(const vec3 &v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// distance between vectors
inline float distance(const vec3 &lhs, const vec3 &rhs) {
	return length(lhs - rhs);
}

// dot product
inline float dot(const vec3 &lhs, const vec3 &rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

// cross product
inline vec3 cross(const vec3 &lhs, const vec3 &rhs) {
	return vec3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
}

// returns unit vector
inline vec3 normalize(const vec3 &v) {
	if(length(v)==0)return v;
	return v / length(v);
}

// if dot(nref, i) < 0 return n else return -n
inline vec3 faceforward(const vec3 &n, const vec3 &i, const vec3 &nref) {
	return (dot(nref, i) < 0) ? n : -n;
}

// for incident i and surface normal n, returns the reflection direction
inline vec3 reflect(const vec3 &i, const vec3 &n) {
	return i - 2 * dot(n, i) * n;
}

// for incident i, surface normal n, and refraction index eta, return refraction vector
inline vec3 refract(const vec3 &i, const vec3 &n, float eta) {
	float k = 1.0 - eta * eta * (1.0 - dot(n, i) * dot(n, i));
	if (k < 0.0) {
		return vec3();
	}
	return eta * i - (eta * dot(n, i) + std::sqrt(k)) * n;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////    _  _   _____   __      ________ _____ _______ ____  _____                                                      ////
////   | || | |  __ \  \ \    / /  ____/ ____|__   __/ __ \|  __ \                                                   ////
////   | || |_| |  | |  \ \  / /| |__ | |       | | | |  | | |__) |                                                ////
////   |__   _| |  | |   \ \/ / |  __|| |       | | | |  | |  _  /                                               ////
////      | | | |__| |    \  /  | |___| |____   | | | |__| | | \ \                                                 ////
////      |_| |_____/      \/   |______\_____|  |_|  \____/|_|  \_\                                                  ////
////                                                                                                                   ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class vec4 {
public:
	float x, y, z, w;

	// float constructors
	vec4() :
			x(0), y(0), z(0), w(0) {
	}
	explicit vec4(float v) :
			x(v), y(v), z(v), w(v) {
	}
	vec4(float _x, float _y, float _z, float _w) :
			x(_x), y(_y), z(_z), w(_w) {
	}

	// vec2 constructors
	vec4(const vec2 &v, float _z, float _w) :
			x(v.x), y(v.y), z(_z), w(_w) {
	}
	vec4(float _x, const vec2 &v, float _w) :
			x(_x), y(v.x), z(v.y), w(_w) {
	}
	vec4(float _x, float _y, const vec2 &v) :
			x(_x), y(_y), z(v.x), w(v.y) {
	}
	vec4(const vec2 &v0, const vec2 &v1) :
			x(v0.x), y(v0.y), z(v1.x), w(v1.y) {
	}

	// vec3 constructors
	vec4(const vec3 &v, float _w) :
			x(v.x), y(v.y), z(v.z), w(_w) {
	}
	vec4(float _x, const vec3 &v) :
			x(_x), y(v.x), z(v.y), w(v.z) {
	}

	// vec2, vec3 down-cast constructors
	explicit operator vec2() const {
		return vec2(x, y);
	}
	explicit operator vec3() const {
		return vec3(x, y, z);
	}

	static vec4 i() {
		return vec4(1, 0, 0, 0);
	}
	static vec4 j() {
		return vec4(0, 1, 0, 0);
	}
	static vec4 k() {
		return vec4(0, 0, 1, 0);
	}
	static vec4 l() {
		return vec4(0, 0, 0, 1);
	}

	static vec4 checknan(const vec4 &v) {
		float sum = v.x + v.y + v.z + v.w;
		assert(sum == sum);
		return v;
	}

	explicit operator float *() {
		return &(x);
	}

	const float & operator[](size_t i) const {
		assert(i < 4);
		return *(&x + i);
	}

	float & operator[](size_t i) {
		assert(i < 4);
		return *(&x + i);
	}

	// stream insertion
	inline friend std::ostream & operator<<(std::ostream &out, const vec4 &v) {
		return out << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w
				<< ')';
	}

	// Operator overload - assign
	//

	// add-assign
	inline vec4 & operator+=(const vec4 &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	// add-assign
	inline vec4 & operator+=(float rhs) {
		x += rhs;
		y += rhs;
		z += rhs;
		w += rhs;
		return *this;
	}

	// subtract-assign
	inline vec4 & operator-=(const vec4 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	// subtract-assign
	inline vec4 & operator-=(float rhs) {
		x -= rhs;
		y -= rhs;
		z -= rhs;
		w -= rhs;
		return *this;
	}

	// mulitply-assign
	inline vec4 & operator*=(const vec4 &rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	// mulitply-assign
	inline vec4 & operator*=(float rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	// divide-assign
	inline vec4 & operator/=(const vec4 &rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		vec4::checknan(*this);
		return *this;
	}

	// divide-assign
	inline vec4 & operator/=(float rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		vec4::checknan(*this);
		return *this;
	}
};

//TODO complete when mat3 is finished
// inline mat3 outerProduct(const vec3 &lhs, const &rhs){

// }

// Vector / Vector Operator Overloads
//

// negate
inline vec4 operator-(const vec4 &rhs) {
	return vec4(-rhs.x, -rhs.y, -rhs.z, -rhs.w);
}

// add
inline vec4 operator+(const vec4 &lhs, const vec4 &rhs) {
	vec4 v = lhs;
	return v += rhs;
}

// subtract
inline vec4 operator-(const vec4 &lhs, const vec4 &rhs) {
	vec4 v = lhs;
	return v -= rhs;
}

// multiply
inline vec4 operator*(const vec4 &lhs, const vec4 &rhs) {
	vec4 v = lhs;
	return v *= rhs;
}

// divide
inline vec4 operator/(const vec4 &lhs, const vec4 &rhs) {
	vec4 v = lhs;
	return v /= rhs;
}

// Vector / Scalar Operator Overloads
//

// add right
inline vec4 operator+(const vec4 &lhs, float rhs) {
	vec4 v = lhs;
	return v += rhs;
}

// add left
inline vec4 operator+(float lhs, const vec4 &rhs) {
	vec4 v = rhs;
	return v += lhs;
}

// subtract right
inline vec4 operator-(const vec4 &lhs, float rhs) {
	vec4 v = lhs;
	return v -= rhs;
}

// subtract left
inline vec4 operator-(float lhs, const vec4 &rhs) {
	vec4 v(lhs);
	return v -= rhs;
}

// multiply right
inline vec4 operator*(const vec4 &lhs, float rhs) {
	vec4 v = lhs;
	return v *= rhs;
}

// multiply left
inline vec4 operator*(float lhs, const vec4 &rhs) {
	vec4 v = rhs;
	return v *= lhs;
}

// divide right
inline vec4 operator/(const vec4 &lhs, float rhs) {
	vec4 v = lhs;
	return v /= rhs;
}

// divide left
inline vec4 operator/(float lhs, const vec4 &rhs) {
	vec4 v(lhs);
	return v /= rhs;
}

// Angle and Trigonometry Functions
//

// degrees to radians
inline vec4 radians(const vec4 &d) {
	return vec4(radians(d.x), radians(d.y), radians(d.z), radians(d.w));
}

// radians to degrees
inline vec4 degrees(const vec4 &r) {
	return vec4(degrees(r.x), degrees(r.y), degrees(r.z), degrees(r.w));
}

// sine
inline vec4 sin(const vec4 &v) {
	return vec4(std::sin(v.x), std::sin(v.y), std::sin(v.z), std::sin(v.w));
}

// cosine
inline vec4 cos(const vec4 &v) {
	return vec4(std::cos(v.x), std::cos(v.y), std::cos(v.z), std::cos(v.w));
}

// tangent
inline vec4 tan(const vec4 &v) {
	return vec4(std::tan(v.x), std::tan(v.y), std::tan(v.z), std::tan(v.w));
}

// arc sine
inline vec4 asin(const vec4 &v) {
	return vec4(std::asin(v.x), std::asin(v.y), std::asin(v.z), std::asin(v.w));
}

// arc cosine
inline vec4 acos(const vec4 &v) {
	return vec4(std::acos(v.x), std::acos(v.y), std::acos(v.z), std::acos(v.w));
}

// arc tangent of y/x
inline vec4 atan(const vec4 &y, const vec4 &x) {
	return vec4(std::atan2(y.x, x.x), std::atan2(y.y, x.y),
			std::atan2(y.z, x.z), std::atan2(y.w, x.w));
}

// arc tangent
inline vec4 atan(const vec4 &v) {
	return vec4(std::atan(v.x), std::atan(v.y), std::atan(v.z), std::atan(v.w));
}

// Exponential Functions
//

// v raised to the power of e
inline vec4 pow(const vec4 &v, const vec4 &e) {
	return vec4(std::pow(v.x, e.x), std::pow(v.y, e.y), std::pow(v.z, e.z),
			std::pow(v.w, e.w));
}

// natural exponentiation of vector
inline vec4 exp(const vec4 &v) {
	return vec4(std::exp(v.x), std::exp(v.y), std::exp(v.z), std::exp(v.w));
}

// natural logarithm of vector
inline vec4 log(const vec4 &v) {
	return vec4(std::log(v.x), std::log(v.y), std::log(v.z), std::log(v.w));
}

// base 2 exponentiation of vector
inline vec4 exp2(const vec4 &v) {
	return vec4(exp2(v.x), exp2(v.y), exp2(v.z), exp2(v.w));
}

// base 2 logarithm of vector
inline vec4 log2(const vec4 &v) {
	return vec4(log2(v.x), log2(v.y), log2(v.z), log2(v.w));
}

// square root of vector
inline vec4 sqrt(const vec4 &v) {
	return vec4(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z), std::sqrt(v.w));
}

// inverse of the square root of vector
inline vec4 inversesqrt(const vec4 &v) {
	return vec4::checknan(
			vec4(1 / std::sqrt(v.x), 1 / std::sqrt(v.y), 1 / std::sqrt(v.z),
					1 / std::sqrt(v.w)));
}

// Common Functions
//

// absolute value of vector
inline vec4 abs(const vec4 &v) {
	return vec4(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
}

// sign (-1, 0, 1) of vector
inline vec4 sign(const vec4 &v) {
	return vec4(sign(v.x), sign(v.y), sign(v.z), sign(v.w));
}

// floor of vector
inline vec4 floor(const vec4 &v) {
	return vec4(std::floor(v.x), std::floor(v.y), std::floor(v.z),
			std::floor(v.w));
}

// ceil of vector
inline vec4 ceil(const vec4 &v) {
	return vec4(std::ceil(v.x), std::ceil(v.y), std::ceil(v.z), std::ceil(v.w));
}

// fractional part of vector : v-floor(v)
inline vec4 fract(const vec4 &v) {
	return v - floor(v);
}

// modulas of vector : v-m*floor(v/m)
inline vec4 mod(const vec4 &v, float m) {
	return v - m * floor(v / m);
}

// modulas of vector : v-m*floor(v/m)
inline vec4 mod(const vec4 &v, const vec4 &m) {
	return v - m * floor(v / m);
}

// minimum of vector components and float
inline vec4 min(const vec4 &lhs, float rhs) {
	return vec4(std::min(lhs.x, rhs), std::min(lhs.y, rhs),
			std::min(lhs.z, rhs), std::min(lhs.w, rhs));
}

// minimum of vector components
inline vec4 min(const vec4 &lhs, const vec4 &rhs) {
	return vec4(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y),
			std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w));
}

// maximum of vector components and float
inline vec4 max(const vec4 &lhs, float rhs) {
	return vec4(std::max(lhs.x, rhs), std::max(lhs.y, rhs),
			std::max(lhs.z, rhs), std::max(lhs.w, rhs));
}

// maximum of vector components
inline vec4 max(const vec4 &lhs, const vec4 &rhs) {
	return vec4(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y),
			std::max(lhs.z, rhs.z), std::max(lhs.w, rhs.w));
}

// clamp components of vector between minVal and maxVal
inline vec4 clamp(const vec4 &v, float minVal, float maxVal) {
	return min(max(v, minVal), maxVal);
}

// clamp components of vector between minVal and maxVal components
inline vec4 clamp(const vec4 &v, const vec4 &minVal, const vec4 &maxVal) {
	return min(max(v, minVal), maxVal);
}

// linear blend of vectors : x*(1-a) + y*a
inline vec4 mix(const vec4 &lhs, const vec4 &rhs, float a) {
	return lhs * (1 - a) + rhs * a;
}

// linear blend of vectors : x*(1-a) + y*a
inline vec4 mix(const vec4 &lhs, const vec4 &rhs, const vec4 &a) {
	return lhs * (1 - a) + rhs * a;
}

// 0.0 if edge < v, else 1.0
inline vec4 step(const vec4 &edge, const vec4 &v) {
	return vec4((edge.x < v.x) ? 0.0 : 1.0, (edge.y < v.y) ? 0.0 : 1.0,
			(edge.z < v.z) ? 0.0 : 1.0, (edge.w < v.w) ? 0.0 : 1.0);
}

// 0.0 if edge < v, else 1.0
inline vec4 step(float edge, const vec4 &v) {
	return vec4((edge < v.x) ? 0.0 : 1.0, (edge < v.y) ? 0.0 : 1.0,
			(edge < v.z) ? 0.0 : 1.0, (edge < v.w) ? 0.0 : 1.0);
}

// smooth hermit interpolation
inline vec4 smoothstep(const vec4 &edge0, const vec4 &edge1, float x) {
	vec4 t = clamp((x - edge0) / (edge1 - edge0), 0, 1);
	return t * t * (3 - 2 * t);
}

// smooth hermit interpolation
inline vec4 smoothstep(const vec4 &edge0, const vec4 &edge1, const vec4 &x) {
	vec4 t = clamp((x - edge0) / (edge1 - edge0), 0, 1);
	return t * t * (3 - 2 * t);
}

// Geometric Functions
//

// length/magnitude of vector
inline float length(const vec4 &v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

// distance between vectors
inline float distance(const vec4 &lhs, const vec4 &rhs) {
	return length(lhs - rhs);
}

// dot product
inline float dot(const vec4 &lhs, const vec4 &rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

// returns unit vector
inline vec4 normalize(const vec4 &v) {
	return v / length(v);
}

// if dot(nref, i) < 0 return n else return -n
inline vec4 faceforward(const vec4 &n, const vec4 &i, const vec4 &nref) {
	return (dot(nref, i) < 0) ? n : -n;
}

// for incident i and surface normal n, returns the reflection direction
inline vec4 reflect(const vec4 &i, const vec4 &n) {
	return i - 2 * dot(n, i) * n;
}

// for incident i, surface normal n, and refraction index eta, return refraction vector
inline vec4 refract(const vec4 &i, const vec4 &n, float eta) {
	float k = 1.0 - eta * eta * (1.0 - dot(n, i) * dot(n, i));
	if (k < 0.0) {
		return vec4();
	}
	return eta * i - (eta * dot(n, i) + std::sqrt(k)) * n;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ___       ___    __  __       _______ _____  _______   __                                                        ////
//// |__ \     |__ \  |  \/  |   /\|__   __|  __ \|_   _\ \ / /                                                      ////
////    ) |_  __  ) | | \  / |  /  \  | |  | |__) | | |  \ V /                                                     ////
////   / /\ \/ / / /  | |\/| | / /\ \ | |  |  _  /  | |   > <                                                    ////
////  / /_ >  < / /_  | |  | |/ ____ \| |  | | \ \ _| |_ / . \                                                     ////
//// |____/_/\_\____| |_|  |_/_/    \_\_|  |_|  \_\_____/_/ \_\                                                      ////
////                                                                                                                   ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class mat2 {
private:
	vec2 data[2];

	void clear(float i = 0) {
		data[0] = vec2(i, 0);
		data[1] = vec2(0, i);
	}

public:
	mat2() {
		clear();
	}

	explicit mat2(float i) {
		clear(i);
	}

	mat2(const vec2 &a, const vec2 &b) {
		data[0] = a;
		data[1] = b;
	}

	mat2(float e00, float e10, float e01, float e11) {
		data[0] = vec2(e00, e10);
		data[1] = vec2(e01, e11);
	}

	explicit operator float *() {
		return &(data[0].x);
	}

	const vec2 & operator[](size_t i) const {
		assert(i < 2);
		return data[i];
	}

	vec2 & operator[](size_t i) {
		assert(i < 2);
		return data[i];
	}

	// stream insertion
	inline friend std::ostream & operator<<(std::ostream &out, const mat2 &m) {
		const size_t field_width = 10;
		std::ostringstream oss;
		oss << std::setprecision(4);
		oss << '[' << std::setw(field_width) << m[0][0] << ", "
				<< std::setw(field_width) << m[1][0] << ']' << std::endl;
		oss << '[' << std::setw(field_width) << m[0][1] << ", "
				<< std::setw(field_width) << m[1][1] << ']' << std::endl;
		return out << oss.str();
	}

	// Operator overload - assign
	//

	// add-assign
	inline mat2 & operator+=(const mat2 &rhs) {
		data[0] += rhs[0];
		data[1] += rhs[1];
		return *this;
	}

	// add-assign
	inline mat2 & operator+=(float rhs) {
		data[0] += rhs;
		data[1] += rhs;
		return *this;
	}

	// subtract-assign
	inline mat2 & operator-=(const mat2 &rhs) {
		data[0] -= rhs[0];
		data[1] -= rhs[1];
		return *this;
	}

	// subtract-assign
	inline mat2 & operator-=(float rhs) {
		data[0] -= rhs;
		data[1] -= rhs;
		return *this;
	}

	// This is a special case where we use matrix product
	// if you want component-wise multiplication see matrixCompMult(mat2, mat2)
	//
	// mulitply-assign
	inline mat2 & operator*=(const mat2 &rhs) {
		const mat2 lhs = *this;
		for (int i = 0; i < 2; i++) {
			(*this)[i] = vec2(0);
			for (int j = 0; j < 2; j++) {
				(*this)[i] += lhs[j] * rhs[i][j];
			}
		}
		return *this;
	}

	// mulitply-assign
	inline mat2 & operator*=(float rhs) {
		data[0] *= rhs;
		data[1] *= rhs;
		return *this;
	}

	// divide-assign
	inline mat2 & operator/=(const mat2 &rhs) {
		data[0] /= rhs[0];
		data[1] /= rhs[1];
		return *this;
	}

	// divide-assign
	inline mat2 & operator/=(float rhs) {
		data[0] /= rhs;
		data[1] /= rhs;
		return *this;
	}

};

// Matrix / Matrix Operator Overloads
// Matrix / Vector Operator Overloads
//

// negate
inline mat2 operator-(const mat2 &rhs) {
	mat2 m;
	m[0] = -rhs[0];
	m[1] = -rhs[1];
	return m;
}

// add
inline mat2 operator+(const mat2 &lhs, const mat2 &rhs) {
	mat2 m = lhs;
	return m += rhs;
}

// subtract
inline mat2 operator-(const mat2 &lhs, const mat2 &rhs) {
	mat2 m = lhs;
	return m -= rhs;
}

// This is a special case where we use matrix product
// if you want component-wise multiplication see matrixCompMult(mat2, mat2)
//
// multiply
inline mat2 operator*(const mat2 &lhs, const mat2 &rhs) {
	mat2 m = lhs;
	return m *= rhs;
}

// Left multiply mat2 m with vec2 v
//
// multiply
inline vec2 operator*(const mat2 &lhs, const vec2 &rhs) {
	vec2 v(0);
	for (int j = 0; j < 2; j++) {
		v += lhs[j] * rhs[j];
	}
	return v;
}

// Right multiply vec2 v with mat2 m
// Equvilent to transpose(m) * v
//
// mulitply-assign
inline vec2 operator*=(vec2 &lhs, const mat2 &rhs) {
	return lhs = vec2(dot(lhs, rhs[0]), dot(lhs, rhs[1]));
}

// multiply
inline vec2 operator*(const vec2 &lhs, const mat2 &rhs) {
	vec2 v = lhs;
	return v *= rhs;
}

// divide
inline mat2 operator/(const mat2 &lhs, const mat2 &rhs) {
	mat2 m = lhs;
	return m /= rhs;
}

// Vector / Scalar Operator Overloads
//

// add right
inline mat2 operator+(const mat2 &lhs, float rhs) {
	mat2 m = lhs;
	return m += rhs;
}

// add left
inline mat2 operator+(float lhs, const mat2 &rhs) {
	mat2 m = rhs;
	return m += lhs;
}

// subtract right
inline mat2 operator-(const mat2 &lhs, float rhs) {
	mat2 m = lhs;
	return m -= rhs;
}

// subtract left
inline mat2 operator-(float lhs, const mat2 &rhs) {
	vec2 v(lhs);
	mat2 m(v, v);
	return m -= rhs;
}

// multiply right
inline mat2 operator*(const mat2 &lhs, float rhs) {
	mat2 m = lhs;
	return m *= rhs;
}

// multiply left
inline mat2 operator*(float lhs, const mat2 &rhs) {
	mat2 m = rhs;
	return m *= lhs;
}

// divide right
inline mat2 operator/(const mat2 &lhs, float rhs) {
	mat2 m = lhs;
	return m /= rhs;
}

// divide left
inline mat2 operator/(float lhs, const mat2 &rhs) {
	vec2 v(lhs);
	mat2 m(v, v);
	return m /= rhs;
}

// Matrix functions
//

// determinant of matrix
inline float determinant(const mat2 &m) {
	return m[0].x * m[1].y - m[1].x * m[0].y;
}

// inverse of matrix (error if not invertible)
inline mat2 inverse(const mat2 &m) {
	float invdet = 1 / determinant(m);
	// FIXME proper detect infinite determinant
	assert(!isinf(invdet) && invdet == invdet && invdet != 0);
	return invdet * mat2(m[1].y, -m[0].y, -m[1].x, m[0].x);
}

// transpose of matrix
inline mat2 transpose(const mat2 &m) {
	return mat2(m[0][0], m[1][0], m[0][1], m[1][1]);
}

// component-wise multiplication
// see (*) and (*=) operator overload for matrix product
inline mat2 matrixCompMult(const mat2 &lhs, const mat2 &rhs) {
	mat2 m = lhs;
	m[0] *= rhs;
	m[1] *= rhs;
	return m;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ____       ____    __  __       _______ _____  _______   __                                                      ////
//// |___ \     |___ \  |  \/  |   /\|__   __|  __ \|_   _\ \ / /                                                    ////
////   __) |_  __ __) | | \  / |  /  \  | |  | |__) | | |  \ V /                                                   ////
////  |__ <\ \/ /|__ <  | |\/| | / /\ \ | |  |  _  /  | |   > <                                                  ////
////  ___) |>  < ___) | | |  | |/ ____ \| |  | | \ \ _| |_ / . \                                                   ////
//// |____//_/\_\____/  |_|  |_/_/    \_\_|  |_|  \_\_____/_/ \_\                                                    ////
////                                                                                                                   ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class mat3 {
private:
	vec3 data[3];

	void clear(float i = 0) {
		data[0] = vec3(i, 0, 0);
		data[1] = vec3(0, i, 0);
		data[2] = vec3(0, 0, i);
	}

public:
	mat3() {
		clear();
	}

	explicit mat3(float i) {
		clear(i);
	}

	mat3(const vec3 &a, const vec3 &b, const vec3 &c) {
		data[0] = a;
		data[1] = b;
		data[2] = c;
	}

	mat3(float e00, float e10, float e20, float e01, float e11, float e21,
			float e02, float e12, float e22) {
		data[0] = vec3(e00, e10, e20);
		data[1] = vec3(e01, e11, e21);
		data[2] = vec3(e02, e12, e22);
	}

	explicit operator float *() {
		return &(data[0].x);
	}

	const vec3 & operator[](size_t i) const {
		assert(i < 3);
		return data[i];
	}

	vec3 & operator[](size_t i) {
		assert(i < 3);
		return data[i];
	}

	// stream insertion
	inline friend std::ostream & operator<<(std::ostream &out, const mat3 &m) {
		const size_t field_width = 10;
		std::ostringstream oss;
		oss << std::setprecision(4);
		oss << '[' << std::setw(field_width) << m[0][0] << ", "
				<< std::setw(field_width) << m[1][0] << ", "
				<< std::setw(field_width) << m[2][0] << ']' << std::endl;
		oss << '[' << std::setw(field_width) << m[0][1] << ", "
				<< std::setw(field_width) << m[1][1] << ", "
				<< std::setw(field_width) << m[2][1] << ']' << std::endl;
		oss << '[' << std::setw(field_width) << m[0][2] << ", "
				<< std::setw(field_width) << m[1][2] << ", "
				<< std::setw(field_width) << m[2][2] << ']' << std::endl;
		return out << oss.str();
	}

	static float det2x2(float e00, float e01, float e10, float e11) {
		return determinant(mat2(e00, e01, e10, e11));
	}

	// Operator overload - assign
	//

	// add-assign
	inline mat3 & operator+=(const mat3 &rhs) {
		data[0] += rhs[0];
		data[1] += rhs[1];
		data[2] += rhs[2];
		return *this;
	}

	// add-assign
	inline mat3 & operator+=(float rhs) {
		data[0] += rhs;
		data[1] += rhs;
		data[2] += rhs;
		return *this;
	}

	// subtract-assign
	inline mat3 & operator-=(const mat3 &rhs) {
		data[0] -= rhs[0];
		data[1] -= rhs[1];
		data[2] -= rhs[2];
		return *this;
	}

	// subtract-assign
	inline mat3 & operator-=(float rhs) {
		data[0] -= rhs;
		data[1] -= rhs;
		data[2] -= rhs;
		return *this;
	}

	// This is a special case where we use matrix product
	// if you want component-wise multiplication see matrixCompMult(mat3, mat3)
	//
	// mulitply-assign
	inline mat3 & operator*=(const mat3 &rhs) {
		const mat3 lhs = *this;
		for (int i = 0; i < 3; i++) {
			(*this)[i] = vec3(0);
			for (int j = 0; j < 3; j++) {
				(*this)[i] += lhs[j] * rhs[i][j];
			}
		}
		return *this;
	}

	// mulitply-assign
	inline mat3 & operator*=(float rhs) {
		data[0] *= rhs;
		data[1] *= rhs;
		data[2] *= rhs;
		return *this;
	}

	// divide-assign
	inline mat3 & operator/=(const mat3 &rhs) {
		data[0] /= rhs[0];
		data[1] /= rhs[1];
		data[2] /= rhs[2];
		return *this;
	}

	// divide-assign
	inline mat3 & operator/=(float rhs) {
		data[0] /= rhs;
		data[1] /= rhs;
		data[2] /= rhs;
		return *this;
	}

};

// Matrix / Matrix Operator Overloads
// Matrix / Vector Operator Overloads
//

// negate
inline mat3 operator-(const mat3 &rhs) {
	mat3 m;
	m[0] = -rhs[0];
	m[1] = -rhs[1];
	m[2] = -rhs[2];
	return m;
}

// add
inline mat3 operator+(const mat3 &lhs, const mat3 &rhs) {
	mat3 m = lhs;
	return m += rhs;
}

// subtract
inline mat3 operator-(const mat3 &lhs, const mat3 &rhs) {
	mat3 m = lhs;
	return m -= rhs;
}

// This is a special case where we use matrix product
// if you want component-wise multiplication see matrixCompMult(mat3, mat3)
//
// multiply
inline mat3 operator*(const mat3 &lhs, const mat3 &rhs) {
	mat3 m = lhs;
	return m *= rhs;
}

// Left multiply mat3 m with vec3 v
//
// multiply
inline vec3 operator*(const mat3 &lhs, const vec3 &rhs) {
	vec3 v(0);
	for (int j = 0; j < 3; j++) {
		v += lhs[j] * rhs[j];
	}
	return v;
}

// Right multiply vec3 v with mat3 m
// Equvilent to transpose(m) * v
//
// mulitply-assign
inline vec3 operator*=(vec3 &lhs, const mat3 &rhs) {
	return vec3(dot(lhs, rhs[0]), dot(lhs, rhs[1]), dot(lhs, rhs[2]));
}

// multiply
inline vec3 operator*(const vec3 &lhs, const mat3 &rhs) {
	vec3 v = lhs;
	return v *= rhs;
}

// divide
inline mat3 operator/(const mat3 &lhs, const mat3 &rhs) {
	mat3 m = lhs;
	return m /= rhs;
}

// Vector / Scalar Operator Overloads
//

// add right
inline mat3 operator+(const mat3 &lhs, float rhs) {
	mat3 m = lhs;
	return m += rhs;
}

// add left
inline mat3 operator+(float lhs, const mat3 &rhs) {
	mat3 m = rhs;
	return m += lhs;
}

// subtract right
inline mat3 operator-(const mat3 &lhs, float rhs) {
	mat3 m = lhs;
	return m -= rhs;
}

// subtract left
inline mat3 operator-(float lhs, const mat3 &rhs) {
	vec3 v(lhs);
	mat3 m(v, v, v);
	return m -= rhs;
}

// multiply right
inline mat3 operator*(const mat3 &lhs, float rhs) {
	mat3 m = lhs;
	return m *= rhs;
}

// multiply left
inline mat3 operator*(float lhs, const mat3 &rhs) {
	mat3 m = rhs;
	return m *= lhs;
}

// divide right
inline mat3 operator/(const mat3 &lhs, float rhs) {
	mat3 m = lhs;
	return m /= rhs;
}

// divide left
inline mat3 operator/(float lhs, const mat3 &rhs) {
	vec3 v(lhs);
	mat3 m(v, v, v);
	return m /= rhs;
}

// Matrix functions
//

// determinant of matrix
inline float determinant(const mat3 &m) {
	float d = 0;
	d += m[0][0] * m[1][1] * m[2][2];
	d += m[0][1] * m[1][2] * m[2][0];
	d += m[0][2] * m[1][0] * m[2][1];
	d -= m[0][0] * m[1][2] * m[2][1];
	d -= m[0][1] * m[1][0] * m[2][2];
	d -= m[0][2] * m[1][1] * m[2][0];
	return d;
}

// inverse of matrix (error if not invertible)
inline mat3 inverse(const mat3 &m) {
	mat3 mi;
	// first column of cofactors, can use for determinant
	float c00 = mat3::det2x2(m[1][1], m[1][2], m[2][1], m[2][2]);
	float c01 = -mat3::det2x2(m[1][0], m[1][2], m[2][0], m[2][2]);
	float c02 = mat3::det2x2(m[1][0], m[1][1], m[2][0], m[2][1]);
	// get determinant by expanding about first column
	float invdet = 1 / (m[0][0] * c00 + m[0][1] * c01 + m[0][2] * c02);
	// FIXME proper detect infinite determinant
	assert(!isinf(invdet) && invdet == invdet && invdet != 0);
	// transpose of cofactor matrix * (1 / det)
	mi[0][0] = c00 * invdet;
	mi[1][0] = c01 * invdet;
	mi[2][0] = c02 * invdet;
	mi[0][1] = -mat3::det2x2(m[0][1], m[0][2], m[2][1], m[2][2]) * invdet;
	mi[1][1] = mat3::det2x2(m[0][0], m[0][2], m[2][0], m[2][2]) * invdet;
	mi[2][1] = -mat3::det2x2(m[0][0], m[0][1], m[2][0], m[2][1]) * invdet;
	mi[0][2] = mat3::det2x2(m[0][1], m[0][2], m[1][1], m[1][2]) * invdet;
	mi[1][2] = -mat3::det2x2(m[0][0], m[0][2], m[1][0], m[1][2]) * invdet;
	mi[2][2] = mat3::det2x2(m[0][0], m[0][1], m[1][0], m[1][1]) * invdet;
	return mi;
}

// transpose of matrix
inline mat3 transpose(const mat3 &m) {
	return mat3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2],
			m[1][2], m[2][2]);
}

// component-wise multiplication
// see (*) operator overload for matrix product
inline mat3 matrixCompMult(const mat3 &lhs, const mat3 &rhs) {
	mat3 m = lhs;
	m[0] *= rhs;
	m[1] *= rhs;
	m[2] *= rhs;
	return m;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////    _  _        _  _     __  __       _______ _____  _______   __                                                 ////
////   | || |      | || |   |  \/  |   /\|__   __|  __ \|_   _\ \ / /                                               ////
////   | || |___  _| || |_  | \  / |  /  \  | |  | |__) | | |  \ V /                                              ////
////   |__   _\ \/ /__   _| | |\/| | / /\ \ | |  |  _  /  | |   > <                                             ////
////      | |  >  <   | |   | |  | |/ ____ \| |  | | \ \ _| |_ / . \                                              ////
////      |_| /_/\_\  |_|   |_|  |_/_/    \_\_|  |_|  \_\_____/_/ \_\                                               ////
////                                                                                                                  ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class mat4 {
private:
	vec4 data[4];

	void clear(float i = 0) {
		data[0] = vec4(i, 0, 0, 0);
		data[1] = vec4(0, i, 0, 0);
		data[2] = vec4(0, 0, i, 0);
		data[3] = vec4(0, 0, 0, i);
	}

public:
	mat4() {
		clear();
	}

	explicit mat4(float i) {
		clear(i);
	}

	mat4(const vec4 &a, const vec4 &b, const vec4 &c, const vec4 &d) {
		data[0] = a;
		data[1] = b;
		data[2] = c;
		data[3] = d;
	}

	mat4(float e00, float e10, float e20, float e30, float e01, float e11,
			float e21, float e31, float e02, float e12, float e22, float e32,
			float e03, float e13, float e23, float e33) {
		data[0] = vec4(e00, e10, e20, e30);
		data[1] = vec4(e01, e11, e21, e31);
		data[2] = vec4(e02, e12, e22, e32);
		data[3] = vec4(e03, e13, e23, e33);
	}

	explicit operator float *() {
		return &(data[0].x);
	}

	const vec4 & operator[](size_t i) const {
		assert(i < 4);
		return data[i];
	}

	vec4 & operator[](size_t i) {
		assert(i < 4);
		return data[i];
	}

	// stream insertion
	inline friend std::ostream & operator<<(std::ostream &out, const mat4 &m) {
		const size_t field_width = 10;
		std::ostringstream oss;
		oss << std::setprecision(4);
		oss << '[' << std::setw(field_width) << m[0][0] << ", "
				<< std::setw(field_width) << m[1][0] << ", "
				<< std::setw(field_width) << m[2][0] << ", "
				<< std::setw(field_width) << m[3][0] << ']' << std::endl;
		oss << '[' << std::setw(field_width) << m[0][1] << ", "
				<< std::setw(field_width) << m[1][1] << ", "
				<< std::setw(field_width) << m[2][1] << ", "
				<< std::setw(field_width) << m[3][1] << ']' << std::endl;
		oss << '[' << std::setw(field_width) << m[0][2] << ", "
				<< std::setw(field_width) << m[1][2] << ", "
				<< std::setw(field_width) << m[2][2] << ", "
				<< std::setw(field_width) << m[3][2] << ']' << std::endl;
		oss << '[' << std::setw(field_width) << m[0][3] << ", "
				<< std::setw(field_width) << m[1][3] << ", "
				<< std::setw(field_width) << m[2][3] << ", "
				<< std::setw(field_width) << m[3][3] << ']';
		return out << oss.str();
	}

	static float det3x3(float e00, float e01, float e02, float e10, float e11,
			float e12, float e20, float e21, float e22) {
		float d = 0;
		d += e00 * e11 * e22;
		d += e01 * e12 * e20;
		d += e02 * e10 * e21;
		d -= e00 * e12 * e21;
		d -= e01 * e10 * e22;
		d -= e02 * e11 * e20;
		return d;
	}

	// Operator overload - assign
	//

	// add-assign
	inline mat4 & operator+=(const mat4 &rhs) {
		data[0] += rhs[0];
		data[1] += rhs[1];
		data[2] += rhs[2];
		data[3] += rhs[3];
		return *this;
	}

	// add-assign
	inline mat4 & operator+=(float rhs) {
		data[0] += rhs;
		data[1] += rhs;
		data[2] += rhs;
		data[3] += rhs;
		return *this;
	}

	// subtract-assign
	inline mat4 & operator-=(const mat4 &rhs) {
		data[0] -= rhs[0];
		data[1] -= rhs[1];
		data[2] -= rhs[2];
		data[3] -= rhs[3];
		return *this;
	}

	// subtract-assign
	inline mat4 & operator-=(float rhs) {
		data[0] -= rhs;
		data[1] -= rhs;
		data[2] -= rhs;
		data[3] -= rhs;
		return *this;
	}

	// This is a special case where we use matrix product
	// if you want component-wise multiplication see matrixCompMult(mat4, mat4)
	//
	// mulitply-assign
	inline mat4 & operator*=(const mat4 &rhs) {
		const mat4 lhs = *this;
		for (int i = 0; i < 4; i++) {
			(*this)[i] = vec4(0);
			for (int j = 0; j < 4; j++) {
				(*this)[i] += lhs[j] * rhs[i][j];
			}
		}
		return *this;
	}

	// mulitply-assign
	inline mat4 & operator*=(float rhs) {
		data[0] *= rhs;
		data[1] *= rhs;
		data[2] *= rhs;
		data[3] *= rhs;
		return *this;
	}

	// divide-assign
	inline mat4 & operator/=(const mat4 &rhs) {
		data[0] /= rhs[0];
		data[1] /= rhs[1];
		data[2] /= rhs[2];
		data[3] /= rhs[3];
		return *this;
	}

	// divide-assign
	inline mat4 & operator/=(float rhs) {
		data[0] /= rhs;
		data[1] /= rhs;
		data[2] /= rhs;
		data[3] /= rhs;
		return *this;
	}

};

// Matrix / Matrix Operator Overloads
// Matrix / Vector Operator Overloads
//

// negate
inline mat4 operator-(const mat4 &rhs) {
	mat4 m;
	m[0] = -rhs[0];
	m[1] = -rhs[1];
	m[2] = -rhs[2];
	m[3] = -rhs[3];
	return m;
}

// add
inline mat4 operator+(const mat4 &lhs, const mat4 &rhs) {
	mat4 m = lhs;
	return m += rhs;
}

// subtract
inline mat4 operator-(const mat4 &lhs, const mat4 &rhs) {
	mat4 m = lhs;
	return m -= rhs;
}

// This is a special case where we use matrix product
// if you want component-wise multiplication see matrixCompMult(mat4, mat4)
//
// multiply
inline mat4 operator*(const mat4 &lhs, const mat4 &rhs) {
	mat4 m = lhs;
	return m *= rhs;
}

// Left multiply mat4 m with vec4 v
//
// multiply
inline vec4 operator*(const mat4 &lhs, const vec4 &rhs) {
	vec4 v(0);
	for (int j = 0; j < 4; j++) {
		v += lhs[j] * rhs[j];
	}
	return v;
}

// Right multiply vec4 v with mat4 m
// Equvilent to transpose(m) * v
//
// mulitply-assign
inline vec4 operator*=(vec4 &lhs, const mat4 &rhs) {
	return vec4(dot(lhs, rhs[0]), dot(lhs, rhs[1]), dot(lhs, rhs[2]),
			dot(lhs, rhs[3]));
}

// multiply
inline vec4 operator*(const vec4 &lhs, const mat4 &rhs) {
	vec4 v = lhs;
	return v *= rhs;
}

// divide
inline mat4 operator/(const mat4 &lhs, const mat4 &rhs) {
	mat4 m = lhs;
	return m /= rhs;
}

// Vector / Scalar Operator Overloads
//

// add right
inline mat4 operator+(const mat4 &lhs, float rhs) {
	mat4 m = lhs;
	return m += rhs;
}

// add left
inline mat4 operator+(float lhs, const mat4 &rhs) {
	mat4 m = rhs;
	return m += lhs;
}

// subtract right
inline mat4 operator-(const mat4 &lhs, float rhs) {
	mat4 m = lhs;
	return m -= rhs;
}

// subtract left
inline mat4 operator-(float lhs, const mat4 &rhs) {
	vec4 v(lhs);
	mat4 m(v, v, v, v);
	return m -= rhs;
}

// multiply right
inline mat4 operator*(const mat4 &lhs, float rhs) {
	mat4 m = lhs;
	return m *= rhs;
}

// multiply left
inline mat4 operator*(float lhs, const mat4 &rhs) {
	mat4 m = rhs;
	return m *= lhs;
}

// divide right
inline mat4 operator/(const mat4 &lhs, float rhs) {
	mat4 m = lhs;
	return m /= rhs;
}

// divide left
inline mat4 operator/(float lhs, const mat4 &rhs) {
	vec4 v(lhs);
	mat4 m(v, v, v, v);
	return m /= rhs;
}

// Matrix functions
//

// determinant of matrix
inline float determinant(const mat4 &m) {
	float d = 0;
	// expand about first column
	d += m[0][0]
			* mat4::det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3],
					m[3][1], m[3][2], m[3][3]);
	d -= m[0][1]
			* mat4::det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3],
					m[3][0], m[3][2], m[3][3]);
	d += m[0][2]
			* mat4::det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3],
					m[3][0], m[3][1], m[3][3]);
	d -= m[0][3]
			* mat4::det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2],
					m[3][0], m[3][1], m[3][2]);
	return d;
}

// inverse of matrix (error if not invertible)
inline mat4 inverse(const mat4 &m) {
	mat4 mi;
	// first column of cofactors, can use for determinant
	float c00 = mat4::det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2],
			m[2][3], m[3][1], m[3][2], m[3][3]);
	float c01 = -mat4::det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2],
			m[2][3], m[3][0], m[3][2], m[3][3]);
	float c02 = mat4::det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1],
			m[2][3], m[3][0], m[3][1], m[3][3]);
	float c03 = -mat4::det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1],
			m[2][2], m[3][0], m[3][1], m[3][2]);
	// get determinant by expanding about first column
	float invdet = 1
			/ (m[0][0] * c00 + m[0][1] * c01 + m[0][2] * c02 + m[0][3] * c03);
	// FIXME proper detect infinite determinant
	assert(!isinf(invdet) && invdet == invdet && invdet != 0);
	// transpose of cofactor matrix * (1 / det)
	mi[0][0] = c00 * invdet;
	mi[1][0] = c01 * invdet;
	mi[2][0] = c02 * invdet;
	mi[3][0] = c03 * invdet;
	mi[0][1] = -mat4::det3x3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2],
			m[2][3], m[3][1], m[3][2], m[3][3]) * invdet;
	mi[1][1] = mat4::det3x3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2],
			m[2][3], m[3][0], m[3][2], m[3][3]) * invdet;
	mi[2][1] = -mat4::det3x3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1],
			m[2][3], m[3][0], m[3][1], m[3][3]) * invdet;
	mi[3][1] = mat4::det3x3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1],
			m[2][2], m[3][0], m[3][1], m[3][2]) * invdet;
	mi[0][2] = mat4::det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2],
			m[1][3], m[3][1], m[3][2], m[3][3]) * invdet;
	mi[1][2] = -mat4::det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2],
			m[1][3], m[3][0], m[3][2], m[3][3]) * invdet;
	mi[2][2] = mat4::det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1],
			m[1][3], m[3][0], m[3][1], m[3][3]) * invdet;
	mi[3][2] = -mat4::det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1],
			m[1][2], m[3][0], m[3][1], m[3][2]) * invdet;
	mi[0][3] = -mat4::det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2],
			m[1][3], m[2][1], m[2][2], m[2][3]) * invdet;
	mi[1][3] = mat4::det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2],
			m[1][3], m[2][0], m[2][2], m[2][3]) * invdet;
	mi[2][3] = -mat4::det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1],
			m[1][3], m[2][0], m[2][1], m[2][3]) * invdet;
	mi[3][3] = mat4::det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1],
			m[1][2], m[2][0], m[2][1], m[2][2]) * invdet;
	return mi;
}

// transpose of matrix
inline mat4 transpose(const mat4 &m) {
	return mat4(m[0][0], m[1][0], m[2][0], m[3][0], m[0][1], m[1][1], m[2][1],
			m[3][1], m[0][2], m[1][2], m[2][2], m[3][2], m[0][3], m[1][3],
			m[2][3], m[3][3]);
}

// component-wise multiplication
// see (*) operator overload for matrix product
inline mat4 matrixCompMult(const mat4 &lhs, const mat4 &rhs) {
	mat4 m = lhs;
	m[0] *= rhs;
	m[1] *= rhs;
	m[2] *= rhs;
	m[3] *= rhs;
	return m;
}
}
