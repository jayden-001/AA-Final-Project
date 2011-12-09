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
	vector<vertex*>* vertices;
	cilk::reducer_opadd<int> *excess_tmp;
	vector<pthread_mutex_t> vlocks;
	cilk::reducer_opadd<int> active;

public:
	parallel_solver(graph *G) : g(G), vertices(G->v()) 
	{
		vlocks.resize(g->n());
		excess_tmp = new cilk::reducer_opadd<int>[g->n()];
		g->s()->set_height(g->n());
	}

	~parallel_solver() { delete[] excess_tmp; }
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
		if (e->opposite(s)->excess() > 0)
			++active;
	}
	g->display_flow();
	assert(active.get_value() == s_adj->size());
	int counter = 0;
	while (active.get_value() > 0) {
		print("count: " << counter << " active: " << active.get_value());
		pulse();
		counter++;
	}
	return 0;
}

void parallel_solver::pulse()
{
	active -= active.get_value();
//	vector<vertex*>* vertices = g->v();
	vector<edge*>* edges;
	vertex *v;
	cilk_for (int i = 2; i < vertices->size(); ++i) {
		v = vertices->at(i);
		push(v);
	}			
	cilk_for(int i = 2; i < vertices->size(); ++i) {
		v = vertices->at(i);
		relabel(v);
		int exc = excess_tmp[i].get_value();
		excess_tmp[i] -= exc;
		v->update_excess(exc);
		if (v->excess() > 0)
			active += 1;
	}
}

//stage 1
void parallel_solver::push(vertex *v)
{
	int d;
	int excess = v->excess();
	vector<edge*>* edges = v->edges();
	int h = g->n()*2;
	bool updated;
	cilk_for (int j = 0; j < edges->size(); ++j) {
		edge *e = edges->at(j);
		int res = e->residue(v);
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
	v->update_excess(excess - v->excess());
}

void parallel_solver::relabel(vertex* v)
{
	cilk::reducer_min<int> h(g->n()*2);
	vector<edge*>* edges = v->edges();
	cilk_for (int i = 0; i < edges->size(); ++i) {
		edge *e = edges->at(i);
		if (e->residue(v) > 0) {
			cilk::min_of(h, e->opposite(v)->height());
		}
	}
	int height = v->height();
	int newh = h.get_value();
	if (height < newh + 1) {
		v->set_height(newh + 1);
	}
}

#endif
