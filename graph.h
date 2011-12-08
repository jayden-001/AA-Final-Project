#include <cstdlib>
#include <vector>
#include "vertex.h"
#include "edge.h"

using namespace std;

class graph
{
	private:
		int _n, _m;
		vector<vertex&> vertices;
		vector<edge&> edges;

	public:
		graph();
		~graph();
		
		int add_vertex();
		int add_edge(int, int, int, int, int);
		
		int n();
		int m();
		vertex& s();
		vertex& t();
		vector<vertex&>& v();
		vector<edge&>& e();
};

graph::graph() : _n(2), _m(0)
{
	add_vertex();	// source
	add_vertex();	// sink
}

int graph::add_vertex()
{
	vertices.push_back(*(new vertex()));
	_n++;
	return _n - 1;
}

int graph::add_edge(int v1, int v2, int upper12, int upper21, int cost = 0)
{
	assert(v1 < n());
	assert(v2 < n());
	assert(upper12 >= 0);
	assert(upper21 >= 0);
	vertices.push_back(*(new edge(vertices[v1], vertices[v2], upper12, upper21, cost)));
	_m++;
	return _m - 1;
}

int graph::n() { return _n; }

int graph::m() { return _m; }

vertex& graph::s() { return vertices[0]; }

vertex& graph::t() { return vertices[1]; }

vector<vertex&>& graph::v() { return vertices; }

vector<vertex&>& graph::e() { return edges; }
