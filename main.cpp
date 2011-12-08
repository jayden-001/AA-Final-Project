#include <iostream>
#include <assert.h>
#include "graph.h"
// #include "vertex.h"
// #include "edge.h"
#include "graph_constructor.cpp"
#include "sequential_maxflow.cpp"
#include "parallel_maxflow.cpp"

#define print(x) std::cout << x << std::endl

int main(int argc, char **argv)
{
  graph* g = generateEasyGraph();
  g->display();
  parallel_solver solver(g);
  sequential_maxflow(g);
  return 0;
}
