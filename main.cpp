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
//  cilk::reducer_opadd<int> r;
//  r += 10;
//  print("reducer value: ");
//  print(r.get_value());
  graph* g = generate_easy_graph();
//   g->display();
  print("solving parallel...");
  parallel_solver solver(g);
  g->display();
//  g = generate_easy_graph();
//  print("solving sequential...");
//  sequential_maxflow(g);
//  g->display();
  return 0;
}
