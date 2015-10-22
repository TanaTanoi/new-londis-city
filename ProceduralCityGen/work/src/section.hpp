#pragma once


#include <vector>
#include "building.hpp"
#include "utility.hpp"



struct lot{
	util::section boundingBox;
	std::vector<util::section> sections;
	int ID;
	buildingLOD buildings;
};

class SectionDivider {
private:
	float goalArea = 1500; // Does not work at 11000 FIXME
	float minArea = 200;
	std::vector<lot> lots;
	int sectionID = 0;
	int lotID =0;
	bool fill = true;
	//for testing
	std::vector<util::section> sections;

	//Section Methods
	util::line findLongestEdge(util::section);
	lot recDivideSection(lot,util::section);
	std::vector<util::section> splitSection(util::section);
	void renderSection(util::section);
	comp308::vec2 getSharedPoint(util::line a, util::line b);
	util::section getInnerSection(util::section, util::line, util::line, util::line);
	float getSectionSize(util::section);
	lot removeUnusableSections(lot);
	bool hasStreetAccess(util::section, lot);
	void divideLot(lot);



	void renderPoly();
	void renderWireFrame();
	//bool intersects(line *, float, float, line *);

	//Lot Methods


public:
	SectionDivider();

	void cleanUp();
	void renderSections();
	void divideAllLots(std::vector<util::section>);
	void addBuildingToLot(lot);
	void changeFill();
	lot testSection(); // for testing
	void renderTest(); // for testing

	inline const std::vector<lot>& getLots() const {
		return lots;
	}
};
