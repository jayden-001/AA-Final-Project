#include <cstdlib>
#include <vector>
#include "vertex.h"
#include "edge.h"

using namespace std;

class graph
{
	private:
		int v_num, m_num;
		vertex source;
		vertex sink;
		vector<vertex> vertices;
		vector<edge> edges;

	public:
		graph(vector<vertex>&, vector<edge>&);
		~graph();
		int n();
		int m();
		vertex& s();
		vertex& t();
		vector<vertex>& v();
		vector<edge>& e();
};

graph::graph(vector<vertex> v, vector<edge> e) :
	vertices(v), edges(e), n_num(v.size()), m_num(e.size())
{
	source.set_h(n_num);
}

int graph::n() { return n_num; }

int graph::m() { return m_num; }

vertex& graph::s() { return source; }

vertex& graph::t() { return sink; }

vector<vertex>& graph::v() { return vertices; }

vector<vertex>& graph::e() { return edges; }
