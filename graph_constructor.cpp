#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "graph.h"

using namespace std;

graph* generateEasyGraph()
{
	
	ifstream reader;
	reader.open("easy.graph");
	
	bool started = false;
	int state = 0;
	int n = 0;
	int v1 = 0;
	int v2 = 0;
	
	int* edges;
	
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
						edges = new int[n*n];
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
					edges[v1*n + v2] = c;
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
	for (int v1 = 0; v1 < n; v1++) {
		for (int v2 = v1+1; v2 < n; v2++) {
			cout << edges[v1*n + v2];
			if (edges[v1*n + v2] != 0 || edges[v2*n + v1] != 0)
				g->add_edge(v1,v2,edges[v1*n + v2],edges[v2*n + v1], 0);
		}
		cout << endl;
	}
	
 	cout << g->n() << endl;
 	cout << g->m() << endl;
	
	delete edges;
	
	return g;
	
}
