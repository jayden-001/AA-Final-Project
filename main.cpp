#include <iostream>
#include <assert.h>
#include "graph.h"
// #include "vertex.h"
// #include "edge.h"
#include "graph_constructor.cpp"
#include "sequential_maxflow.cpp"
#include "parallel_maxflow.cpp"
#include <cilk/reducer_opadd.h>
#include <time.h>

#define print(x) std::cout << x << std::endl

void result(graph*, clock_t, clock_t);
int parallel(graph*);
int sequential(graph*);

int main(int argc, char **argv)
{
//   graph* g = generate_easy_graph(argv[1]);
  srand(1);
  graph* g = generate_dumbbell_graph(10);
  parallel(g);
//  g = generate_easy_graph(argv[1]);
  delete g;
  srand(1);
  g = generate_dumbbell_graph(10);
//   g = generate_easy_graph(argv[1]);
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
<<<<<<< HEAD:main.cpp
////  g->display_flow();
=======
//   g->display_flow();
>>>>>>> c75a2514aa72700a1c20124918cf5cc343474177:main.cpp
  if (g->is_valid_flow())
		cout<<"flow is valid: "<<g->flow()<<endl;
  else
		cout<<"flow is invalid"<<endl;
	cout << ((double) (end - start)) / CLOCKS_PER_SEC
			<< " seconds in total" << endl;
}
