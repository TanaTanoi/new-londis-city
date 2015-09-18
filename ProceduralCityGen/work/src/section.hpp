#include <vector>

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
};

class SectionDivider {
private:
	float goalArea;

	line findLongestEdge(section);
	std::vector<section> recDivideSection(section);

public:
	void renderSections();
	void divideSection(section);
};