#ifndef REDUCER_LIST
#define REDUCER_LIST

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_list.h>
#include <pthread.h>

#define print(x) std::cout << x << std::endl

class reducer_list
{
private:
	graph *g;
	vector<vertex*>* vertices;
	vector<pthread_mutex_t> vlocks;
	vector<int> ex;
	list<int> active;

public:
	reducer_list(graph *G) : g(G), vertices(G->v()) {
		vlocks.resize(g->n());
		ex.resize(g->n());
		g->s()->set_height(g->n());
		for (int i = 0; i < g->n(); ++i) {
			pthread_mutex_init(&vlocks[i], NULL);
		}
	}

	~reducer_list() {
		for (int i = 0; i < g->n(); ++i)
			pthread_mutex_destroy(&vlocks[i]);
	}
	int solve_maxflow();
	bool pulse(int);
	bool push(int);
	void relabel(int);
};

int reducer_list::solve_maxflow()
{
	vertex *s = g->s();
	vector<edge*>* s_adj = s->edges();
	int a = 0;
	cilk::reducer_list_append<int> active_v;
	for (int i = 0; i < s_adj->size(); ++i) {
		edge *e = s_adj->at(i);
		e->update_flow(s, e->upper(s));
		e->opposite(s)->update_excess(e->upper(s));
		active.push_back(e->opposite(s)->index());
	}

//	g->display_flow();
//	active = active_v.get_value();
//	print("initial size: " << active.size());
	int counter = 0;
	while	(pulse(counter)) {
//		g->display_flow();
//		print("count: " << counter << ", active: " << active.size());
//		counter++;
	}
	return 0;
}

//return whether there still active nodes
bool reducer_list::pulse(int c)
{
	cilk::reducer_list_append<int> rl;
	list<int>::iterator it = active.begin();
	for (; it != active.end(); ++it) {
		if (push(*it)) {
			rl.push_back(*it);
		}
	}			
	cilk_sync;
	list<int> excess_v = rl.get_value();
	for (it = excess_v.begin(); it != excess_v.end(); ++it) {
		relabel(*it);
	}

	cilk::reducer_list_append<int> active_r;
	cilk_for (int i = 2; i < vertices->size(); ++i) {
		vertex *v = vertices->at(i);
		v->update_excess(ex[i]);
		ex[i] = 0;
		if (v->excess() > 0) {
			active_r.push_back(i);
		}
	}
	cilk_sync;
	active = active_r.get_value();
	return !active.empty();
}

//stage 1
bool reducer_list::push(int i)
{
	vertex *v = vertices->at(i);
	int excess = v->excess();
	if (excess <= 0)
		return false;
	vector<edge*>* edges = v->edges();
	int h = v->height();
	cilk_for (int j = 0; j < edges->size(); ++j) {
		edge *e = edges->at(j);
		int res = e->residue(v);
		if (h == e->opposite(v)->height()+1 &&
			res > 0)
		{
			bool updated = false;
			int d;
			pthread_mutex_lock(&vlocks[i]);
			if (excess > 0) {
				d = min(excess, res);
				assert(d > 0);
				excess -= d;
				updated = true;
			}
			pthread_mutex_unlock(&vlocks[i]);
			if (updated) {
				e->update_flow(v, d);
				int k = e->opposite(v)->index();
				pthread_mutex_lock(&vlocks[k]);
				ex[k] += d;
				pthread_mutex_unlock(&vlocks[k]);
			}
		}
	}
	cilk_sync;
	v->update_excess(excess - v->excess());
	if (excess > 0)
		return true;
	return false;
}

void reducer_list::relabel(int i)
{
	assert(i > 1);
	vertex* v = vertices->at(i);
	assert(v->excess() > 0);
	cilk::reducer_min<int> h(g->n()*2);
	vector<edge*>* edges = v->edges();
	cilk_for (int i = 0; i < edges->size(); ++i) {
		edge *e = edges->at(i);
		if (e->residue(v) > 0) {
			int k = e->opposite(v)->height();
			assert(v->height() <= k);
			cilk::min_of(h, e->opposite(v)->height());
		}
	}
	v->set_height(h.get_value() + 1);

//	int height = v->height();
//	int newh = h.get_value();
//	if (height < newh) {
//		v->set_height(newh);
//	}
}

#endif
