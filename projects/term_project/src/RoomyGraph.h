#include <RoomyHashTable.h>
#include <roomy.h>

/* 
RoomyGraphs consist of Nodes and Edges linking two nodes.
Nodes are stores as uint64 to minimize space.  Considering, 
any given node can have many edges (up to maxEdges, to be
exact) we want to reduce the amount of unnecessary, redundant
space.  So allowing a Node to be of some type occupying more
than sizeof(uint64) bytes, would be a waste because there will
be many copies of it around.  To map your data structures to 
Nodes (which are basically ids) you could have a RoomyHashTable
containing this mapping.
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

// For documentation, see RoomyGraph.c
// =================== Basic RoomyGraph Functions ==============================
RoomyGraph* RoomyGraph_make(char* name, uint64 maxEdges, uint64 initialCapacity);
void RoomyGraph_addNodesFromDigraph(RoomyGraph *g, FILE *filepath);
void RoomyGraph_addEdgesFromDigraph(RoomyGraph *g, FILE *filepath);
void RoomyGraph_destroy(RoomyGraph *g);
void RoomyGraph_addNode(RoomyGraph *g, uint64 node);
void RoomyGraph_addNodes(RoomyGraph *g, uint64 nodes[], uint64 nodeCount);
void RoomyGraph_sync(RoomyGraph *g);
int RoomyGraph_containsNode(RoomyGraph *g, uint64 node);
void RoomyGraph_print(RoomyGraph *g);
uint64 RoomyGraph_nodeCount(RoomyGraph *g);
void RoomyGraph_addEdge(RoomyGraph *g, uint64 from, uint64 to);
void RoomyGraph_addEdges(RoomyGraph *g, uint64 from, uint64 *children, uint64 childCount);
int RoomyGraph_containsEdge(RoomyGraph *g, uint64 from, uint64 to);
// Currently not working... do not use!
typedef struct {
	uint64 count;
	uint64 *child;
} Children;
Children* RoomyGraph_getChildren(RoomyGraph *g, uint64 parent);

// ======================= Graph Algorithms ======================================
uint64 RoomyGraphAlg_degreeCentrality(RoomyGraph *g, uint64 node);
double RoomyGraphAlg_degreeCentralityStandardized(RoomyGraph *g, uint64 node);
uint64 RoomyGraphAlg_degreePrestige(RoomyGraph *g, uint64 node);
double RoomyGraphAlg_degreePrestigeStandardized(RoomyGraph *g, uint64 node);
void RoomyGraph_findCliques(RoomyGraph *g);
