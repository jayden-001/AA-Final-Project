#ifndef GRAPH_H
#define GRAPH_H

#include <cstdlib>
#include <vector>
#include "vertex.h"
#include "edge.h"

using namespace std;

class graph
{
	private:
		int _n, _m;
		vector<vertex> _vertices;
		vector<edge> _edges;

	public:
		graph();
		
		int add_vertex(int);
		int add_edge(int, int, int, int, int);
		
		int n();
		int m();
		vertex& s();
		vertex& t();
		vector<vertex>* v();
		vector<edge>* e();
};

graph::graph() : _n(0), _m(0)
{
	add_vertex(0);	// source
	add_vertex(1);	// sink
}

int graph::add_vertex(int i)
{
	assert(vertices.size() == i);
	_vertices.push_back(vertex(i));
	_n++;
	return _n - 1;
}

int graph::add_edge(int v1, int v2, int upper12, int upper21, int cost = 0)
{
	assert(v1 < n());
	assert(v2 < n());
	assert(upper12 >= 0);
	assert(upper21 >= 0);
	_edges.push_back(edge(&(_vertices[v1]), &(_vertices[v2]), upper12, upper21, cost));
	_m++;
	return _m - 1;
}

int graph::n() { return _n; }

int graph::m() { return _m; }

vertex& graph::s() { return _vertices[0]; }

vertex& graph::t() { return _vertices[1]; }

vector<vertex>* graph::v() { return &_vertices; }

vector<edge>* graph::e() { return &_edges; }

#endif
