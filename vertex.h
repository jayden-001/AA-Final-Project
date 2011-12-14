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

		vertex* _prev;
		vertex* _next;		

		int _height;
		int _excess;
		int _cur_edge;
		int _last_edge;
		int _index;
		vector<edge*> _edges;

	public:
		vertex();
		vertex(int);
		
		void set_index(int i) { _index = i; }
		int index() { return _index; }

		int height() { return _height; }
		void set_height(int i) { _height = i; }
		
		int excess() { return _excess; }
		void update_excess(int);

		vertex* prev() { return _prev; }
		void set_prev(vertex* p) { _prev = p; }

		vertex* next() { return _next; }
		void set_next(vertex* n) { _next = n; }		

		void add_edge(edge*);
		vector<edge*>* edges() { return &_edges; }
		edge* next_edge();
		edge* cur_edge() { return _edges[_cur_edge]; }
		bool is_last() { return _cur_edge == _last_edge; }
};

vertex::vertex() : _height(0), _excess(0), _cur_edge(0), _last_edge(0), _prev(NULL), _next(NULL)
{}

vertex::vertex(int i) : _index(i), _height(0), _excess(0), _cur_edge(0), _last_edge(0),  _prev(NULL), _next(NULL)
{}

void vertex::add_edge(edge* e)
{
	_edges.push_back(e);
	_last_edge = _edges.size() - 1;
}

edge* vertex::next_edge()
{
	_cur_edge++;
	if (_cur_edge > _last_edge) {
		_cur_edge = 0;
		return _edges[_cur_edge];
	}
	return _edges[_cur_edge];
}

void vertex::update_excess(int i)
{
	_excess += i;
	assert(_excess >= 0);
}

class CompareVertex {
	public:
		bool operator()( vertex* v1, vertex* v2 )
		{
			return v1->height() < v2->height();
		}
};


#endif
