/*
 * Parabola.hpp
 *
 *  Created on: 19/10/2015
 *      Author: craighhann
 */
#pragma once

#include "comp308.hpp"
#include "utility.hpp"
#include "cycleUtil.hpp"
#include "VEdge.hpp"
#include "VEvent.hpp"


class Parabola{

private:

public:

	bool isLeaf; // whether the node is internal or a leaf
	VEvent cEvent; //an arch of parabola or an intersection between two archs (which defines an edge).
	VEdge edge; // edge
	int parent; // parent node in tree
	int left;
	int right;

	int ID;
	comp308::vec2 site;// focus point of parabola

	Parabola(comp308::vec2 rn, int id){
		isLeaf = true;
		site = rn;
		ID = id;
		parent = -1;
		right = -1;
		left = -1;} // note an ID of - 1 means null

	Parabola(){
		isLeaf = false;
	site = vec2();
	ID = -1;
	parent = -1;
	right = -1;
	left = -1;}

	Parabola(int id){
		isLeaf = false;
		ID = id;		parent = -1;
	right = -1;
	left = -1;}

//	static Parabola  GetLeft(Parabola p);
//	static Parabola  GetRight(Parabola p);
//	static Parabola  GetLeftParent(Parabola p);
//	static Parabola  GetRightParent(Parabola p);
//	static Parabola  GetLeftChild(Parabola p);
//	static Parabola  GetRightChild(Parabola p);

};
