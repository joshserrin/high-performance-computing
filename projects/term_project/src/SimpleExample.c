#include <stdio.h>
#include "RoomyGraph.h"

/*
Creates and prints the graph seen in diagrams/simple-graph.png.
*/
void createSimpleGraph() {
	uint64 one = 1;
	uint64 two = 2;
	uint64 three = 3;
	uint64 four = 4;
	uint64 five = 5;

	// From our sample graph we know that 1 has the most children (3) so 
	// maxEdges MUST be at least that number.
	uint64 maxEdges = 3;
	// We also know that there are 5 nodes.
	uint64 initialCapacity = 5;
	RoomyGraph *g = RoomyGraph_make("simple-graph", maxEdges, initialCapacity);

	// Add all the nodes
	RoomyGraph_addNode(g, one);
	RoomyGraph_addNode(g, two);
	RoomyGraph_addNode(g, three);
	RoomyGraph_addNode(g, four);
	RoomyGraph_addNode(g, five);

	// Now, add all the edges
	RoomyGraph_addEdge(g, one, two);
	RoomyGraph_addEdge(g, one, three);
	RoomyGraph_addEdge(g, one, five);
	RoomyGraph_addEdge(g, three, one);
	RoomyGraph_addEdge(g, five, five);
	
	RoomyGraph_sync(g);
	uint64 nodeCount = RoomyGraph_nodeCount(g);
	printf("There are %lli nodes.\n", nodeCount);
	RoomyGraph_print(g);
	
	RoomyGraph_destroy(g);
}
int main(int argc, char **argv) {
	Roomy_init(&argc, &argv);

	createSimpleGraph();

	Roomy_finalize();
}
