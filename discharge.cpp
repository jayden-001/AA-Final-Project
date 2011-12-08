#include <cstdlib>
#include <cassert>
#include <iostream>
#include <vector>
#include <queue>
#include <cilk/cilk.h>
#include "graph.h"
#include "limits.h"

using namespace std;

vector<int> excess_tmp;

void pulse(graph &g)
{
	vector<vertex*>* vertices = g.v();
	vector<edge*>* edges;
	int d;
	cilk_for (int i = 2; i < g.v()->size(); ++i) {
		vertex *v = vertices->at(i);
		edges = v->edges();
		cilk_for (int j = 0; j < edges->size(); ++j) {
			if (v->excess() > 0) {
				edge *e = edges->at(j);
				if (e->residue(v) > 0 &&
					v->height() == e->opposite(v)->height()+1)
				{
					d = min(v->excess(), e->residue(v));
					v->update_excess(-d);
					e->update_flow(v, d);
				}
			}
		}
	}			
}

void push(vertex* v, edge *e)
{
	assert(e->v1() == v || e->v2() == v);
	assert(v->excess() > 0);
	assert(e->residue(v) > 0);
	assert(v->height() == e->opposite(v)->height() + 1);
	int d = min(v->excess(), e->residue(v));
	e->update_flow(v, d);
	vertex* w = e->opposite(v);
	v->update_excess(-1*d);
	w->update_excess(d);	

}

void relabel(vertex* v)
{
	vector<edge*>* edges = v->edges();
	int numOfEdges = edges->size();
	int minHeight = INT_MAX;

	for( int i = 0; i < numOfEdges; i++){
		edge* currentEdge = edges->at(i);
		vertex* w = currentEdge->opposite(v);
		if (currentEdge->residue(v) > 0 && w->height()+1 < minHeight) {
			minHeight = w->height()+1;
			v->set_height(minHeight);
		}
	}
	
	assert(minHeight != INT_MAX);
	
}

void push_relabel(vertex* v)
{
	assert(v->excess() > 0);	// must be active
	edge* e = v->cur_edge();

	if (e->residue(v) > 0 && v->height() == e->opposite(v)->height() + 1) {
		push(v, e);
	} else {
		if (!v->is_last()) {
			v->next_edge();
		} else {
			v->next_edge();
			relabel(v);
		}
	}

}

void discharge(queue<vertex*>* Q)
{
	vertex* v = Q->front();
	Q->pop();
	int h = v->height();
	int i = 0;
 	while (v->excess() != 0 && v->height() == h){
 		cout << i++ << endl;
		push_relabel(v);
		vertex* w = v->cur_edge()->opposite(v);
		if ( w->excess() > 0 )
			Q->push(w);
 	}
	if ( v->excess() > 0 )
		Q->push(v);
}
