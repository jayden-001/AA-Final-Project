#include "graph.h"
#include "discharge.cpp"
#include <queue>

using namespace std;

void global_update(graph* g, priority_queue<vertex*, vector<vertex*>, CompareVertex>* Q, bool isToSink)
{
	vertex* s = g->s();
	vertex* t = g->t();
	int n = g->n();

	vertex* vertices = g->v();
	for (int i = 2; i < n; i++) {
		vertices[i].set_height(n);
	}
	if (!isToSink)
		vertices[0].set_height(0);
	
	vertex** bfsQ = new vertex*[n];
	vertex** frontPt = bfsQ;
	vertex** backPt = bfsQ + 1;
	*frontPt = isToSink ? t : s;

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

void global_update(graph* g, bool isToSink)
{
	vertex* s = g->s();
	vertex* t = g->t();
	int n = g->n();

	vertex* vertices = g->v();
	for (int i = 2; i < n; i++) {
		vertices[i].set_height(n);
	}
	if (!isToSink)
		vertices[0].set_height(0);
	
	vertex** bfsQ = new vertex*[n];
	vertex** frontPt = bfsQ;
	vertex** backPt = bfsQ + 1;
	*frontPt = isToSink ? t : s;

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
		if (e->residue() <= 0)
			continue;
		e->push_flow(e->residue());
		e->reverse()->v()->update_excess(e->upper());
		if (e->v_op() != t)
			Q.push(e->v_op());
	}
	
	// initialize label
	s->set_height(n);
	global_update(g, &Q, true);
	
	// loop
	while (!Q.empty()) {
		discharge(&Q, s, t, &push_counter, &relabel_counter);
		discharge_counter++;
		
		if ((discharge_counter + 1) % n  == 0) {
			global_update(g, &Q, true);
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
 	priority_queue<vertex*, vector<vertex*>, CompareVertex> Q_first_phase;
	priority_queue<vertex*, vector<vertex*>, CompareVertex> Q_second_phase;
//	queue<vertex*> Q_first_phase;
//	queue<vertex*> Q_second_phase;

	// initialize preflow
	vector<edge*>* edges = s->edges();
	for (int i = 0; i < edges->size(); i++) {
		edge* e = edges->at(i);
		if (e->residue() <= 0)
			continue;
		e->push_flow(e->residue());
		e->reverse()->v()->update_excess(e->upper());
		if (e->v_op() != t)
			Q_first_phase.push(e->v_op());
	}
	
	// initialize label
	s->set_height(n);
	global_update(g, &Q_first_phase, true);

	vertex* v;
	vertex* w;	
	// loop first phase
	while (!Q_first_phase.empty()) {
		v = Q_first_phase.top();	
		Q_first_phase.pop();

		while (v->excess() != 0 ){
			w = v->cur_edge()->v_op();
			int old_excess = w->excess();
			push_relabel(v, &push_counter, &relabel_counter);
			if ( old_excess == 0 && w->excess() > 0 && w != s && w != t){
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
		v = Q_second_phase.top();
                Q_second_phase.pop();

                while (v->excess() != 0 ){
                		w = v->cur_edge()->v_op();
						int old_excess = w->excess();
                        push_relabel(v, &push_counter, &relabel_counter);
                        if ( old_excess == 0 && w->excess() > 0 && w != s && w != t){
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

void sequential_maxflow_two_phases_fifo(graph* g)
{
	int push_counter = 0;
	int relabel_counter = 0;
	int discharge_counter1 = 0;
	int discharge_counter2 = 0;

	vertex* s = g->s();
	vertex* t = g->t();
	int n = g->n();
// 	priority_queue<vertex*, vector<vertex*>, CompareVertex> Q_first_phase;
//	priority_queue<vertex*, vector<vertex*>, CompareVertex> Q_second_phase;
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
		if (e->v_op() != t)
			Q_first_phase.push(e->v_op());
	}
	
	// initialize label
	s->set_height(n);
	global_update(g, true);

	vertex* v;
	vertex* w;	
	// loop first phase
	while (!Q_first_phase.empty()) {
		v = Q_first_phase.front();	
		Q_first_phase.pop();

		while (v->excess() != 0 ){
			w = v->cur_edge()->v_op();
			int old_excess = w->excess();
			push_relabel(v, &push_counter, &relabel_counter);
			if ( old_excess == 0 && w->excess() > 0 && w != s && w != t){
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
			global_update(g, true);
		}
	}

	global_update(g, false);

	while ( !Q_second_phase.empty() ){
		v = Q_second_phase.front();
                Q_second_phase.pop();

                while (v->excess() != 0 ){
                        w = v->cur_edge()->v_op();
                        int old_excess = w->excess();
                        push_relabel(v, &push_counter, &relabel_counter);
                        if ( old_excess == 0 && w->excess() > 0 && w != s && w != t){
                        	Q_second_phase.push(w);
                        }
                }

                discharge_counter2++;

                if ((discharge_counter2 + 1) % n  == 0) {
                        global_update(g, false);
                }

	}
	
	cout << "Push counts: " << push_counter << endl;
	cout << "Relabel counts: " << relabel_counter << endl;
	cout << "Discharge counts first: " << discharge_counter1 << " second: " << discharge_counter2 <<endl;
}

/*
void sequential_maxflow_two_phases_bucket(graph* g)
{
	int push_counter = 0;
	int relabel_counter = 0;
	int discharge_counter1 = 0;
	int discharge_counter2 = 0;

	vertex* s = g->s();
	vertex* t = g->t();
	int n = g->n();

	vector<list<vertex*> > bucket;
	vector<bool> active;
	int highest;
	int num_of_active_vertices = 0;

	active.resize( n, false);
	active[0] = true;
        active[1] = true;
	bucket.resize( n+1 ); // height can have value from 0 to n

	// initialize preflow
	s->set_height(n);
	vector<edge*>* edges = s->edges();
	for (int i = 0; i < edges->size(); i++) {
		edge* e = edges->at(i);
		if (e->residue() <= 0)
			continue;
		e->push_flow(e->residue());
		vertex *op = e->v_out();
		op->update_excess(e->upper());
		active[op->index()] = true;
                bucket[1].push_front(op);
		num_of_active_vertices++;
	}
	highest = 1;
	
	// initialize label
	global_update(g, &bucket, true);

	vertex* v;
	vertex* w;	
	// loop first phase
	while (num_of_active_vertices > 0) {
		v = Q_first_phase.top();	
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
		v = Q_second_phase.top();
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

}*/
