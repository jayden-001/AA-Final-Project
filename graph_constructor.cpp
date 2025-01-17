#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "graph.h"

using namespace std;

graph* generate_easy_graph(string graph_file)
{
	ifstream reader;
	reader.open(graph_file.c_str());
	
	bool started = false;
	int state = 0;
	int n = 0;
	int m = 0;
	long long v1 = 0;
	long long v2 = 0;
	
	map <long long, int> costmap;
	
	// parsing
	char buffer[100];
	if (reader.is_open()) {
		while (!reader.eof()) {
			reader >> buffer;
			switch (buffer[0]) {
			case 'p':
				if (buffer[1]  != '\0')
					break;
				started = true;
				state = 1;
				break;
			case 'a':
				if (started) {
					state = 3;
				}
				break;
			default:
				switch (state) {
				case 1:
					reader >> buffer;
					state = 2;
					istringstream(string(buffer)) >> n; 
					break;
				case 2:
					state = 0;
					istringstream(string(buffer)) >> m;
					break;
				case 3:
					state = 4;

					istringstream(string(buffer)) >> v1;
					if (v1 == 1) {
						v1 = 0;
					} else if (v1 == n) {
						v1 = 1;
					}
					break;
				case 4:
					state = 5;
					istringstream(string(buffer)) >> v2;
					if (v2 == 1) {
						v2 = 0;
					} else if (v2 == n) {
						v2 = 1;
					}
					break;
				case 5:
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
	graph* g = new graph(n, m);

	map<long long,int>::iterator iter;   
	for( iter = costmap.begin(); iter != costmap.end(); iter++ ) {
		long long index = iter->first;
		long long v1 = index / n;
		long long v2 = index % n;
		assert (index >= 0);
		assert (v1 < n && v1 >= 0);
		assert (v2 < n && v2 >= 0);
		
		int upper12 = costmap[v1*n + v2];

		g->add_edge(v1,v2,upper12);
	}
	
 	cout << "n=" << g->n() << endl;
 	cout << "m=" << g->m() << endl;
 	cout << "Done generating easy graph!" << endl;
		
	return g;
	
}

graph* generate_trivial_graph()
{
	graph* g = new graph(4,6);

	g->add_edge(0,2,5);
	g->add_edge(0,3,1);
	g->add_edge(2,3,2);
	g->add_edge(3,1,5);
	g->add_edge(3,2,3);
	g->add_edge(2,1,2);

	return g;
}

graph* generate_bipartite_graph(int a, int b)
{
	graph* g = new graph(a+b+2, a*b+a+b);
	
	// add edges
	for (int i = 2; i < a+2; i++) {
		g->add_edge(0,i,30*b);
	}
	
	for (int i = 2; i < a+2; i++) {
		for (int j = a+2; j < a+b+2; j++) {
			g->add_edge(i,j,rand()%30+1);
		}
	}
	
	for (int i = a+2; i < a+b+2; i++) {
		g->add_edge(i,1,30*a);
	}
	
	return g;
}

graph* generate_complete_graph(int graph_size)
{
	graph* g = new graph(graph_size, graph_size*(graph_size-1));

	// add edges
	for (int i = 0; i < graph_size; i++){
		for (int j = i+1; j < graph_size; j++){
			g->add_edge(i,j,rand()%1000 + 1);
			g->add_edge(j,i,rand()%1000 + 1);
		}
	}
	
	return g;
}

graph* generate_dumbbell_graph(int graph_size)
{
	graph* g = new graph(2*graph_size, 2*graph_size*(graph_size-1) + 1);
	
	// add edges
	for (int i = 0; i < graph_size; i++){
		for (int j = i+1; j < graph_size; j++){
			g->add_edge(i==0 ? 0 : i+1, j+1, rand()%1000 + 1);
			g->add_edge(j+1, i==0 ? 0 : i+1, rand()%1000 + 1);
		}
	}
	for (int i = graph_size; i < graph_size*2; i++){
		for (int j = i+1; j < graph_size*2; j++){
			g->add_edge(i==graph_size ? 1 : i,j,rand()%1000 + 1);
			g->add_edge(j,i==graph_size ? 1 : i,rand()%1000 + 1);
		}
	}
	g->add_edge(2, graph_size*2-1, 10);
	return g;
	
}