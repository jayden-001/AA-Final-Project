#include <iostream>
#include <assert.h>
#include "graph.h"
// #include "vertex.h"
// #include "edge.h"
#include "graph_constructor.cpp"
#include "sequential_maxflow.cpp"
#include "parallel_maxflow.cpp"
#include <cilk/reducer_opadd.h>

#define print(x) std::cout << x << std::endl

int main(int argc, char **argv)
{
  graph* g = generate_easy_graph(argv[1]);
  g->display_upper();
  print("solving sequential...");
  sequential_maxflow(g);
  if (g->is_valid_flow())
	cout<<"flow is valid: "<<g->flow()<<endl;
  else
	cout<<"flow is invalid"<<endl;
  return 0;
}
