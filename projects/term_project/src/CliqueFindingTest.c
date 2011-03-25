#include <stdio.h>
#include "RoomyGraph.h"
#include <assert.h>

// This graph is taken from 'Social Network Analysis: Methods and Application'
// by Wasserman et al page 255
RoomyGraph* createFig71() {
	RoomyGraph *g = RoomyGraph_make("fig 7.1", 5, 7);

  uint64 i;
  for(i = 1; i <= 7; i++) {
    RoomyGraph_addNode(g, i);
  }
	
	uint64 c1[3] = { 2, 3, 5 };
	RoomyGraph_addEdges(g, 1, c1, 3);

	uint64 c2[3] = { 1, 3, 7 };
	RoomyGraph_addEdges(g, 2, c2, 3);

	uint64 c3[5] = { 1, 2, 4, 5, 6 };
	RoomyGraph_addEdges(g, 3, c3, 5);

	uint64 c4[4] = { 3, 5, 6, 7 };
	RoomyGraph_addEdges(g, 4, c4, 4);

	uint64 c5[4] = { 1, 3, 4, 6 };
	RoomyGraph_addEdges(g, 5, c5, 4);

	uint64 c6[3] = { 3, 4, 5 };
	RoomyGraph_addEdges(g, 6, c6, 3);

	uint64 c7[2] = { 2, 4 };
	RoomyGraph_addEdges(g, 7, c7, 2);
	
	RoomyGraph_sync(g);

	return g;
}
void runSimpleCliqueFinding() {
	RoomyGraph *g = createFig71();
	printf("The graph...\n");
	RoomyGraph_print(g);
	
	RoomyGraph_findCliques(g);
	
	return;
}
void main(int argc, char **argv) {
	Roomy_init(&argc, &argv);

	runSimpleCliqueFinding();

	Roomy_finalize();
}
