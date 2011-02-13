#include <stdio.h>
#include "RoomyGraph.h"
#include <assert.h>

RoomyGraph *g;

void main(void) {

  if(createGraph()) {
    printf("All tests passed");
  } else {
    printf("ERROR OCCURRED... SEE LOGS\n");
  }
}
// Failing functions should return 1, otherwise 0.
int createGraph() {
  uint64 maxEdges = 10;
  uint64 initialCapacity = 10;
  g = RoomyGraph_makeBytes("g", sizeof(uint64), maxEdges, initialCapacity);

  assert(NULL != g);
  assert(g->maxEdges == 10);
  assert(NULL != g->graph);

  return 1;
}
