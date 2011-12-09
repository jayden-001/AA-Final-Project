#ifndef PARALLEL_SOLVER
#define PARALLEL_SOLVER

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <cilk/reducer_min.h>
#include <pthread.h>
//#include "discharge.cpp"

#define print(x) std::cout << x << std::endl

class parallel_solver
{
private:
	graph *g;
//	vector<cilk::reducer_opadd<int> > excess_tmp;
	cilk::reducer_opadd<int> *excess_tmp;
	vector<pthread_mutex_t> vlocks;
	vector<pthread_mutex_t> elocks;
	cilk::reducer_opadd<int> active;

public:
	parallel_solver(graph *G) : g(G) {
		vlocks.resize(g->n());
		elocks.resize(g->m());
//		excess_tmp.resize(g->n());
//		vector<cilk::reducer_opadd<int> >::iterator it;
//		cilk_for (int i = 2; i < g->n(); ++i) {
//			excess_tmp[i] += 0;
//		}
		excess_tmp = new cilk::reducer_opadd<int>[g->n()];
		print("finish initializing");
	}
	int solve_maxflow();
	void pulse();
	void push(vertex*);
	void relabel(vertex*);
};

int parallel_solver::solve_maxflow()
{
	vertex *s = g->s();
	vector<edge*>* s_adj = s->edges();
	edge *e;
	cilk_for (int i = 0; i < s_adj->size(); ++i) {
		e = s_adj->at(i);
		e->update_flow(s, e->upper(s));
		e->opposite(s)->update_excess(e->upper(s));
	}
	g->display_flow();
	active += s_adj->size();
	int counter = 0;
	print("start while loop");
	while (active.get_value() > 0) {
		print("counter: " << counter);
		pulse();
	}

	return 0;
}

void parallel_solver::pulse()
{
//	print("active count");
//	print(active.get_value());
	active -= active.get_value();
	vector<vertex*>* vertices = g->v();
	vector<edge*>* edges;
	vertex *v;
	cilk_for (int i = 2; i < vertices->size(); ++i) {
		v = vertices->at(i);
		push(v);
	}			
	print("finish first loop");
	cilk_for(int i = 2; i < vertices->size(); ++i) {
		v = vertices->at(i);
		relabel(v);
//		cilk::reducer_opadd<int> reducer = excess_tmp.at(i);
		int exc = excess_tmp[i].get_value();
		excess_tmp[i] -= exc;
		print("excess: " << exc);
		v->update_excess(exc);
		print("v excess: " << v->excess());
		if (v->excess() > 0)
			active += 1;
	}
	print("finish second loop");
	print("active: " << active.get_value());
}

//stage 1
void parallel_solver::push(vertex *v)
{
	edge *e;
	int d, res;
	int excess = v->excess();
//	print("excess");
//	print(excess);
	vector<edge*>* edges = v->edges();
	int h = g->n()*2;
	bool updated;
	cilk_for (int j = 0; j < edges->size(); ++j) {
		//lock edge first before v
		e = edges->at(j);
		res = e->residue(v);
		if (v->height() == e->opposite(v)->height()+1 &&
			e->residue(v) > 0)
		{
			updated = false;
			pthread_mutex_lock(&vlocks[v->index()]);
			if (excess > 0) {
				d = min(excess, res);
				excess -= d;
				updated = true;
			}
			pthread_mutex_unlock(&vlocks[v->index()]);
			if (updated) {
				e->update_flow(v, d);
				excess_tmp[e->opposite(v)->index()] += d;
			}
		}
	}
}

void parallel_solver::relabel(vertex* v)
{
	cilk::reducer_min<int> h(g->n()*2);
	vector<edge*>* edges = v->edges();
	cilk_for (int i = 0; i < edges->size(); ++i) {
		edge *e = edges->at(i);
		if (e->residue(v) > 0) {
//			print("comparing " << h.get_value << ", " << e->opposite(v)->height());
			cilk::min_of(h, e->opposite(v)->height());
//			print(h.get_value());
		}
	}
	int height = v->height();
	int newh = h.get_value();
//	print("newh: " << newh);
	if (height + 1 < newh) {
		v->set_height(newh + 1);
	}
}

#endif
