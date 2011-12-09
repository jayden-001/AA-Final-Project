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
		int _cur_edge;
		int _index;
		vector<edge*> _edges;

	public:
		vertex(int);
		
		int index();

		int height();
		void set_height(int); 
		
		int excess();
		void update_excess(int);
				
		vector<edge*>* edges();
		edge* next_edge();
		edge* cur_edge();
		bool is_last();
};

vertex::vertex(int i) : _index(i), _height(0), _excess(0), _cur_edge(0)
{}

int vertex::height() { return _height; }

int vertex::index()
{
	return _index;
}

void vertex::set_height(int i)
{
	assert(i >= 0);
	_height = i;
}

edge* vertex::next_edge()
{
	int s = _edges.size();
	_cur_edge++;
	if (_cur_edge < s){
		return _edges[_cur_edge];
	}
	else {
		_cur_edge = 0;
		return _edges[_cur_edge];
	}
}

int vertex::excess() { return _excess; }

void vertex::update_excess(int i)
{
	_excess += i;
	assert(_excess >= 0);
}

edge* vertex::cur_edge()
{
	return _edges[_cur_edge];
}

bool vertex::is_last()
{
	return _cur_edge == _edges.size() - 1;
}

vector<edge*>* vertex::edges() { return &_edges; }

class CompareVertex {
        public:
                bool operator()( vertex* v1, vertex* v2 )
                {
                        return v1->height() >= v2->height();
                }
};


#endif
