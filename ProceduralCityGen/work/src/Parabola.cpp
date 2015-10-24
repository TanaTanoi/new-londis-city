/*
 * Parabola.cpp
 *
 *  Created on: 19/10/2015
 *      Author: craighhann
 */

#include "utility.hpp"
#include "comp308.hpp"
#include "Parabola.hpp"

using namespace util;
using namespace comp308;



/*
	Constructors
*/

Parabola::Parabola()
{
	site	= 0;
	isLeaf	= false;
	cEvent	= 0;
	edge	= 0;
	parent	= 0;
}

Parabola::Parabola(vec2 * s)
{
	site	= s;
	isLeaf	= true;
	cEvent	= 0;
	edge	= 0;
	parent	= 0;
}

/*
	Tree operations (described in the header file)
*/

Parabola * Parabola::GetLeft			(Parabola * p)
{
	return GetLeftChild(GetLeftParent(p));
}


Parabola * Parabola::GetRight			(Parabola * p)
{
	return GetRightChild(GetRightParent(p));
}

Parabola * Parabola::GetLeftParent	(Parabola * p)
{
	Parabola * par		= p->parent;
	Parabola * pLast	= p;
	while(par->Left() == pLast)
	{
		if(!par->parent) return 0;
		pLast = par;
		par = par->parent;
	}
	return par;
}

Parabola * Parabola::GetRightParent	(Parabola * p)
{
	Parabola * par		= p->parent;
	Parabola * pLast	= p;
	while(par->Right() == pLast)
	{
		if(!par->parent) return 0;
		pLast = par; par = par->parent;
	}
	return par;
}

Parabola * Parabola::GetLeftChild		(Parabola * p)
{
	if(!p) return 0;
	Parabola * par = p->Left();
	while(!par->isLeaf) par = par->Right();
	return par;
}

Parabola * Parabola::GetRightChild	(Parabola * p)
{
	if(!p) return 0;
	Parabola * par = p->Right();
	while(!par->isLeaf) par = par->Left();
	return par;
}


