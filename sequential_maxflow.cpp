#include "graph.h"
#include "discharge.cpp"
#include <queue>

using namespace std;
vector<bool> queued;

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
// 	priority_queue<vertex*, vector<vertex*>, CompareVertex> Q;
	queue<vertex*> Q;
	queued.resize(g->n(), false);
	queued[0] = true;
	queued[1] = true;

	// initialize preflow
	vector<edge*>* edges = s->edges();
	for (int i = 0; i < edges->size(); i++) {
		edge* e = edges->at(i);
		if (e->residue() <= 0)
			continue;
		e->push_flow(e->residue());
		e->reverse()->v()->update_excess(e->upper());
		Q.push(e->v_op());
		queued[e->v_op()->index()] = true;
	}
	
	// initialize label
	s->set_height(n);
	global_update(g, &Q);
	
	// loop
	while (!Q.empty()) {
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


void sequential_maxflow_two_phases(graph* g)
{
	int push_counter = 0;
	int relabel_counter = 0;
	int discharge_counter1 = 0;
	int discharge_counter2 = 0;

	vertex* s = g->s();
	vertex* t = g->t();
	int n = g->n();
// 	priority_queue<vertex*, vector<vertex*>, CompareVertex> Q;
	queue<vertex*> Q_first_phase;
	queue<vertex*> Q_second_phase;

	// initialize preflow
	vector<edge*>* edges = s->edges();
	for (int i = 0; i < edges->size(); i++) {
		edge* e = edges->at(i);
		if (e->residue() <= 0)
			continue;
		e->push_flow(e->residue());
		e->reverse()->v()->update_excess(e->upper());
		Q_first_phase.push(e->v_op());
	}
	
	// initialize label
	s->set_height(n);
	global_update(g, &Q);

	vertex* v, w;	
	// loop first phase
	while (!Q_first_phase.empty()) {
		v = Q_first_phase.front();	
		Q_first_phase.pop();

		while (v->excess() != 0 ){
			push_relabel(v, &push_counter, &relabel_counter);
			w = v->cur_edge()->v_op();
			if ( w->excess() > 0 && w != s && w != t){
				if ( w->height() < n ){
					Q_first_phase.push(w);
				}
				else {
					Q_second_phase.push(w);
				}
			}
		}

		discharge_counter1++;
		
		if ((discharge_counter1 + 1) % n  == 0) {
			global_update(g, &Q_first_phase, true);
		}
	}

	global_update(g, &Q_second_phase, false);

	while ( !Q_second_phase.empty() ){
		v = Q_second_phase.front();
                Q_second_phase.pop();

                while (v->excess() != 0 ){
                        push_relabel(v, &push_counter, &relabel_counter);
                        w = v->cur_edge()->v_op();
                        if ( w->excess() > 0 && w != s && w != t){
                        	Q_second_phase.push(w);
                        }
                }

                discharge_counter2++;

                if ((discharge_counter2 + 1) % n  == 0) {
                        global_update(g, &Q_second_phase, false);
                }

	}
	
	cout << "Push counts: " << push_counter << endl;
	cout << "Relabel counts: " << relabel_counter << endl;
	cout << "Discharge counts first: " << discharge_counter1 << " second: " << discharge_counter2 <<endl;
}
