#include <stdio.h>
#include "RoomyGraph.h"
#include <string.h>
#include <RoomyHashTable.h>

// ===================================================
// If documentation is lacking, please check RoomyGraph.h
// ===================================================
void RoomyGraph_destroy(RoomyGraph *g) {
	RoomyHashTable_destroy(g->graph);
	free(g);
}
void RoomyGraph_addNode(RoomyGraph *g, uint64* node) {
	// We are adding 1 because that is what stores the stored
	// number of edges.
	void *EMPTY = calloc((1 + g->maxEdges), sizeof(uint64));
	RoomyHashTable_insert(g->graph, node, EMPTY);
}
void RoomyGraph_sync(RoomyGraph *g) {
	RoomyHashTable_sync(g->graph);
}
// Private members and functions for containsNode
uint64 count; // if you are going to use this, don't forget to initialize it!
uint64 searchNode;
// We don't care about the val (which is the edgelist, BTW)
void countMatches(void* key, void* val) {
	uint64 node = *(uint64 *)key;
	if(node == searchNode) { count++; }
}
// End private members
int RoomyGraph_containsNode(RoomyGraph *g, uint64* node) {
	searchNode = *node;
	count = 0;
	RoomyHashTable_map(g->graph, countMatches);
	RoomyGraph_sync(g);
	if(count > 0) { return 1; }
	else { return 0; }
}
uint64 glo_from;
uint64 glo_to;
void countEdges(void *key, void *value) {
	uint64 from = *(uint64 *)key;
	if(from == glo_from) {
		uint64 *edges = (uint64 *)value;
		uint64 size = edges[0];
		uint64 i;
		for(i = 0; i < size; i++) {
			uint64 e = edges[i + 1];
			if(e == glo_to) {	count++; }
		}
	} 
}
int RoomyGraph_containsEdge(RoomyGraph *g, uint64 from, uint64 to) {
	glo_from = from;
	glo_to = to;
	count = 0;
	RoomyHashTable_map(g->graph, countEdges);
	RoomyGraph_sync(g);
	if(count > 0) return 1;
	else return 0;
}
// Private functions for nodeCount
void increaseCount(void *k, void *v) {
	count++;
}
// end private function
int RoomyGraph_nodeCount(RoomyGraph *g) {
	count = 0;
	RoomyHashTable_map(g->graph, increaseCount);
	RoomyGraph_sync(g);
	return count;
}
void printNodeAndChildren(void *k, void *v) {
	uint64 *edges = (uint64 *)v;
	uint64 size = edges[0];
	uint64 i;
	printf("%lli -> [", *(uint64 *)k);
	for(i = 0; i < size; i++) {
		uint64 edge = edges[i + 1];
		printf("%lli, ", edge);
	}
	printf("]\n");
}
void RoomyGraph_print(RoomyGraph *g) {
	RoomyHashTable_map(g->graph, printNodeAndChildren);
	RoomyGraph_sync(g);
}
typedef struct {
	uint64 newEdge; // the new edge to be added
	uint64 maxEdges; // the max number of edges that can leave a node
} AddEdgePassed;
// Private function that adds the newEdge to the nodes edgeList
void addEdge(void *node, void *oldEdgeList, void *passed, void *newEdgeList) {
	AddEdgePassed *arg = (AddEdgePassed *)passed;
	// TODO HOW DO WE MAKE SURE TO NOT ADD TOO MANY NODES!!!!
	memcpy(newEdgeList, oldEdgeList, sizeof(uint64)*(arg->maxEdges+1));
	uint64 *edges = newEdgeList;
	uint64 size = edges[0];
	uint64 insertIndex = size + 1;
	uint64 newSize = size + 1;
	edges[0] = newSize;
	edges[insertIndex] = arg->newEdge;
}
void RoomyGraph_addEdge(RoomyGraph *g, uint64* from, uint64* to) {
	AddEdgePassed arg;
	arg.newEdge = *to;
	arg.maxEdges = g->maxEdges;
	RoomyHashTable_update(g->graph, from, &arg, addEdge);
}
RoomyGraph* RoomyGraph_make(char* name, uint64 maxEdges, 
                                 uint64 initialCapacity) {
  RoomyGraph* g = (RoomyGraph *)malloc(sizeof(RoomyGraph));

  uint64 keySize = sizeof(uint64);
  uint64 valueSize = (maxEdges + 1)*keySize;

  g->graph = RoomyHashTable_make(name, keySize, valueSize, initialCapacity);
  g->maxEdges = maxEdges;

	// We must also attach the functions used for searching
	RoomyHashTable_registerUpdateFunc(g->graph, addEdge, sizeof(AddEdgePassed));

  return g;
}
