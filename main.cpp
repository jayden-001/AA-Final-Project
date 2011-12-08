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
  graph* g = generate_easy_graph();
//  print("solving parallel...");
//  parallel_solver solver(g);
//  solver.solve_maxflow();
//  g->display();
//  g = generate_easy_graph();
  print("solving sequential...");
  g->display_upper();
  sequential_maxflow(g);
  g->display_flow();
  return 0;
}
