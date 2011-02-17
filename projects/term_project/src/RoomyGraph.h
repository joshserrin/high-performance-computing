#include <RoomyHashTable.h>
#include <roomy.h>

/* 
RoomyGraphs consist of Nodes and Edges linking two nodes.
Nodes are of the same NodeType where a NodeType is the type 
of Node (e.g., integer, character).
*/
typedef struct {
  
  uint64 maxEdges; /* The maximum number of edges that leave a Node
   maxEdges is a requirement due to the fact that RoomyHashTable cannot
   contain values with undermined size and, threfore, increasing size.
   Ideally, the Value of the RHT should be a collection of undermined 
   size that can grow indefintely, but this is not possible.
   We should also consider using a [Node -> RoomyArray[Node]].  This 
   will guarrentee that Edges will not take up all available RAM.*/
  
  RoomyHashTable* graph; /* NodeType -> List[NodeType]
   You can think of this as the collection of all edges come out of 
   the Node as the Key.*/

} RoomyGraph;

/*
Constructs a RoomyGraph where each Node consists of bytesPerElt bytes
with a maximum outgoing edge count of maxEdges.
*/
RoomyGraph* RoomyGraph_makeBytes(char* name, uint64 bytesPerElt, uint64 maxEdges, uint64 initialCapacity);
