#include "graph.h"
#include "discharge.cpp"

using namespace std;

#define print(x) std::cout << x << std::endl

struct node {
	node *prev;
	node *next;
	vertex &v;
	node(vertex &v): prev(NULL), next(NULL), v(v) {}
};

node *head;
int push_count = 0;
int relabel_count = 0;


void initialize(graph *g)
{
	head = new node(g->v()[g->n()-1]);
	for (int i = 3; i < g->n(); ++i) {
		node *current = new node(g->v()[g->n() - i + 1]);
		current->next = head;
		head->prev = current;
		head = current;
	}

	// initialize preflow
	g->s()->set_height(g->n());
	vector<edge*>* edges = g->s()->edges();
	for (int i = 0; i < edges->size(); i++) {
		edge* e = edges->at(i);
//		e->update_flow(g->s(), e->upper(g->s()));
//		e->opposite(g->s())->update_excess(e->upper(g->s()));
		e->push_flow(e->upper());
		e->v_op()->update_excess(e->upper());
	}
}

void discharge(vertex &v)
{
	edge *cur;
	int c = 0;
	while (v.excess() > 0) {
//		print("count :" << ++c << ", size: " << v.edges()->size());
		cur = v.cur_edge();
		if (cur->residue() > 0 && v.height() == cur->v_op()->height() + 1) {
			push(&v, cur);
			push_count++;
//			print("vertex " << v.index() << ", excess: " << v.excess());
		}
		if (v.is_last()) {
			relabel(&v);
			relabel_count++;
//			print("vertex: " << v.index() << ", height: " << v.height());
		}
		v.next_edge();
	}
}

void relabel_to_front(graph* g)
{
	node *cur = head;

	while (cur != NULL) {
		int h = cur->v.height();
		discharge(cur->v);
		if (cur->v.height() > h && cur != head) {
			cur->prev->next = cur->next;
			if (cur->next != NULL)
				cur->next->prev = cur->prev;
			cur->prev = NULL;
			cur->next = head;
			head->prev = cur;
			head = cur;
// 			print("head is: " << head->v.index());
		}
		cur = cur->next;
//		g->display_flow();
	}

	print("Push count: " << push_count);
	print("Relabel count: " << relabel_count);
}

