#include "graph.h"
#include "discharge.cpp"
#include <queue>

using namespace std;

void sequential_maxflow(graph* g)
{
	vertex* s = g->s();
	vertex* t = g->t();
// 	priority_queue<vertex*, vector<vertex*>, CompareVertex> Q;
	queue<vertex*> Q;
	
	// initialize preflow
	vector<edge*>* edges = s->edges();
	for (int i = 0; i < edges->size(); i++) {
		edge* e = edges->at(i);
		
		e->push_flow(e->upper());
		e->reverse()->v()->update_excess(e->upper());
		Q.push(e->reverse()->v());
	}
	
	// initialize label
	s->set_height(g->n());
	
	// loop
	while (Q.size() != 0) {
		discharge(&Q, g->s(), g->t());
	}
}
