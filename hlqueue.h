#include <cstdlib>
#include <vector>
#include "vertex.h"

using namespace std;

class hlqueue
{
	private:
		vector<vertex*> _bucket;
		vector<bool> _active;
		int _highest;
		int _size;
		int _n;
		vertex *s;
		vertex *t;

	public:
		hlqueue(graph* g);
		int highest() { return _highest; };
		int size() { return _size; };
		vertex* pop();
		void push(vertex* v);
		void remove(vertex* v);
		void gap_relabel(int h);
		bool contains(int i) { return _active[i]; }
		bool contains(vertex *v) { return _active[v->index()]; }
		bool empty() { return _size == 0; }
};

hlqueue::hlqueue(graph* g): _size(0), _highest(0), _n(g->n())
{
	s = g->s();
	t = g->t();
	_active.resize(_n + 1, false);
	_active[0] = true;
	_active[1] = true;
	_bucket.resize(_n + 1, NULL);
}

void hlqueue::push(vertex *v)
{
	assert(!contains(v->index()));
	_active[v->index()] = true;
	_size++;

	v->set_next( _bucket[v->height()]->next() );
	_bucket[v->height()]->next()->set_prev(v);
	_bucket[v->height()] = v;
	_highest = max(_highest, v->height());

	assert( _bucket[_highest] != NULL );
}

void hlqueue::remove(vertex *v)
{
	assert(contains(v->index()));
	_active[v->index()] = false;
	_size--;

	vertex *pr = v->prev();
	vertex *ne = v->next();
	pr->set_next(ne);
	ne->set_prev(pr);

	v->set_prev(NULL);
	v->set_next(NULL);

	if (  _bucket[_highest] == NULL )
		_highest--;

	assert( _bucket[_highest] != NULL );
}


//return true if become empty
vertex* hlqueue::pop()
{
	assert( _bucket[_highest] != NULL );

	_size--;
	vertex *v = _bucket[_highest];
	_bucket[_highest] = v->next();
	_active[v->index()] = false;
	if (_bucket[_highest] == NULL)
		_highest--;
	assert( _bucket[_highest] != NULL );
	return v;
}
/*
void hlqueue::gap_relabel(int h)
{
	
	for (int i = h + 1; i < _n; ++i) {
		for (j = _bucket[i].begin(); j != _bucket[i].end(); ++j) {
			(*j)->set_height(_n);
			_size--;
		}
		_bucket[_n].splice(it, _bucket[i]);
	}
}
*/
