#include <cstdlib>
#include <iostream>
#include <assert.h>
#include "graph.h"
#include "vertex.h"
#include "edge.h"

#define print(x) std::cout << x << std::endl;

int main(int argc, char **argv)
{
	vertex v;
	v.set_height(5);
  print("Height of v is");
  print(v.height());
  return 0;
}
