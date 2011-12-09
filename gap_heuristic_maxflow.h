#ifndef GAP_HEURISTIC
#define GAP_HEURISTIC

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <cilk/reducer_min.h>
#include <pthread.h>

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
		int solve_maxflow()

	private:
		graph *g;
		vector<vertex*> *vertices;
		vector<vnode*> bucket(g->n()*2);
		int highest;

}

int gap_heuristic::solve_maxflow()
{
	int n = g->n();
	for (int i = 2; i < vertices; ++i) {
		vnode v(vertices->at[n + 1 - i]);
		v.next = bucket[0];
		bucket[0] = &v;
	}
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

#endif
