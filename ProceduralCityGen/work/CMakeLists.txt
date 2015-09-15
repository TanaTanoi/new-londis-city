################################################################
#   ______ _____  ______  ______      ________ ______  _____   #
#  / _____) ___ \|  ___ \(_____ \    (_______// __   |/ ___ \  #
# | /    | |   | | | _ | |_____) )      ____ | | //| ( (   ) ) #
# | |    | |   | | || || |  ____/      (___ \| |// | |> > < <  #
# | \____| |___| | || || | |          _____) )  /__| ( (___) ) #
#  \______)_____/|_||_||_|_|         (______/ \_____/ \_____/  #
#                                                              #
#                 ______             ___ _                     #
#                / _____)           / __|_)                    #
#               | /      ___  ____ | |__ _  ____               #
#               | |     / _ \|  _ \|  __) |/ _  |              #
#               | \____| |_| | | | | |  | ( ( | |              #
#                \______)___/|_| |_|_|  |_|\_|| |              #
#                                         (_____|              #
#                                                              #
################################################################
#                          Created by:                         #
#                    Ben Allen & Joshua Scott                  #
################################################################

# require new behaviour of: CMP0054
cmake_minimum_required(VERSION 3.1)

# Assignment Name
set(COMP308_ASSIGNMENT "RandCityGen" CACHE STRING "COMP308 Assignment Name")

# Project
project("COMP308_${COMP308_ASSIGNMENT}" CXX C)

# Enable IDE Project Folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

#########################################################
# Force Output Directories
# In general, this isn't a very friendly thing to do, but
# we'll do it anyway so the exes are in a reliable place.
#########################################################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

#########################################################
# Find OpenGL
#########################################################
find_package(OpenGL REQUIRED)

#########################################################
# Include GLFW Subproject
#########################################################
add_subdirectory("${PROJECT_SOURCE_DIR}/ext/glfw")
include_directories("${PROJECT_SOURCE_DIR}/ext/glfw/include")

#########################################################
# Include GLEW Subproject
#########################################################
add_subdirectory("${PROJECT_SOURCE_DIR}/ext/glew-1.10.0")

#########################################################
# Set Compiler Flags
#########################################################
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# Full normal warnings
	add_compile_options(/W4)
	# Ignore C4800: forcing X to bool (performance warning)
	add_compile_options(/wd4800)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# C++14
	add_compile_options(-std=c++1y)
	# Full normal warnings
	add_compile_options(-Wall -Wextra -pedantic)
	# Promote missing return to error
	add_compile_options(-Werror=return-type)
	# enable coloured output if gcc >= 4.9
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	if (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9)
		add_compile_options(-fdiagnostics-color)
	endif()
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
	# C++14
	add_compile_options(-std=c++1y)
	# Full normal warnings
	add_compile_options(-Wall -Wextra -pedantic)
	# Promote missing return to error
	add_compile_options(-Werror=return-type)
endif()

#########################################################
# Source Files
#########################################################
add_subdirectory(src)
set_property(TARGET ${COMP308_ASSIGNMENT} PROPERTY FOLDER "COMP308")





