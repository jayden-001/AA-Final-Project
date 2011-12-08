#include "graph.h"
#include "discharge.cpp"
#include <queue>

using namespace std;

void sequential_maxflow(graph* g)
{
	vertex* s = g->s();
	vertex* t = g->t();
	queue<vertex*> Q;
	
	// initialize preflow
	vector<edge*>* edges = s->edges();
	for (int i = 0; i < edges->size(); i++) {
		edge* e = edges->at(i);
		e->update_flow(s, e->upper(s));
		e->opposite(s)->update_excess(e->upper(s));
		Q.push(e->opposite(s));
	}
	
	// initialize label
	s->set_height(g->n());
	
	// loop
	while (Q.size() != 0) {
		discharge(&Q);
	}
}