#include "graph.h"
#include <queue>

#define print(x) std::cout << x << std::endl

using namespace std;
vector<int> minresidue;
vector<int> pred;
vector<vertex*> q;
vector<edge*> path;

bool find_path(graph* g)
{
	vertex* s = g->s();
	vertex *t = g->t();
	int n = g->n();
	minresidue.resize(n, -1);
	pred.resize(n, -2);
	path.resize(n);
	q.resize(n);

	vertex* vertices = g->v();
	for (int i = 0; i < g->n(); i++) {
		pred[i] = -2;
	}
	
	pred[0] = -1;
	minresidue[0] = INT_MAX;
	int head = 0;
	int tail = 1;
	q[head] = s;

	while (head != tail) {
		vertex* v = q[head];
		head++;
		vector<edge*>* edges = v->edges();
		for (int i = 0; i < edges->size(); i++) {
			edge* e = edges->at(i);
			vertex* op = e->v_op();
			if (e->residue() > 0 && pred[op->index()] == -2) {
				pred[op->index()] = v->index();
				path[op->index()] = e;
				minresidue[op->index()] = min(e->residue(), minresidue[v->index()]);
//				print(v->index() << " to " << op->index() << " residue: " << minresidue[op->index()]);
//				print("min: " << minresidue[v->index()] << ", res: " << e->residue());
				if (op->index() == 1) {
					return true;
				}
				q[tail] = op;
				tail++;
			}
		}
	}
	print("no path to t");
	return false;
}

int edmond_karp(graph *g)
{
	vertex *v = g->v();
	int flow = 0;
	int augment = 0;
	int cur;
	while (find_path(g)) {
		augment++;
		cur = 1;
		while (cur != 0) {
			path[cur]->push_flow(minresidue[1]);
			cur = pred[cur];
		}
		flow += minresidue[1];
	} //while has augmenting path
	print("edmond karp: flow " << flow);
	print("edmond karp: augment count " << augment);
	return flow;
}
