#ifndef EDGE_H
#define EDGE_H

#include <cstdlib>
#include <cassert>
#include <iostream>
#include "vertex.h"

using namespace std;

class vertex;

class edge
{
	private:
		int _cost;
		int _upper12; //capacity
		int _upper21; //capacity
		int _flow;
		vertex& _v1;
		vertex& _v2;

	public:
		edge(vertex&, vertex&, int, int, int);
		~edge();
		int cost();
		int flow();
		int residue(vertex&);
		void update_flow(vertex&, int);
		vertex& v1();
		vertex& v2();
		vertex& opposite(vertex& v);
};

edge::edge(vertex& v1, vertex& v2, int upper12 = 0, int upper21 = 0, int c = 0) :
	_v1(v1), _v2(v2), _cost(c), _flow(0), _upper12(upper12), _upper21(upper21)
{
	v1.edges.push_back(this);
	v2.edges.push_back(this);
}

int edge::cost() { return _cost; }

int edge::flow() { return _flow; }

vertex& edge::v1() { return _v1; }

vertex& edge::v2() { return _v2; }

int edge::residue(vertex& v)
{
	if (&v == &_v1)
		return _upper12 - _flow;
	else {
		assert(&v == &_v2);
		return _flow;
	}	
}

void edge::update_flow(vertex& v, int i)
{
	assert(i > 0);
	if (&v == &_v1)
		_flow += i;
	else {
		assert(&v == &_v2);
		_flow -= i;
	}
}

#endif