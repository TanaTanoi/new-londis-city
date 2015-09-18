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

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>

#include "comp308.hpp"

namespace comp308 {

	class shader_error : public std::runtime_error {
	public:
		explicit shader_error(const std::string &what_ = "Generic shader error.") : std::runtime_error(what_) { }
	};

	class shader_compile_error : public shader_error {
	public:
		explicit shader_compile_error(const std::string &what_ = "Shader compilation failed.") : shader_error(what_) { }
	};

	class shader_link_error : public shader_error {
	public:
		explicit shader_link_error(const std::string &what_ = "Shader program linking failed.") : shader_error(what_) { }
	};

	inline void printShaderInfoLog(GLuint obj) {
		int infologLength = 0;
		int charsWritten = 0;
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 1) {
			std::vector<char> infoLog(infologLength);
			glGetShaderInfoLog(obj, infologLength, &charsWritten, &infoLog[0]);
			std::cout << "SHADER:" << std::endl << &infoLog[0] << std::endl;
		}
	}

	inline void printProgramInfoLog(GLuint obj) {
		int infologLength = 0;
		int charsWritten = 0;
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 1) {
			std::vector<char> infoLog(infologLength);
			glGetProgramInfoLog(obj, infologLength, &charsWritten, &infoLog[0]);
			std::cout << "PROGRAM:" << std::endl << &infoLog[0] << std::endl;
		}
	}

	inline GLuint compileShader(GLenum type, const std::string &text) {
		GLuint shader = glCreateShader(type);
		const char *text_c = text.c_str();
		glShaderSource(shader, 1, &text_c, nullptr);
		glCompileShader(shader);
		GLint compile_status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
		if (!compile_status) {
			printShaderInfoLog(shader);
			throw shader_compile_error();
		}
		// always print, so we can see warnings
		printShaderInfoLog(shader);
		return shader;
	}

	inline void linkShaderProgram(GLuint prog) {
		glLinkProgram(prog);
		GLint link_status;
		glGetProgramiv(prog, GL_LINK_STATUS, &link_status);
		if (!link_status) {
			printProgramInfoLog(prog);
			throw shader_link_error();
		}
		// always print, so we can see warnings
		printProgramInfoLog(prog);
	}

	inline GLuint makeShaderProgram(const std::string &vertSource, const std::string &fragSource) {
		GLuint prog = glCreateProgram();

		std::ifstream fileStream(vertSource);

		if (fileStream) {
			std::ostringstream buffer;
			buffer << fileStream.rdbuf();
			auto vertshader = compileShader(GL_VERTEX_SHADER, buffer.str());
			glAttachShader(prog, vertshader);
		} else {
			throw std::runtime_error("Failed to open file " + vertSource);
		}


		fileStream = std::ifstream(fragSource);

		if (fileStream) {
			std::ostringstream buffer;
			buffer << fileStream.rdbuf();
			auto vertshader = compileShader(GL_FRAGMENT_SHADER, buffer.str());
			glAttachShader(prog, vertshader);
		} else {
			throw std::runtime_error("Failed to open file " + fragSource);
		}

		linkShaderProgram(prog);
		return prog;
	}
}