#include <vector>
#include "building.hpp"

/**
		A line represents a single line in a section outline
*/
struct line {
	comp308::vec2 start;
	comp308::vec2 end;
	float length;
};

struct section {
	std::vector<line> lines;
	float area;
	//Building:: building;
};

class SectionDivider {
private:
	float goalArea;
	std::vector<section> sections;

	line* findLongestEdge(section *);
	void recDivideSection(section *);
	std::vector<section> splitSection(section *);
	void renderSection(section *);
	bool intersects(line *, float, float, line *);

public:
	SectionDivider();
	void renderSections();
	void divideSection(section);
	void testSection(); // for testing 
	void renderTest(); // for testing 
};