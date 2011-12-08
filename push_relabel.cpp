#include <cstdlib>
#include <cassert>
#include <iostream>
#include <cilk/cilk.h>
#include "graph.h"

bool push(edge &e)
{
	assert(e.v1.excess > 0);
	assert(e.v1.height = e.v2.height + 1);
	int d = min(v.excess, e.residue);
	e.v1.excess -= d;
	e.v2.excess += d;
	e.residue -= d;
	e.reverse->residue += d;
}

bool relabel(vertex &v)
{
	assert(v.excess > 0);
}
