#include <vector>
#include "building.hpp"

/**
		A line represents a single line in a section outline
*/
struct line {
	float x1;
	float y1;
	float x2;
	float y2;

	float length;
};

struct section {
	std::vector<line> lines;
	float area;
	Building::Building building;
};

class SectionDivider {
private:
	float goalArea;
	std::vector<section> sections;

	line findLongestEdge(section *);
	void recDivideSection(section *);
	std::vector<section> splitSection(section *);

public:
	SectionDivider();
	void renderSections();
	void divideSection(section);
};