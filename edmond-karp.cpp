#include "graph.h"
#include <queue>

#define print(x) std::cout << x << std::endl

using namespace std;
vector<int> minresidue;
vector<int> pred;

bool find_path(graph* g)
{
	vertex* s = g->s();
	vertex *t = g->t();
	int n = g->n();
	minresidue.resize(n, -1);
	pred.resize(n, -2);

	vertex* vertices = g->v();
	for (int i = 0; i < g->n(); i++) {
		minresidue[i] = -1;
		pred[i] = -2;
	}
	
	pred[0] = -1;
	vertex** bfsQ = new vertex*[n];
	vertex** frontPt = bfsQ;
	vertex** backPt = bfsQ + 1;
	*frontPt = s;

	while (frontPt != backPt) {
		vertex* v = *frontPt;
		frontPt++;
		vector<edge*>* edges = v->edges();
		for (int i = 0; i < edges->size(); i++) {
			edge* e = edges->at(i);
			vertex* op = e->v_op();
			if (e->residue() > 0 && pred[op->index()] == -2) {
				pred[op->index()] = v->index();
				//				print("pred of " << op->index() << "is " << v->index());
				if (minresidue[v->index()] <= -1)
					minresidue[op->index()] = e->residue();
				else
					minresidue[op->index()] = min(e->residue(), minresidue[v->index()]);
//				print(v->index() << " to " << op->index() << " residue: " << minresidue[op->index()]);
//				print("min: " << minresidue[v->index()] << ", res: " << e->residue());
//				print("record min: " << minresidue[op->index()]);
				*backPt = op;
				if (op->index() == 1) {
					delete[] bfsQ;
//					print("shortest path of capacity " << minresidue[1]);
					return true;
				}
				backPt++;
			}
		}
	}
	delete[] bfsQ;
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
		list<vertex*> path;
		cur = 1;
		while (cur != -1) {
			path.push_front(&v[cur]);
			cur = pred[cur];
		}
		list<vertex*>::iterator it;
		for (it = path.begin(); it != path.end();) {
			vertex u = **it;
			++it;
			vector<edge*>* edges = u.edges();
			for (int i = 0; i < edges->size(); i++) {
				edge* e = edges->at(i);
				vertex* op = e->v_op();
				if (op->index() == (*it)->index() && e->residue() >=  minresidue[1]) {
					e->push_flow(minresidue[1]);
				}
			} //for all edges
		} //iterate path list
		flow += minresidue[1];
//		print("current flow: " << flow);
	} //while has augmenting path
	print("edmond karp: flow " << flow);
	print("edmond karp: augment count " << augment);
	return flow;
}
