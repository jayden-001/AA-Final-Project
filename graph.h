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
		vertex* _vertices;
		edge* _edges;

	public:
		graph(int n, int m);
		
		void add_edge(int v1, int v2, int upper12);
		void display_upper();
		void display_flow();
// 		bool is_valid_flow();
		int flow();
		int n();
		int m();
		vertex* s();
		vertex* t();
};

graph::graph(int n, int m) : _n(n), _m(0)
{
	_vertices = new vertex[n];
	_edges = new edge[m*2];
	
	for (int i = 0; i < n; i++) {
		_vertices[i].set_index(i);
	}
// 	add_vertex();	// source
// 	add_vertex();	// sink
}

void graph::add_edge(int v1, int v2, int upper12)
{
	_edges[_m].set_edge(&(_vertices[v1]), upper12, &(_edges[_m+1]));
	_edges[_m+1].set_edge(&(_vertices[v2]), 0, &(_edges[_m]));
	_m += 2;
	
// 	_edges[_m].set_edge(&(_vertices[v1]), 0, &(_edges[_m+1]));
// 	_edges[_m+1].set_edge(&(_vertices[v2]), upper21, &(_edges[_m]));
// 	_m += 2;
}

void graph::display_upper()
{
	for (int i = 0; i < _m; i++) {
		edge* e = &(_edges[i]);
		vertex* v = e->v();
		int v_index = e->v()->index();

		if (e->upper() != 0)
			cout << v_index << '\t' << e->reverse()->v()->index() << '\t' << e->upper() <<  endl;
	}

}

void graph::display_flow()
{
	for (int i = 0; i < _m; i++) {
		edge* e = &(_edges[i]);
		vertex* v = e->v();
		int v_index = e->v()->index();
		
		if (e->flow() > 0)
			cout << v_index << '\t' << e->reverse()->v()->index() << '\t' << e->upper() <<","<< e->flow() <<  endl;
	}

}

// bool graph::is_valid_flow()
// {
// 	bool flag = true;
// 	for (int i = 2; i < _n; i++){
// 		int excess = 0;
//         	int n_outgoing_edges = _vertices[i].edges()->size();
//         	for (int j = 0; j < n_outgoing_edges; j++){
// 			edge* e = _vertices[i].edges()->at(j);
// 			int flow = e->flow(&(_vertices[i]));
//                 	excess += flow;
//         	}
// 		if (excess != 0){
// 			cout << "flow is not conserved at vertex: " << i << " excess: " << excess << endl;
// 			flag = false;
// 		}
// 	
// 	}
// 	for (int i = 0; i < _m; i++){
// 		edge* e = &(_edges[i]);
// 		vertex* v1 = e->v1();
// 		vertex* v2 = e->v2();
// 		if (  e->flow(v1) > e->upper(v1) ){
//                 	cout << "flow exceeds upper bound from vertex: " << v1->index() << " to vertex: " << v2->index()
//                         <<  " flow: " << e->flow(v1) << " upper: " <<  e->upper(v1) << endl;
// 			flag = false;
//                 }
// 		else if (e->flow(v2) > e->upper(v2)){
// 			cout << "flow exceeds upper bound from vertex: " << v2->index() << " to vertex: " << v1->index() 
// 			<<  " flow: " << e->flow(v2) << " upper: " <<  e->upper(v2) << endl;
//                         flag = false;
// 		}
// 	}
// 
// 	return flag;
// }

int graph::flow()
{
// 	int flow = 0;
// 	int n_outgoing_edges = _vertices[0].edges()->size();
// 	for (int i = 0; i < n_outgoing_edges; i++){
// 		flow += _vertices[0].edges()->at(i)->flow(&(_vertices[0]));
// 	}
// 	return flow;
	return t()->excess();
}

int graph::n() { return _n; }

int graph::m() { return _m; }

vertex* graph::s() { return &(_vertices[0]); }

vertex* graph::t() { return &(_vertices[1]); }

// vector<vertex*>* graph::v() { return &_vertices; }
// 
// vector<edge*>* graph::e() { return &_edges; }

#endif
