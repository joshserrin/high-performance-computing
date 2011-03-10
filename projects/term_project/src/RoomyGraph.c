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
	void *EMPTY = calloc(1 + g->maxEdges, sizeof(uint64));
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
	printf("%lli -> [%p]\n", *(uint64 *)k, v);
}
void RoomyGraph_print(RoomyGraph *g) {
	RoomyHashTable_map(g->graph, printNodeAndChildren);
	RoomyGraph_sync(g);
}
// Private function that adds the newEdge to the nodes edgeList
void addEdge(void *node, void *oldEdgeList, void *newEdge, void *newEdgeList) {
	int i = 0;
	// START HERE!
	// WE NEED TO ADD NEWEDGE TO OLDEDGELIST AND SET IT TO NEWEDGELIST.
	// HOW DO WE MAKE SURE TO NOT ADD TOO MANY NODES!!!!
	// ALSO, HOW DO WE KNOW HOW MANY NODES ARE IN OLDEDGELIST?
	newEdgeList = oldEdgeList;
}
void RoomyGraph_addEdge(RoomyGraph *g, uint64* from, uint64* to) {
	RoomyHashTable_update(g->graph, from, to, addEdge);
}
int RoomyGraph_containsEdge(RoomyGraph *g, uint64* from, uint64* to) {
	return 0;
}
RoomyGraph* RoomyGraph_make(char* name, uint64 maxEdges, 
                                 uint64 initialCapacity) {
  RoomyGraph* g = (RoomyGraph *)malloc(sizeof(RoomyGraph));

  uint64 keySize = sizeof(uint64);
  uint64 valueSize = maxEdges*keySize;

  g->graph = RoomyHashTable_make(name, keySize, valueSize, initialCapacity);
  g->maxEdges = maxEdges;

	// We must also attach the functions used for searching
	RoomyHashTable_registerUpdateFunc(g->graph, addEdge, keySize);

  return g;
}
