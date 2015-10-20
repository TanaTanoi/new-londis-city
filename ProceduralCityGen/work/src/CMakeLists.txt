#
# COMP 308
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	"comp308.hpp"
	"main.hpp"
	"building.hpp"
	"shaderLoader.hpp"
	"imageLoader.hpp"
	"section.hpp"
	"generator.h"
	"vehicle.hpp"
	"vehicleController.hpp"
	"utility.hpp"
	"roadNetwork.hpp"
	"spline.hpp"
	"cycleUtil.hpp"
	"aStarSearchAlgo.hpp"
	"VEvent.hpp"
	"VEdge.hpp"
	"Voronoi.hpp"
)


# TODO list your source files (.cpp) here
SET(sources
	"main.cpp"
	"building.cpp"
	"section.cpp"
	"generator.cpp"
	"vehicleController.cpp"
	"vehicle.cpp"
	"roadNetwork.cpp"
	"spline.cpp"
	"aStarSearchAlgo.cpp"
	"Voronoi.cpp"
)

# Add executable target and link libraries
# You do not need to touch this
add_executable(${COMP308_ASSIGNMENT} ${headers} ${sources})
target_link_libraries(${COMP308_ASSIGNMENT} PRIVATE glew glfw ${GLFW_LIBRARIES})
target_link_libraries(${COMP308_ASSIGNMENT} PRIVATE stb)
