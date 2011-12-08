#include <iostream>
#include <assert.h>
#include "graph.h"
#include "vertex.h"
#include "edge.h"
#include "graph_constructor.cpp"
#include "sequential_maxflow.cpp"

int main(int argc, char **argv)
{
  graph* g = generateEasyGraph();
  sequential_maxflow(g);
  return 0;
}
