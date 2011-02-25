#include <stdio.h>
#include "RoomyGraph.h"
#include <string.h>
#include <RoomyHashTable.h>

// ===================================================
// If documentation is lacking, please check RoomyGraph.h
// ===================================================
/*uint64 searchNode;
uint8 sameNode(void *key, void *value) {
	// For the tests we are only using uint64s so we can cast safetly.
	// TODO: this will need to be updated or the function will break.
	printf("key: %lli\n", *(uint64 *)key);
	printf("searchKey: %lli\n", searchNode);
	uint64 *node = (uint64 *)key;
	uint64 sNode = searchNode;
	if(*node == sNode) {	return 1;	}
	else { return 0; }
}*/
RoomyGraph* RoomyGraph_make(char* name, uint64 bytesPerElt, uint64 maxEdges, 
                                 uint64 initialCapacity) {
  RoomyGraph* g = (RoomyGraph *)malloc(sizeof(RoomyGraph));

  uint64 keySize = bytesPerElt;
  uint64 valueSize = maxEdges*bytesPerElt;

  g->graph = RoomyHashTable_make(name, keySize, valueSize, initialCapacity);
  g->maxEdges = maxEdges;
	g->bytesPerElt = bytesPerElt;

	// We must also attach the functions used for searching
//	RoomyHashTable_attachPredicate(g->graph, sameNode);

  return g;
}
void RoomyGraph_destroy(RoomyGraph *g) {
	RoomyHashTable_destroy(g->graph);
	free(g);
}
void RoomyGraph_addNode(RoomyGraph *g, void* node) {
	void *EMPTY = malloc(g->bytesPerElt * g->maxEdges);
	RoomyHashTable_insert(g->graph, node, EMPTY);
}
void RoomyGraph_sync(RoomyGraph *g) {
	RoomyHashTable_sync(g->graph);
}
// Private members and functions for containsNode
uint64 count; // if you are going to use this, don't forget to initialize it!
uint64 searchNode;
void countMatches(void* key, void* val) {
	uint64 node = *(uint64 *)key;
	if(node == searchNode) { count++; }
}
// End private members
int RoomyGraph_containsNode(RoomyGraph *g, void* node) {
	searchNode = *(uint64 *)node;
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
