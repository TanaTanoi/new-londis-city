#pragma once


#include <vector>
#include "building.hpp"
#include "utility.hpp"



struct section {
	std::vector<util::line > lines;
	int ID;
	float area;
	//Building:: building;
};

struct lot{
	section boundingBox;
	std::vector<section > sections;
	int ID;
};

class SectionDivider {
private:
	float goalArea = 5500; // Does not work at 11000 FIXME
	float minArea = 500;
	std::vector<lot> lots;
	int sectionID = 0;
	//for testing
	std::vector<section> sections;

	//Section Methods
	util::line findLongestEdge(section);
	lot recDivideSection(lot, section);
	std::vector<section> splitSection(section);
	void renderSection(section);
	comp308::vec2 getSharedPoint(util::line a, util::line b);
	section getInnerSection(section, util::line, util::line, util::line);
	float getSectionSize(section);
	lot removeUnusableSections(lot);
	bool hasStreetAccess(section, lot);


	void renderPoly();
	void renderWireFrame();
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
