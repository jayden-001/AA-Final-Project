#include <cstdlib>
#include <vector>
#include <list>

using namespace std;

class hlqueue
{
	private:
		vector<list<vertex*> > _bucket;
		vector<bool> _active;
		int _highest;
		int _size;
		int _n;
		vertex *s;
		vertex *t;

	public:
		hlqueue(graph* g);
		int highest() { return _highest; };
		int size() (return _size; );
		vertex* pop();
		void push(vertex* v);
		void gap_relabel(int h);
		bool contains(int i) { return _active[i]; }
		bool contains(vertex *v) { return _active[vertex->index()]; }
		bool empty() { return _size == 0; }
		list<vertex*> remaining() { return _bucket[_n]; }
};

hlqueue::hlqueue(graph* g): _size(0), _highest(0), _n(g->n())
{
	s = g->s();
	t = g->t();
	_active.resize(_n + 1, false);
	_active[0] = true;
	_active[1] = true;
	_bucket.resize(_n + 1);
}

void hlqueue::push(vertex *v)
{
	assert(!contains(v->index()));
	_active[v->index()] = true;
	_size++;
	_bucket[v->height()].push_front(v);
	_highest = max(_highest, v->height());
}

//return true if become empty
vertex* hlqueue::pop()
{
	_size--;
	vertex *v = _bucket[_highest].front();
	_bucket[_highest].pop_front();
	_active[v->index()] = false;
	if (_bucket[_highest].empty())
		_highest--;
	assert(!_bucket[_highest].empty());
	return v;
}

void hlqueue::gap_relabel(int h)
{
	list<vertex*>::iterator it = _bucket[_n].begin();
	list<vertex*>::iterator j;
	for (int i = h + 1; i < _n; ++i) {
		for (j = _bucket[i].begin(); j != _bucket[i].end(); ++j) {
			(*j)->set_height(_n);
			_size--;
		}
		_bucket[_n].splice(it, _bucket[i]);
	}
}
