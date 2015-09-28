#pragma once


#include <vector>
#include "building.hpp"
#include "utility.hpp"



struct section {
	std::vector<util::line > lines;
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
	util::line findLongestEdge(section);
	void recDivideSection(section);
	std::vector<section> splitSection(section);
	void renderSection(section);
	comp308::vec2 getSharedPoint(util::line a, util::line b);
	section getInnerSection(section, util::line, util::line, util::line);
	//bool intersects(line *, float, float, line *);

	//Lot Methods


public:
	SectionDivider();

	void cleanUp();
	void renderSections();
	void divideLot(lot);
	void testSection(); // for testing
	void renderTest(); // for testing
};
