#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include "vertex.h"
#include "edge.h"

using namespace std;

class graph
{
	private:
		int _n, _m;
		vector<vertex*> _vertices;
		vector<edge*> _edges;

	public:
		graph();
		
		int add_vertex();
		int add_edge(int, int, int, int, int);
		void display_upper();
		void display_flow();
		bool is_valid_flow();
		int flow();
		int n();
		int m();
		vertex* s();
		vertex* t();
		vector<vertex*>* v();
		vector<edge*>* e();
};

graph::graph() : _n(0), _m(0)
{
	add_vertex();	// source
	add_vertex();	// sink
}

int graph::add_vertex()
{
	_vertices.push_back(new vertex(_n));
	_n++;
	return _n - 1;
}

int graph::add_edge(int v1, int v2, int upper12, int upper21, int cost = 0)
{
	assert(v1 < n() && v1 >= 0);
	assert(v2 < n() && v2 >= 0);
	assert(upper12 >= 0);
	assert(upper21 >= 0);
	_edges.push_back(new edge(_vertices[v1], _vertices[v2], upper12, upper21, cost));
	_m++;
	return _m - 1;
}

void graph::display_upper()
{
	for (int i = 0; i < _m; i++) {
		edge* e = _edges[i];
		int v1 = e->v1()->index();
		int v2 = e->v2()->index();

		if (e->upper(e->v1()) != 0)
			cout << v1 << '\t' << v2 << '\t' << e->upper(e->v1()) <<  endl;
		
		if (e->upper(e->v2()) != 0)
			cout << "*" << v2 << '\t' << v1 << '\t' << e->upper(e->v2()) << endl;
	}

}

void graph::display_flow()
{
	for (int i = 0; i < _m; i++) {
		edge* e = _edges[i];
		int v1 = e->v1()->index();
		int v2 = e->v2()->index();
		
		if (e->flow() > 0)
			cout << v1 << '\t' << v2 << '\t' << e->upper(e->v1()) <<","<< e->flow() <<  endl;
		
		if (e->flow() < 0)
			cout << v2 << '\t' << v1 << '\t' << e->upper(e->v2()) <<","<< -(e->flow()) << endl;
		
// 		if (e->upper(e->v1()) != 0)
// 			cout << v1 << '\t' << v2 << '\t' << e->upper(e->v1()) <<","<< e->flow() <<  endl;
// 		
// 		if (e->upper(e->v2()) != 0)
// 			cout << v2 << '\t' << v1 << '\t' << e->upper(e->v2()) <<","<< e->flow() << endl;
	}

}

bool graph::is_valid_flow()
{
	for (int i = 2; i < _n; i++){
		if ( _vertices[i]->excess() != 0)
			return false;	
	}
	return true;
}

int graph::flow()
{
	int flow = 0;
	int n_outgoing_edges = _vertices[0]->edges()->size();
	for (int i = 0; i < n_outgoing_edges; i++){
		flow += _vertices[0]->edges()->at(i)->flow();
	}
	return flow;
}

int graph::n() { return _n; }

int graph::m() { return _m; }

vertex* graph::s() { return _vertices[0]; }

vertex* graph::t() { return _vertices[1]; }

vector<vertex*>* graph::v() { return &_vertices; }

vector<edge*>* graph::e() { return &_edges; }

#endif
