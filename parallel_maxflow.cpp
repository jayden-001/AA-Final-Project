#ifndef PARALLEL_SOLVER
#define PARALLEL_SOLVER

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include "discharge.cpp"

class parallel_solver
{
private:
	graph *g;
	vector<cilk::reducer_opadd<int> > excess_tmp;
	cilk::reducer_opadd<int> active;

public:
	parallel_solver(graph *G) : g(G) {}
	int solve_maxflow();
	void pulse();
	void stage1(vertex*);
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
	while (active.get_value() > 0) {
		pulse();
	}

	return 0;
}

void parallel_solver::pulse()
{
	vector<vertex*>* vertices = g->v();
	vector<edge*>* edges;
	vertex *v;
	cilk_for (int i = 2; i < g->v()->size(); ++i) {
		v = vertices->at(i);
		stage1(v);
	}			
	cilk_for(int i = 2; i < g->v()->size(); ++i) {
		v = vertices->at(i);
		v->update_excess(excess_tmp[i].get_value());
	}
}

void parallel_solver::stage1(vertex *v)
{
	edge *e;
	int d, res;
	vector<edge*>* edges = v->edges();
	int h = g->n()*2;
	cilk_for (int j = 0; j < edges->size(); ++j) {
		if (v->excess() > 0) {
			e = edges->at(j);
			res = e->residue(v);
			if (res > 0 &&
				v->height() == e->opposite(v)->height()+1)
			{
				d = min(v->excess(), res);
				v->update_excess(-d);
				e->update_flow(v, d); //update excess table for neighbors
				excess_tmp[e->opposite(v)->index()] += d;
				if (e->residue(v) > 0 && e->opposite(v)->height() < h)
					h = e->opposite(v)->height();
				//update excess table for neighbors
			}
		}
	}
	if (v->excess() > 0) {
		active++;
		v->set_height(h + 1);
		//stage 3 broadcast
	}
}

#endif
