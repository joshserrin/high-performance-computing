#include <stdio.h>
#include "RoomyGraph.h"
#include <string.h>
#include <RoomyHashTable.h>

// ===================================================
// If documentation is lacking, please check RoomyGraph.h
// ===================================================

RoomyGraph* RoomyGraph_makeBytes(char* name, uint64 bytesPerElt, uint64 maxEdges, 
                                 uint64 initialCapacity) {
  RoomyGraph* g = (RoomyGraph *)malloc(sizeof(RoomyGraph));

  uint64 keySize = bytesPerElt;
  uint64 valueSize = maxEdges*bytesPerElt;

  printf("name = %s \n",name);
	fflush(stdout);
  g->graph = RoomyHashTable_make(name, keySize, valueSize, initialCapacity);
  g->maxEdges = maxEdges;

  return g;
}
/*
void main(void) {
  printf("compiles!");
}
*/
