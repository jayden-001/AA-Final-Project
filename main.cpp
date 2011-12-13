#include <cstdlib>
#include <iostream>
#include <assert.h>
#include <time.h>
#include "graph.h"
#include "graph_constructor.cpp"
#include "sequential_maxflow.cpp"
// #include "parallel_maxflow.cpp"
// #include "reducer_list.cpp"
//#include "relabel_to_front.cpp"
#include "gap_heuristic.cpp"
#include "edmond-karp.cpp"

using namespace std;

#ifndef print
#define print(x) cout << x << endl
#endif

#define OPS_NUM 5

void result(graph*, clock_t, clock_t);
int parallel(graph*);
int sequential(graph*);
int gap(graph*);
int edmondkarp(graph*);

int look_up(string s)
{
	string ops[OPS_NUM] = {"-f", "-r", "-g", "-n1", "-n2"};

	for (int i = 0; i < OPS_NUM; i++){
		if (ops[i].compare(s) == 0)
			return i;
	} 
	return -1;
}

void help() {
	print("-f filename: I/O file");
	print("-r: running mode (0=sequential, 1=gap, 2=edmondkarp)");
	print("-g: generation mode (0=trivial, 1=bipartite, 2=complete, 3=dumbbell)");
	print("-n1: argument 1 for graph generation");
	print("-n2: argument 2 for graph generation");
}

void error() {
	print("Invalid arguments!");
	help();
}

int main(int argc, char **argv)
{
	bool isRun = false;
	bool isGen = false;
	string filename;
	int mode = -1;
	int n1 = 5;
	int n2 = 5;
	
	if (argc <= 1) {
		help();
		return 0;
	}		
	
	for (int i = 1; i < argc; i++) {
		switch (look_up(argv[i])) {
			case 0:	// f
				filename = argv[++i];
				break;
			case 1:	// r
				isRun = true;
				mode = atoi(argv[++i]);
				break;
			case 2:	// g
				isGen = true;
				mode = atoi(argv[++i]);
				break;
			case 3:	// n1
				n1 = atoi(argv[++i]);
				break;
			case 4:	// n2
				n2 = atoi(argv[++i]);
				break;
		}
	}

	if ((isRun && isGen) || (!isRun && !isGen)) {
		error();
		return 0;
	}
	
	if (isRun) {
		graph* g = generate_easy_graph(filename);
	
		switch (mode) {
			case 0:	// sequential
				sequential(g);
				break;
			case 1:	// gap
				gap(g);
				break;
			case 2:	// edmondkarp
				edmondkarp(g);
				break;
		}
	}
	
	if (isGen) {
		graph* g;
	
		switch (mode) {
			case '0': // trivial
				g = generate_trivial_graph();
				break;
			case '1': // bipartite
				g = generate_bipartite_graph(n1, n2);
				break;
			case '2': // complete
				g = generate_complete_graph(n1);
				break;
			case '3': // dumbbell
				g = generate_dumbbell_graph(n1);
				break;
		}
		
		g->write_graph(filename);
	}
//	graph* g = generate_easy_graph(argv[1]);
//	graph* g = generate_bipartite_graph(100,100);
//	g->write_graph(argv[1]);
//	graph* g = generate_bipartite_graph(1000,1000);
// 	edmondkarp(g);
//	gap(g);
// 	g = generate_easy_graph(argv[1]);
// 	g = generate_trivial_graph();
//  sequential(g);
	return 0;
}

// int parallel(graph *g)
// {
// 	clock_t start, end;
// 	print("running parallel push relabel");
// 	parallel_solver solver(g);
// 	start = clock();
// 	solver.solve_maxflow();
// 	end = clock();
// 	result(g, start, end);
//  	return 0;
// }

int edmondkarp(graph *g)
{
	clock_t start, end;
  print("running edmond karp");
  start = clock();
	edmond_karp(g);
  end = clock();
  result(g, start, end);
  return 0;
}

int gap(graph *g)
{
	initialize(g);
	clock_t start, end;
  print("running gap relabel");
  start = clock();
	gap_relabel(g);
  end = clock();
  result(g, start, end);
  return 0;
}

int sequential(graph *g)
{
	clock_t start, end;
  print("running sequential push relabel");
  start = clock();
	sequential_maxflow(g);
  end = clock();
  result(g, start, end);
  return 0;
}

void result(graph *g, clock_t start, clock_t end)
{
//    g->display_flow();
   if (g->is_valid_flow())
 		cout<<"flow is valid: "<<g->flow()<<endl;
   else
 		cout<<"flow is invalid"<<endl;
	cout << ((double) (end - start)) / CLOCKS_PER_SEC
			<< " seconds in total" << endl;
}
