#include <stdio.h>
#include "RoomyGraph.h"
#include <assert.h>

RoomyGraph *g;

int main(int argc, char ** argv) {

  Roomy_init(&argc,&argv);
  if(createGraph()) {
    printf("All tests passed\n");
  } else {
    printf("ERROR OCCURRED... SEE LOGS\n");
  }
	Roomy_finalize();
}
// Failing functions should return 1, otherwise 0.
int createGraph() {
  uint64 maxEdges = 10;
  uint64 initialCapacity = 10;
  RoomyHashTable_make("a",1,1,10);
	g = RoomyGraph_makeBytes("g", sizeof(uint64), maxEdges, initialCapacity);

  assert(NULL != g);
  assert(g->maxEdges == 10);
  assert(NULL != g->graph);

  return 1;
}
