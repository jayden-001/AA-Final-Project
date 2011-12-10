#include <cstdlib>
#include <iostream>
#include <assert.h>
#include <time.h>
#include "graph.h"
#include "graph_constructor.cpp"
#include "sequential_maxflow.cpp"
#include "parallel_maxflow.cpp"
#include "reducer_list.cpp"
//#include <cilk/cilk_stub.h>

#define print(x) std::cout << x << std::endl

void result(graph*, clock_t, clock_t);
int parallel(graph*);
int sequential(graph*);

int main(int argc, char **argv)
{
   graph* g = generate_complete_graph(atoi(argv[1]));
//	parallel(g);
//  delete g;
   g = generate_complete_graph(atoi(argv[1]));
  sequential(g);
  return 0;
}

int parallel(graph *g)
{
	clock_t start, end;
	print("running parallel push relabel");
	parallel_solver solver(g);
	start = clock();
	solver.solve_maxflow();
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
//   g->display_flow();
  if (g->is_valid_flow())
		cout<<"flow is valid: "<<g->flow()<<endl;
  else
		cout<<"flow is invalid"<<endl;
	cout << ((double) (end - start)) / CLOCKS_PER_SEC
			<< " seconds in total" << endl;
}
