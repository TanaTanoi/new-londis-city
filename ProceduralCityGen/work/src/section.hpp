#include <vector>
#include "building.hpp"

/**
		A line represents a single line in a section outline
*/
struct line {
	comp308::vec2 start;
	comp308::vec2 end;
	int ID;
	float length;
};

struct section {
	std::vector<line > lines;
	int ID;
	//float area;
	//Building:: building;
};

struct lot{
	section boundingBox;
	std::vector<section > sections;
};

class SectionDivider {
private:
	float goalArea;
	std::vector<lot> lots;
	//for testing
	std::vector<section> sections;

	//Section Methods
	line findLongestEdge(section);
	void recDivideSection(section *);
	std::vector<section> splitSection(section *);
	void renderSection(section *);
	bool intersects(line *, float, float, line *);

	//Lot Methods


public:
	SectionDivider();

	void cleanUp();
	void renderSections();
	void divideLot(lot);
	void testSection(); // for testing
	void renderTest(); // for testing
};
