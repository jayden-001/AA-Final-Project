#include <cstdlib>
#include <cassert>
#include <iostream>
#include <vector>
#include <cilk/cilk.h>
#include "graph.h"
#include "limits.h"

using namespace std;

bool push(vertex& v, edge &e)
{
        assert(v.excess() > 0);
        assert(e.residue(v) > 0);
        assert(v.height() == e.opposite(v)->height() + 1)
        int d = min(v.excess(), e.residue(v));
        e.updateFlow(v);
}


bool push_relabel(vertex &v)
{
	assert(v.excess() > 0);
	vector<edge>* edges = v.edges();
	int numOfEdges = edges->size();
	int minHeight = INT_MAX;
	for( int i = 0; i <numOfEdges, i++){
		edge& currentEdge = (*edges)[i];
		vertex& w = *(currentEdge.opposite(v));
		if ( currentEdge.residue(v) > 0 &&
		     v.height() == w.height() + 1 )
			push(v, currentEdge);
		if (currentEdge.residue(v) > 0 && w.height()+1 < minHeight)
			minHeight =  w.height()+1;

		v.height(minHeight);
		
	}

}


