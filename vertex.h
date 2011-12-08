#include <cstdlib>
#include <cassert>
#include <iostream>

using namespace std;

class edge;

class vertex
{
	private:
		int height;
		int excess;
		int name;
		vector<edge> outgoing;
		vector<edge> ingoing;

	public:
		vertex();
		~vertex();

		int h();
		void set_h(int); 
		
		int e();
		void adjust_e(int);
		
		int	name();
		
		vector<edge> out();
		vector<edge> in();
};

int vertex::h() { return height; }

void vertex::set_h(int i)
{
	assert(i >= 0);
	height = i;
}

int vertex::e() { return excess; }

void vertex::adjust_e(int i)
{
	excess += i;
	assert(excess >= 0);
}

int vertex::name() { return name; }

vector<edger>& vertex::out() { return outgoing; }

vector<edger>& vertex::in() { return ingoing; }


