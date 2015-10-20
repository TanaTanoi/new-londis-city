/*
 * Voronio.cpp
 *
 *  Created on: 19/10/2015
 *      Author: craighhann
 */

#include "Voronoi.hpp"
#include "comp308.hpp"
#include "utility.hpp"

#include "VEvent.hpp"
#include "Parabola.hpp"
#include "cycleUtil.hpp"
#include <iostream>
#include <algorithm>
#include <set>
#include <functional>
#include <vector>
//changed branch
using namespace vor;
using namespace util;
using namespace cycle;
using namespace std;

Voronoi::Voronoi(){}

edges Voronoi::GetEdges(vertices v, int w, int h){
	places = v;
	width = w;
	height = h;
	//root = Parabola(-1);

	edgeList.clear(); // clears all edges
	points.clear(); // clears all points

	cout <<"___________ Adding Points _____________" << endl;
	for(vertices::iterator i = places.begin(); i!=places.end(); ++i){
		vec2 rn = *i;
		VEvent newEvent = VEvent( rn, true, eventID++);
		cout << "Pushing ID " << newEvent.ID << "  "<< rn.x << " " << rn.y << endl;
		queue.push(newEvent);
	}

	cout << "Pushed all vertices" << endl;

	VEvent e;
	while(!queue.empty()){
		e = queue.top();
		queue.pop();
		ly = e.point.y;
		cout << "Popped event" << e.ID << " at y: " << ly << endl;
		if(deleted.find(e) != deleted.end()) {cout << "Deleted erased" << endl;	deleted.erase(e); continue;}
		if(e.pe){cout <<"Inserting Parabola" << endl; InsertParabola(e.point);}
		else{cout << "Removing Parabola" << endl; RemoveParabola(e);}
	}

	cout << "Processed all events" << endl;

	FinishEdge(parabolas[rootID]);

	cout << "Finished edge" << endl;

	for(edges::iterator i = edgeList.begin(); i != edgeList.end(); ++i)	{
		if( (*i).neighbourID == -1){
			i->edge.start = points[(*i).neighbourID]; // HALP XXX
			i->neighbourID = -1;
		}
	}

	return edgeList;
}

void Voronoi::InsertParabola(vec2 p){
	if((int)parabolas.size() == 0){parabolas.push_back(Parabola(p,parabolaID++)); return;}
	cout << "Not root" << endl;
	Parabola root = parabolas[rootID];
	cout << "got root " << endl;
	if(root.isLeaf && root.site.y - p.y < 1) {
		cout <<"root is a leaf" << endl;
		vec2 fp = root.site;
		root.isLeaf = false;

		Parabola pLeft =  Parabola(fp,parabolaID++);
		Parabola pRight =  Parabola(p,parabolaID++);
		parabolas.push_back(pLeft);
		parabolas.push_back(pRight);
		root.left = pLeft.ID;
		root.right = pRight.ID;

		roadNode s;
		s.location = vec2((p.x + fp.x)/2, height);
		s.ID = nodeID++;

		points.push_back(s);
		if(p.x > fp.x) root.edge = VEdge(s, fp, p, edgeID++);
		else root.edge = VEdge(s, p, fp, edgeID++);
		parabolas[rootID] = root; // copies across changes
		edgeList.push_back(root.edge);
		return;
	}
	Parabola par = GetParabolaByX(p.x);
	cout << "Par is " << par.ID<<endl;
	cout << "cEvent " << par.cEvent.ID << endl;
	if(par.cEvent.ID != -1)	{
		cout <<"Proccesed this event " << endl;
 		par.cEvent.ID = -1;
		deleted.insert(par.cEvent);
		parabolas[par.ID] = par;
	}

	roadNode start;
	start.location = vec2(p.x, GetY(par.site, p.x));
	points.push_back(start);

	VEdge el = VEdge(start, par.site, p,edgeID++);
	VEdge er = VEdge(start, p, par.site,edgeID++);

	el.neighbourID = er.ID;
	edgeList.push_back(el);

	par.edge = er;
	par.isLeaf = false;

	Parabola p0 = Parabola(par.site, parabolaID++);
	Parabola p1 = Parabola(p, parabolaID++);
	Parabola p2 = Parabola(par.site, parabolaID++);
	parabolas.push_back(p0);
	parabolas.push_back(p1);
	parabolas.push_back(p2);

	par.right = p2.ID;
	Parabola blank = Parabola(parabolaID++); // does this need ID?
	blank.edge = el;
	parabolas.push_back(blank);
	par.left = blank.ID;

	parabolas[par.left].left = p0.ID;
	parabolas[par.left].right = p1.ID;

	parabolas[par.ID] = par; // copies changes across

	CheckCircle(p0);
	CheckCircle(p2);
}

void Voronoi::RemoveParabola(VEvent e){
	Parabola p1 = parabolas[e.archIndex];

	Parabola xl = GetLeftParent(p1);
	Parabola xr = GetRightParent(p1);

	Parabola p0 = GetLeftChild(xl);
	Parabola p2 = GetRightChild(xr);

	if(p0.cEvent.ID != -1){ deleted.insert(p0.cEvent); parabolas[p0.ID].cEvent.ID = -1;}
	if(p2.cEvent.ID != -1){ deleted.insert(p2.cEvent); parabolas[p2.ID].cEvent.ID = -1;}

	roadNode p;
	p.location = vec2(e.point.x, GetY(p1.site, e.point.x));
	p.ID = nodeID++;
	points.push_back(p);

	xl.edge.edge.end = p;
	xr.edge.edge.end = p;

	Parabola higher = Parabola();
	Parabola par = p1;
	while(par.ID != rootID) // will root always be at 0?
	{
		par = parabolas[par.parent];
		if(par.ID == xl.ID) higher = xl;
		if(par.ID == xr.ID) higher = xr;
	}
	if(higher.ID >= 0){
		higher.edge = VEdge(p, p0.site, p2.site, edgeID++);
	}

	edgeList.push_back(higher.edge);

	Parabola gparent = parabolas[parabolas[p1.parent].parent];
	if(parabolas[p1.parent].left == p1.ID)
	{
		if(gparent.left  == p1.parent) gparent.left =  parabolas[p1.parent].right;
		if(gparent.right == p1.parent) gparent.right = parabolas[p1.parent].right;
		parabolas[gparent.ID] = gparent;
	}
	else
	{
		if(gparent.left  == p1.parent) gparent.left = parabolas[p1.parent].left;
		if(gparent.right == p1.parent) gparent.right = parabolas[p1.parent].left;
		parabolas[gparent.ID] = gparent;
	}

	int parID = parabolas[p1.parent].ID;
	int p1ID = p1.ID;

	parabolas[p1.parent].ID = -1;
	p1.ID = -1;

	parabolas[parID] =  parabolas[p1.parent];
	parabolas[p1ID] = p1;

	CheckCircle(p0);
	CheckCircle(p2);
}

void Voronoi::FinishEdge(Parabola n){
	if(n.isLeaf) {cout << "N is a leaf" << endl; int ID  = n.ID; n.ID = -1; parabolas[ID] = n; return;} // what do I do here?
	cout << "N is not a leaf" << endl;
	double mx;
	//struct primitive{
	if(n.edge.direction.x > 0.0){ mx = max(width, double(n.edge.edge.start.location.x) + 10 );}
	else{ mx = min(0.0, double(n.edge.edge.start.location.x) - 10);}

	roadNode end;
	end.location = vec2(mx, mx * n.edge.f + n.edge.g);
	cout<<"MX "<<mx << "mx * n.edge.f + n.edge.g " << mx * n.edge.f + n.edge.g <<endl;
	n.edge.edge.end.location = end.location;
	points.push_back(end);
	parabolas[n.ID] = n;
	Parabola thingy = parabolas[n.ID];
	cout<<"p end " << thingy.edge.edge.end.location.x <<  " "<<thingy.edge.edge.end.location.y << endl;
	if(n.left == -1){cout << "Left is neg 1 " << endl;}
	FinishEdge(parabolas[n.left]);
	if(n.right == -1){cout << "Right is neg 1 " << endl;}
	FinishEdge(parabolas[n.right]);
	parabolas[n.ID].ID = -1;
	//}
}

double	Voronoi::GetXOfEdge(Parabola par, double y){
	Parabola left = GetLeftChild(par);
	Parabola right= GetRightChild(par);

	vec2 p = left.site;
	vec2 r = right.site;

	double dp = 2.0 * (p.y - y);
	double a1 = 1.0 / dp;
	double b1 = -2.0 * p.x / dp;
	double c1 = y + dp / 4 + p.x * p.x / dp;

	dp = 2.0 * (r.y - y);
	double a2 = 1.0 / dp;
	double b2 = -2.0 * r.x/dp;
	double c2 = ly + dp / 4 + r.x * r.x / dp;

	double a = a1 - a2;
	double b = b1 - b2;
	double c = c1 - c2;

	double disc = b*b - 4 * a * c;
	double x1 = (-b + sqrt(disc)) / (2*a);
	double x2 = (-b - sqrt(disc)) / (2*a);

	double ry;
	if(p.y < r.y ) ry =  max(x1, x2);
	else ry = min(x1, x2);

	cout << "Returning x of edge" << endl;

	return ry;
}

Parabola Voronoi::GetParabolaByX(double xx){
	Parabola par = parabolas[rootID];
	double x = 0.0;

	while(!par.isLeaf){
		cout <<"I'm stucj healp" << endl;
		x = GetXOfEdge(par, ly);
		if(x>xx) par = parabolas[par.left];
		else par = parabolas[par.right];
	}
	return par;
}

double Voronoi::GetY(vec2 p, double x){
	double dp = 2 * (p.y - ly);
	double a1 = 1 / dp;
	double b1 = -2 * p.x / dp;
	double c1 = ly + dp / 4 + p.x * p.x / dp;

	return(a1*x*x + b1*x + c1);
}

void Voronoi::CheckCircle(Parabola b){
	cout << "Checking circle " << endl;
	Parabola lp = GetLeftParent(b);
	Parabola rp = GetRightParent(b);

	cout << "Check circle has parent parabolas " << endl;

	Parabola a  = GetLeftChild (lp);
	Parabola c  = GetRightChild(rp);

	cout << "Check circle has child parabolas " << endl;

	if(a.ID == -1 || c.ID == -1 || vecEqual(a.site, c.site)) return;
	roadNode s;
	s = GetEdgeIntersection(lp.edge.edge, rp.edge.edge);
	if(s.ID == -1) return;

	double dx = a.site.x - s.location.x;
	double dy = a.site.y - s.location.y;

	double d = sqrt( (dx * dx) + (dy * dy) );

	if(s.location.y - d >= ly) { return;}

	s.location.y = s.location.y - d;
	VEvent e = VEvent(s.location, false,eventID++);
	roadNode newNode = {e.point, nodeID++};
	points.push_back(newNode);
	e.archIndex = b.ID;
	b.cEvent = e;
	parabolas[b.ID] = b; // relays changes into parabolas

	queue.push(e);

	cout << "checked circle" << endl;
}

roadNode Voronoi::GetEdgeIntersection(road a, road b){
	roadNode r;
	if(!intersects(a.start.location,a.end.location,b.start.location, b.end.location)){
		r.ID = -1;
		return r;
	}
	vec2 inter = getIntersection(a.start.location,a.end.location,b.start.location, b.end.location);
	r.location = inter;
	r.ID = nodeID++;
	points.push_back(r);
	return r;
}

Parabola Voronoi::GetLeft(Parabola p){
	return GetLeftChild(GetLeftParent(p));
}

Parabola Voronoi::GetRight(Parabola p){
	return GetRightChild(GetRightParent(p));
}

Parabola Voronoi::GetLeftParent(Parabola p){
	if(p.parent == -1){return Parabola();}
	Parabola par = parabolas[p.parent];
	Parabola pLast = p;
	while(par.left == pLast.ID){
		if(par.parent == -1){return Parabola();}
		pLast = par;
		par = parabolas[par.parent];
	}
	return par;
}
Parabola Voronoi::GetRightParent(Parabola p){
	if(p.parent == -1){return Parabola();}
	Parabola par = parabolas[p.parent];
	Parabola pLast = p;
	while(par.right == pLast.ID){
		if(par.parent == -1){return Parabola();}
		pLast = par;
		par = parabolas[par.parent];
	}
	return par;
}

Parabola Voronoi::GetLeftChild(Parabola p){
	if(p.ID == -1){return p;}
	if(p.left == -1){return Parabola();}
	Parabola par = parabolas[p.left];
	while(!par.isLeaf){
		if(par.right == -1){return Parabola();}
		cout << "eternal loop" << endl;
		par = parabolas[par.right];
	}
	return par;
}

Parabola Voronoi::GetRightChild(Parabola p){
	if(p.ID == -1){return p;}
	if(p.right == -1){return Parabola();}
	Parabola par = parabolas[p.right];
	while(!par.isLeaf){
		if(par.left == -1){return Parabola();}
		par = parabolas[par.left];
	}
	return par;
}




