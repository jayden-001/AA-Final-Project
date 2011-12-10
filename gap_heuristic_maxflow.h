#ifndef GAP_HEURISTIC
#define GAP_HEURISTIC

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <cilk/reducer_min.h>
#include <pthread.h>
#include <vector>
#include <queue>
#include "graph.h"
#include "limits.h"

#define print(x) std::cout << x << std::endl

class gap_heuristic
{
	public: 
		struct vnode {
			vertex *v;
			vertex *next;
			vnode(vertex *ver) : next(NULL), v(ver) {}
		};

		gap_heuristic(graph *G) : g(G), vertices(G->v()), highest(0) {}
		~gap_heuristic();
		int solve_maxflow();

	private:
		vertex* pop(int i);
	
		push(vertex* v, edge *e);
		relabel(vertex* v);
		push_relabel(vertex* v);
		discharge(queue<vertex*>* Q, vertex* source, vertex* sink);
	
		graph *g;
		vector<vertex*>* vertices;
		vector<vnode*> buckets(g->n()*2);
		int highest;

}

int gap_heuristic::solve_maxflow()
{	
	// initialize buckets
	int n = g->n();
	for (int i = 2; i < vertices; ++i) {
		insert(0, vertices->at[i]);
	}
	highest = 0;
	
	// initialize preflow
	vector<edge*>* edges = g->s()->edges();
	for (int i = 0; i < edges->size(); i++) {
		edge* e = edges->at(i);
		e->update_flow(s, e->upper(s));
		e->opposite(s)->update_excess(e->upper(s));
	}
	// initialize label
	s->set_height(g->n());
	
	// loop
	while (Q.size() != 0) {
		discharge(&Q, g->s(), g->t());
	}

}

vertex* gap_heuristic::pop(int i)
{
	vnode* node = buckets[i];
	buckets[i] = node->next;
	
	vertex* v = node->v;
	delete node;
	return v;
}

void gap_heuristic::insert(int i, vertex* v)
{
	vnode* node = new vnode(v);
	node->next = buckets[i];
	buckets[i] = node;
}

void gap_heuristic::push(vertex* v, edge *e)
{
	assert(e->v1() == v || e->v2() == v);
	assert(v->excess() > 0);
	assert(e->residue(v) > 0);
	assert(v->height() == e->opposite(v)->height() + 1);
	int d = min(v->excess(), e->residue(v));
	e->update_flow(v, d);
	vertex* w = e->opposite(v);
	v->update_excess(-d);
	w->update_excess(d);	

}

void gap_heuristic::relabel(vertex* v)
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

void gap_heuristic::push_relabel(vertex* v)
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

void gap_heuristic::discharge(queue<vertex*>* Q, vertex* source, vertex* sink)
{
	vertex* v = Q->front();
	Q->pop();
	int h = v->height();
	int i = 0;
 	while (v->excess() != 0 && v->height() == h){
		push_relabel(v);
		vertex* w = v->cur_edge()->opposite(v);
		if ( w->excess() > 0 && w != source && w!= sink )
			Q->push(w);
 	}
	if ( v->excess() > 0 )
		Q->push(v);
}

#endif
