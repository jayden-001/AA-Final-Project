#include "graph.h"
#include "discharge.cpp"
#include <queue>

using namespace std;

void global_update(graph* g, priority_queue<vertex*, vector<vertex*>, CompareVertex>* Q)
{
	vertex* s = g->s();
	vertex* t = g->t();
	int n = g->n();

	vertex* vertices = g->v();
	for (int i = 2; i < n; i++) {
		vertices[i].set_height(n);
	}
	
	vertex** bfsQ = new vertex*[n];
	vertex** frontPt = bfsQ;
	vertex** backPt = bfsQ + 1;
	*frontPt = t;

	while (frontPt != backPt) {
		vertex* v = *frontPt;
		int new_height = v->height() + 1;
		frontPt++;
		vector<edge*>* edges = v->edges();
		for (int i = 0; i < edges->size(); i++) {
			edge* e = edges->at(i);
			vertex* op = e->v_op();
			if (e->reverse()->residue() != 0 && op->height() == n) {
				op->set_height(new_height);
				*backPt = op;
				backPt++;
			}
		}
	}
	delete [] bfsQ;
	
	// reupdate queue
	vector<vertex*> vs;
	while (Q->size() != 0) {
		vs.push_back(Q->top());
		Q->pop();
	}
	for (int i = 0; i < vs.size(); i++) {
		Q->push(vs[i]);
	}
	
}

void sequential_maxflow(graph* g)
{
	int push_counter = 0;
	int relabel_counter = 0;
	int discharge_counter = 0;

	vertex* s = g->s();
	vertex* t = g->t();
	int n = g->n();
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
	s->set_height(n);
	global_update(g, &Q);
	
	// loop
	while (Q.size() != 0) {
		discharge(&Q, s, t, &push_counter, &relabel_counter);
		discharge_counter++;
		
		if ((discharge_counter + 1) % n  == 0) {
			global_update(g, &Q);
		}
	}
	
	cout << "Push counts: " << push_counter << endl;
	cout << "Relabel counts: " << relabel_counter << endl;
	cout << "Discharge counts: " << discharge_counter << endl;
}
