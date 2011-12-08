#include <cstdlib>
#include <cassert>
#include <iostream>
#include <vector>
#include <queue>
#include <cilk/cilk.h>
#include "graph.h"
#include "limits.h"

using namespace std;

void push(vertex* v, edge *e)
{
        assert(v->excess() > 0);
        assert(e->residue(v) > 0);
        assert(v->height() == e->opposite(v)->height() + 1)
        int d = min(v->excess(), e->residue(v));
        e->updateFlow(v);
}

void relabel(vertex*v)
{
	vector<edge*>* edges = v->edges();
	numOfEdges = edges->size();
	int minHeight = INT_MAX;

	 for( int i = 0; i <numOfEdges, i++){
                edge* currentEdge = (*edges)[i];
                vertex* w = currentEdge->opposite(v);
                if (currentEdge.residue(v) > 0 && w.height()+1 < minHeight)
                        minHeight =  w.height()+1;
                	v.set_height(minHeight);

        }
	
}

void push_relabel(vertex* v)
{
	assert(v->excess() > 0);
	edge* e = v->nextEdge();

	if (e == 0){
		relabel(v);
	}
	else{
		push(v, e);
	}	

}

void discharge(queue<vertex*>* Q)
{
	vertex* v = Q->pop();
	int h = v->height();
	while (v->excess() != 0 && v->height() == h){
		push_relabel(v);
		vertex* w = currentEdge.opposite(v);
		if ( w->excess() > 0 )
			Q->push_back(w);
	}
	if ( v->excess() > 0 )
		Q->push_back(v);
}
