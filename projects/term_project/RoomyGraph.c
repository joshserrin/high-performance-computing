#include "RoomyGraph.h"
#include <string.h>
#include <RoomyHashTable.h>

// ===================================================
// If documentation is lacking, please check RoomyGraph.h
// ===================================================

RoomyGraph* RoomyGraph_makeBytes(char* name, uint64 bytesPerElt, uint64 maxEdges, 
                                 uint64 initialCapacity) {
  RoomyGraph* g = MALLOC(sizeof(RoomyGraph));

  uint64 keySize = bytesPerElt;
  uint64 valueSize = maxEdges*bytesPerElt;

  g->graph = RoomyHashTable_make(name, keySize, valueSize, initialCapacity);
  g->maxEdges = maxEdges;

  return graph;
}
