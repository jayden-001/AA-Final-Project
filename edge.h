#include <cstdlib>
#include <cassert>
#include <iostream>

using namespace std;

class vertex;

class edge
{
	private:
		int cost;
		int upper1; //capacity
		int upper2; //capacity
		int flow;
		int residue;
		vertex v1, v2;

	public:
		edge(vertex&, vertex&, int);
		~edge();
		int c();
		int f();
		int rf();
		void adjust_rf(int);
		vertex& v1();
		vertex& v2();
		void set_rev();
};

edge::edge(vertex& v1, vertex& v2, int c = 0) :
	from(v1), to(v2), cost(c), residue(c)
{}

int edge::c() { return cost; }

int edge::f() { return flow; }

int edge::rf() { return residue; }

void edge::adjust_rf(int i)
{
	residue += i;
	assert(residue <= cost || residue <= reverse.c());
}

vertex& edge::v1() { return from; }

vertex& edge::v2() { return to; }

edge& rev() { return *reverse; }

void set_reverse(edge& e) { reverse = &e; }
