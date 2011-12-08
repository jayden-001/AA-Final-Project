#ifndef VERTEX_H
#define VERTEX_H

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

class edge;

class vertex
{
	private:
		int _height;
		int _excess;
		int _currEdge;
		vector<edge*> _edges;

	public:
		vertex();

		int height();
		void set_height(int); 
		
		int excess();
		void update_excess(int);
				
		vector<edge*>* edges();
		edge* next_edge();
		edge* curr_edge();
};

vertex::vertex() : _height(0), _excess(0), _currEdge(0)
{}

int vertex::height() { return _height; }

void vertex::set_height(int i)
{
	assert(i >= 0);
	_height = i;
}

edge* vertex::next_edge()
{
	int s = _edges.size();
	if (_currEdge < s){
		return _edges[_currEdge++];
	}
	else {
		_currEdge = 0;
		return 0;
	}
}

int vertex::excess() { return _excess; }

void vertex::update_excess(int i)
{
	_excess += i;
	assert(_excess >= 0);
}

void vertex::curr_edge()
{
	return _edges[_currEdge];
}

vector<edge*>* vertex::edges() { return &_edges; }

#endif
