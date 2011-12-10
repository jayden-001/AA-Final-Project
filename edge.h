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
		int _upper;
		int _residue;
		edge* _reverse;
		vertex* _v;
		vertex* _v_op;
	public:
		edge();
		void set_edge(vertex* v, vertex* _v_op, int upper, edge* reverse);
		
		int flow() { return _upper - _residue; }
		int upper() { return _upper; }
		int residue() { return _residue; };
		void add_residue(int i) { _residue += i; }
		void set_residue(int i) { _residue = i; }
		void push_flow(int);
		edge *reverse() { return _reverse; }
		vertex* v() { return _v; }
		vertex* v_op() { return _v_op; }
};

edge::edge() : _v(NULL), _residue(0), _upper(0)
{}

void edge::set_edge(vertex* v, vertex* v_op, int upper, edge* reverse)
{
	_v = v;
	_v_op = v_op;
	_upper = upper;
	_residue = upper;
	_reverse = reverse;
	
	v->edges()->push_back(this);
}

void edge::push_flow(int i)
{
	assert(i <= _residue);
	_residue -= i;
	_reverse->add_residue(i);
	assert(_residue <= _upper || _reverse->residue() <= _reverse->upper());
}

#endif
