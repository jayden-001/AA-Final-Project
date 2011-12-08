#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "graph.h"

using namespace std;

graph* generate_easy_graph()
{
	
	ifstream reader;
	reader.open("easy.graph");
	
	bool started = false;
	int state = 0;
	int n = 0;
	long long v1 = 0;
	long long v2 = 0;
	
	map <long long, int> costmap;
	
	// parsing
	char buffer[100];
	if (reader.is_open()) {
		while (!reader.eof()) {
			reader >> buffer;
			switch (buffer[0]) {
			case 'n':
				started = true;
				state = 1;
				break;
			case 'a':
				if (started) {
					state = 2;
				}
				break;
			default:
				switch (state) {
				case 1:
					state = 0;
					istringstream(string(buffer)) >> n; 
					if (n > 1) {
						cout << n << endl;
					}
					break;
				case 2:
					state = 3;
					istringstream(string(buffer)) >> v1;
					if (v1 == 1) {
						v1 = 0;
					} else if (v1 == n) {
						v1 = 1;
					}
					break;
				case 3:
					state = 4;
					istringstream(string(buffer)) >> v2;
					if (v2 == 1) {
						v2 = 0;
					} else if (v2 == n) {
						v2 = 1;
					}
					break;
				case 4:
					state = 0;
					int c;
					istringstream(string(buffer)) >> c;
					long long index = v1*n + v2;
					assert(index >= 0);
					costmap[index] = c;
				}
			}

		}
	}
	reader.close();
	
	// creating
	graph* g = new graph();
	for (int i = 2; i < n; i++) {
		g->add_vertex();
	}
	cout << g->n() << endl;
	
	map<long long,int>::iterator iter;   
	for( iter = costmap.begin(); iter != costmap.end(); iter++ ) {
		long long index = iter->first;
		long long v1 = index / n;
		long long v2 = index % n;
		assert (index >= 0);
		assert (v1 < n && v1 >= 0);
		assert (v2 < n && v2 >= 0);
		
		int upper12 = costmap.count(v1*n+v2) == 0 ? 0 : costmap[v1*n + v2];
		int upper21 = costmap.count(v2*n+v1) == 0 ? 0 : costmap[v2*n + v1];
		if (v1 >= v2 && upper21!=0)	continue;
		g->add_edge(v1,v2,upper12,upper21,0);
		
	}
	
 	cout << "n=" << g->n() << endl;
 	cout << "m=" << g->m() << endl;
 	cout << "Done generating easy graph!" << endl;
		
	return g;
	
}


graph* generate_trivial_graph()
{
	graph* g = new graph();
	g->add_vertex();
	g->add_vertex();

	g->add_edge(0,2,5,0,0);
	g->add_edge(0,3,1,0,0);
	g->add_edge(2,3,2,3,0);
	g->add_edge(3,1,5,0,0);
	g->add_edge(2,1,2,0,0);

	return g;
}
