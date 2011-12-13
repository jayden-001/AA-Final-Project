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
		vertex *s;
		vertex *t;
	public:
		hlqueue(graph* g);
		int highest() { return _highest; };
		int size() (return _size; );
		vertex* pop();
		void push(vertex* v);
		bool is_in(int index);
};

hlqueue::hlqueue(graph* g)
{
	s = g->s();
        t = g->t();
        active.resize(g->n(), false);
        active[0] = true;
        active[1] = true;
        bucket.resize(g->n());
}
