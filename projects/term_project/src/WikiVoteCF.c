#include <stdio.h>
#include "RoomyGraph.h"


int main(int argc, char **argv) {
  Roomy_init(&argc, &argv);
  
	uint64 maxEdges = 12710;
	uint64 initialCapacity = 999;
	RoomyGraph *g = RoomyGraph_make("wiki-vote-cf", maxEdges, initialCapacity);
  
  printf("Populating Graph\n");
  FILE *f1 = fopen("./datasets/Wiki-Vote50000.dot", "r");
  RoomyGraph_addNodesFromDigraph(g, f1);
  FILE *f2 = fopen("./datasets/Wiki-Vote50000.dot", "r");
  RoomyGraph_addEdgesFromDigraph(g, f2);
  fclose(f1);
  fclose(f2);
  
  printf("Graph has been populated\n");
  printf("Clique Finding started at %ld\n", time(NULL));
  RoomyGraph_findCliques(g);
  printf("Clique Finding ended at %ld\n", time(NULL));
  
  Roomy_finalize();
}
