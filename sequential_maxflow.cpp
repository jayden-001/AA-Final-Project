#include "graph.h"
#include "discharge.cpp"
#include <queue>

using namespace std;

void global_update(graph* g)
{
	vertex* s = g->s();
	vertex* t = g->t();

	vertex* vertices = g->v();
	for (int i = 2; i < g->n(); i++) {
		(vertices + i)->set_height(g->n());
	}
	
	queue<vertex*> bfsQ;
	bfsQ.push(t);
	while (bfsQ.size() > 0) {
		vertex* v = bfsQ.front();
		bfsQ.pop();
		vector<edge*>* edges = v->edges();
		for (int i = 0; i < edges->size(); i++) {
			edge* e = edges->at(i);
			vertex* op = e->v_op();
			if (e->reverse()->residue() != 0 && op->height() == g->n() && op != t) {
				op->set_height(v->height()+1);
				bfsQ.push(op);
			}
		}
	}
}

void sequential_maxflow(graph* g)
{
	int push_counter = 0;
	int relabel_counter = 0;
	int discharge_counter = 0;

	vertex* s = g->s();
	vertex* t = g->t();
 	priority_queue<vertex*, vector<vertex*>, CompareVertex> Q;
//	queue<vertex*> Q;
	
	// initialize preflow
	vector<edge*>* edges = s->edges();
	for (int i = 0; i < edges->size(); i++) {
		edge* e = edges->at(i);
		
		e->push_flow(e->upper());
		e->reverse()->v()->update_excess(e->upper());
		Q.push(e->v_op());
	}
	
	// initialize label
	s->set_height(g->n());
	global_update(g);
	
	// loop
	while (Q.size() != 0) {
		discharge(&Q, s, t, &push_counter, &relabel_counter);
		discharge_counter++;
		
		if ((discharge_counter + 1) % g->n()  == 0) {
			global_update(g);
		}
	}
	
	cout << "Push counts: " << push_counter << endl;
	cout << "Relabel counts: " << relabel_counter << endl;
}
