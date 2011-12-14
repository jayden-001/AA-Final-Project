#include "graph.h"
#include "discharge.cpp"
#include <vector>
#include <list>

using namespace std;

#define print(x) std::cout << x << std::endl
#define for_vertex(i) for (int i = 2; i < g->n(); ++i)

int push_count = 0;
int relabel_count = 0;
vector<list<vertex*> > bucket;
vector<bool> active;
int highest;
int count;
vertex *s;
vertex *t;

void initialize(graph *g)
{
	s = g->s();
	t = g->t();
	active.resize(g->n(), false);
	active[0] = true;
	active[1] = true; 
	highest = 1;
	bucket.resize(g->n() * 2);
	g->s()->set_height(g->n());
	vector<edge*>* edges = g->s()->edges();
	for (int i = 0; i < edges->size(); i++) {
		edge* e = edges->at(i);
		if (e->residue() == 0)
			continue;
		e->push_flow(e->residue());
		vertex *op = e->v_op();
		op->update_excess(e->upper());
		op->set_height(1);
		active[op->index()] = true;
		bucket[1].push_front(op);
		++count;
	}
}

void discharge1(vertex *v)
{
	int h = v->height();
 	while (v->excess() != 0) {
		push_relabel(v, &push_count, &relabel_count);
		vertex* w = v->cur_edge()->v_op();
		if ( w->excess() > 0 && w != s && w!= t) {
			if (!active[w->index()]) {
				bucket[w->height()].push_front(w);
				active[w->index()] = true;
				count++;
				highest = max(highest, w->height());
				if (w->height() > highest)
					print("height: " << w->height() << ", highest: " << highest);
			}
		}
 	}
}

void discharge(vertex *v)
{
	edge *cur;
	while (v->excess() > 0) {
		cur = v->cur_edge();
		vertex *op = cur->v_op();
		if (cur->residue() > 0 && v->height() == cur->v_op()->height() + 1) {
			if (!active[op->index()]) {
				count++;
				active[op->index()] = true;
				bucket[op->height()].push_front(op);
				highest = max(highest, op->height());
//	 			print("current highest: " << highest);
			}
			push(v, cur);
			push_count++;
//			print("vertex " << v->index() << ", excess: " << v->excess());
		}
		else if (v->is_last()) {
			relabel(v);
			relabel_count++;
//			print("vertex: " << v->index() << ", height: " << v->height());
		}
		v->next_edge();
	}
}

void gap_relabel(graph *g)
{
	int c = 0;
	int gap = 0;
//	g->display_flow();
	while (count > 0) {
//		print("highest: " << highest);
//		for (int i = 0; i < 2*g->n(); ++i) {
//			if (!bucket[i].empty()) {
//				highest = i;
//			}
//		}
		int oldh = highest;

//		print("current highest: " << highest);
		vertex *f = bucket[highest].front();
		bucket[highest].pop_front();
		count--;
		discharge(f);
		active[f->index()] = false;

		vertex *v = g->v();
		if (bucket[oldh].empty()) {
			while (bucket[highest--].empty());
			print("next highest " << highest);
			gap++;
//			for (int i = 2; i < g->n(); ++i) {
//				int h = v[i].height();
//				if (v[i].height() > oldh && active[v[i].index()]) {
//					v[i].set_height(max(h, g->n()));
//					highest = max(v[i].height(), highest);
//				}
//			}		
			list<vertex*>::iterator it = bucket[g->n()].begin();
			list<vertex*>::iterator j;
			for (int i = oldh + 1; i < g->n(); ++i) {
				if (bucket[i].empty())
					continue;
				highest = g->n();
				bucket[g->n()].splice(it, bucket[i]);
				for (j = bucket[i].begin(); j != bucket[i].end(); ++j) {
					(*j)->set_height(g->n());
				}
			}
			print("highest after gap relabel: " << highest);
		}
		if (bucket[highest].empty())
			print("bucket " << highest << " empty");
	}
	print("Push count: " << push_count);
	print("Relabel count: " << relabel_count);
	print("Gap count: " << gap);
}

void gap_relabel1(graph *g)
{
	int gap = 0;
	while (count > 0) {
		while (!bucket[highest].empty()) {
//			print("highest: " << highest);
			vertex *f = bucket[highest].front();
			bucket[highest].pop_front();
			discharge(f);
			active[f->index()] = false;
			count--;
		}
//		print("height " << highest << " is empty");
		vertex *v = g->v();
//		print("clearing height > " << highest);
		for (int i = 2; i < g->n(); ++i) {
			int h = v[i].height();
//			if (v[i].height() > highest) {
//				v[i].set_height(max(h, g->n()));
//			}
		}		

		--highest;
		while (bucket[highest].empty() && highest > 0)
			--highest;

		if (highest < 0)
			print("highest < 0, count: " << count);
	}
//	g->display_flow();
	print("Push count: " << push_count);
	print("Relabel count: " << relabel_count);
	print("Gap count: " << gap);
}
