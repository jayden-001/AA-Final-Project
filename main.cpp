#include <iostream>
#include <assert.h>
#include "graph.h"
#include "vertex.h"
#include "edge.h"
#include "graph_constructor.cpp"

#define print(x) std::cout << x << std::endl

int main(int argc, char **argv)
{
  generateEasyGraph();
	vertex v(5);
  print("Index of v is");
  print(v.index());
  return 0;
}
