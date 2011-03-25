#include <stdio.h>
#include "RoomyGraph.h"
#include <string.h>
#include <RoomyHashTable.h>

const uint64 RGTRUE = 1;
const uint64 RGFALSE = 0;
const uint64 NOTHING = -47;

// =============================================================================
// Use this function to traverse over all edges and execute a 
// function on each child
void forEachChild(uint64 parent, uint64 *edges, void (*f)(uint64 parent, uint64 child)) {
	uint64 size = edges[0];
	uint64 i;
	for(i = 0; i < size; i++) {
		f(parent, edges[i + 1]);
	}
}

// =============================================================================
char* uint64_to_string(char *buffer, uint64 i) {
  sprintf(buffer, "%lli", i);
  return buffer;
}

// ============================================================================
/*
 Destroys the RoomyGraph and frees up memory allocated for g. 
 */
void RoomyGraph_destroy(RoomyGraph *g) {
	RoomyHashTable_destroy(g->graph);
	free(g);
}

// ============================================================================
/* Adds the node to the RoomyGraph (RG).  
	 NOTE: This is a delayed operation therefore you must call RoomyGraph_sync before 
   the node is ensured to be added to the RG */
void RoomyGraph_addNode(RoomyGraph *g, uint64 node) {
	// We are adding 1 because that is what stores the stored
	// number of edges.
	void *EMPTY = calloc((1 + g->maxEdges), sizeof(uint64));
	RoomyHashTable_insert(g->graph, &node, EMPTY);
}
void RoomyGraph_addNodes(RoomyGraph *g, uint64 nodes[], uint64 nodeCount) {
	uint64 i;
	for(i = 0; i < nodeCount; i++) {
	  uint64 n = nodes[i];
		RoomyGraph_addNode(g, n);
	}
}

 // ============================================================================
/* Completes all delayed operations */
void RoomyGraph_sync(RoomyGraph *g) {
	RoomyHashTable_sync(g->graph);
}

// ============================================================================
uint64 count; // if you are going to use this, don't forget to initialize it!
uint64 searchNode;
// We don't care about the val (which is the edgelist, BTW)
void countMatches(void* key, void* val) {
	uint64 node = *(uint64 *)key;
	if(node == searchNode) { count++; }
}
/* Returns RGTRUE if the RoomyGraph contains a node equal to the provided node, or RGFALSE
   if no node can be found.
	 NOTE: It is recommended to ensure that the RoomyGraph has been sync'd before
	   calling this function. */
int RoomyGraph_containsNode(RoomyGraph *g, uint64 node) {
	searchNode = node;
	count = 0;
	RoomyHashTable_map(g->graph, countMatches);
	RoomyGraph_sync(g);
	if(count > 0) { return RGTRUE; }
	else { return RGFALSE; }
}

// =============================================================================
uint64 glo_from;
uint64 glo_to;
void increaseCountByMatchingEdges(uint64 parent, uint64 child) {
	if(child == glo_to) { count++; }
}
// Increases count if key is glo_from by the number of its edges
// equal to glo_to
void countEdges(void *key, void *value) {
	uint64 from = *(uint64 *)key;
	if(from == glo_from) {
		uint64 *edges = (uint64 *)value;
		forEachChild(from, edges, increaseCountByMatchingEdges);
	}
}

/* Returns 1 if an edge originating at from and terminating at to is contained
		within the RoomyGraph */
int RoomyGraph_containsEdge(RoomyGraph *g, uint64 from, uint64 to) {
	glo_from = from;
	glo_to = to;
	count = 0;
	RoomyHashTable_map(g->graph, countEdges);
	RoomyGraph_sync(g);
	if(count > 0) return RGTRUE;
	else return RGFALSE;
}

// =============================================================================
/* Returns the number of nodes in the RoomyGraph */
void increaseCount(void *k, void *v) { count++; }
uint64 RoomyGraph_nodeCount(RoomyGraph *g) {
	count = 0;
	RoomyHashTable_map(g->graph, increaseCount);
	RoomyGraph_sync(g);
	return count;
}

// =============================================================================
void printEdge(uint64 parent, uint64 child) {
	printf("%lli,", child);
}
void printNodeAndChildren(void *k, void *v) {
	uint64 parent = *(uint64 *)k;
	uint64 *edges = (uint64 *)v;
	printf("%lli -> [", parent);
	forEachChild(parent, edges, printEdge);
	printf("]\n");
}
/* Prints the contents of the RoomyGraph to the console */
void RoomyGraph_print(RoomyGraph *g) {
	RoomyHashTable_map(g->graph, printNodeAndChildren);
	RoomyGraph_sync(g);
}

// =============================================================================
typedef struct {
	uint64 newEdge; // the new edge to be added
	uint64 maxEdges; // the max number of edges that can leave a node
} AddEdgePassed;
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
/* Adds an directed edge originating at from and terminating at to.  It is
   assumed that the nodes have already been added.  There is no check to 
	 ensure this, though. 
	 NOTE: this is a delayed operation */
void RoomyGraph_addEdge(RoomyGraph *g, uint64 from, uint64 to) {
	AddEdgePassed arg;
	arg.newEdge = to;
	arg.maxEdges = g->maxEdges;
	RoomyHashTable_update(g->graph, &from, &arg, addEdge);
}
void RoomyGraph_addEdges(RoomyGraph *g, uint64 from, uint64 *children, uint64 childCount) {
	uint64 i = 0;
	for(i = 0; i < childCount; i++) {
		uint64 child = children[i];
		RoomyGraph_addEdge(g, from, child);
	}
}

// =============================================================================
Children *children;
void createChildren(void *key, void *value, void *passedVal) {
	printf("create Children started\n");
	uint64 *c = (uint64 *)value;
	printf("value is %p\n", value);
	printf("c is %p\n", c);
	uint64 num = c[0];
	printf("node has %lli number of edges\n", num);
	//printf("here1a: %p\n", children);
	//printf("here2: %d\n", num);
	//children->count = num;
	//printf("here3\n");
	//memcpy(children->child, c, num*sizeof(uint64));
}
Children* RoomyGraph_getChildren(RoomyGraph *g, uint64 parent) {
	//children = NULL;
	printf("Getting children of %lli\n", parent);
	RoomyHashTable_access(g->graph, &parent, &g->maxEdges, &createChildren);
	printf("access called\n");
	RoomyGraph_sync(g);
	printf("Got children of %lli\n", parent);
	//return children;
	return NULL;
}

// =============================================================================
//                       Graph Algorithms
// =============================================================================

// Helper functions ============================================================
double standardize(RoomyGraph *g, uint64 node, uint64 (*f)(RoomyGraph*, uint64)) {
  uint64 d = f(g, node);
  uint64 nodeCount = RoomyGraph_nodeCount(g);
  return ((double) d) / ((double)(nodeCount - 1));
}
// =============================================================================

uint64 degreeCentralityOfNode;
void computeDegreeCentrality(void *key, void *value, void *DOESNOTMATTER) {
	uint64 *edgeList = (uint64 *)value;
	uint64 count = edgeList[0];
	degreeCentralityOfNode = count;
}
/*
The Degree Centrality of a node is defined as:
C_d(n_i) = Xi+

where, 
n_i is the node
Xi+ is the number of edges originating at n_i

*source: Social Network Analysis: Methods and Applications by Stanley Wasserman and Katherine Faust
*/
uint64 RoomyGraphAlg_degreeCentrality(RoomyGraph *g, uint64 node) {
	RoomyHashTable_access(g->graph, &node, &NOTHING, &computeDegreeCentrality);
	RoomyGraph_sync(g);
	return degreeCentralityOfNode;
}
/*
C'_d(n_i) = C_d(n_i) / (g-1)
where g is the number of nodes in graph g
*/
double RoomyGraphAlg_degreeCentralityStandardized(RoomyGraph *g, uint64 node) {
  return standardize(g, node, RoomyGraphAlg_degreeCentrality);
}

// =============================================================================
/*
Degree Prestige is the number of indegree edges
*/
uint64 prestigeNode;
uint64 prestigeCount;
void increaseCountIfPrestigeNode(uint64 parent, uint64 child) {
  if(child == prestigeNode) { prestigeCount++; }
}
void computePrestige(void *key, void *value) {
  uint64 parent = *(uint64 *)key;
  uint64 *edges = (uint64 *)value;
  forEachChild(parent, edges, increaseCountIfPrestigeNode);
}
uint64 RoomyGraphAlg_degreePrestige(RoomyGraph *g, uint64 node) {
  prestigeNode = node;
  prestigeCount = 0;
  RoomyHashTable_map(g->graph, computePrestige);
  RoomyGraph_sync(g);
  return prestigeCount;
}
double RoomyGraphAlg_degreePrestigeStandardized(RoomyGraph *g, uint64 node) {
  return standardize(g, node, RoomyGraphAlg_degreePrestige);
}

// =============================================================================

// All of the current cliques.  This is basically an array of RoomyList*
uint64 nextCliqueIndex;
RoomyList **cliques;

void printListElement(void *val) {
  uint64 i = *(uint64 *)val;
  printf("%lli, ", i);
}
void RoomyList_print(RoomyList *rl) {
  RoomyList_map(rl, printListElement);
}
void printCliques() { 
  int k;
  for(k = 0; k < nextCliqueIndex; k++) {
    RoomyList *rl = cliques[k];
    printf("Clique [%i]: ", k);
    RoomyList_print(rl);
    printf("\n");
  }
}

void makeInitialCliques(void *key, void *val) {
  uint64 node = *(uint64 *)key;
  char buffer[10]; // assume nodes are from 0 to 1bil
  RoomyList *clique = RoomyList_make(uint64_to_string(buffer, node), sizeof(uint64));
  RoomyList_add(clique, &node);
  RoomyList_sync(clique);
  cliques[nextCliqueIndex] = clique;
  nextCliqueIndex++;
}
typedef struct {
  int answer;
  RoomyGraph *g;
  RoomyList *b;
} CanMergeArgs;
typedef struct {
  int answer;
  uint64 node;
  RoomyGraph *g;
} CanMergeArgs2;
void mergeValAns2(void *ansInOut, void *val) {
  CanMergeArgs2 args = *(CanMergeArgs2 *)ansInOut;
  
  if(args.answer == RGTRUE) {
    // parent has been connected to all previous children so lets see if it is 
    // connected to this child.
    RoomyGraph *g = args.g;
    uint64 parent = args.node;
    uint64 child = *(uint64 *)val;
    args.answer = RoomyGraph_containsEdge(g, parent, child);
  } else {
    // We've already determined that parent was not connected to a previous node
    // so there is no point to determine if child is connected to parent.
    args.answer = RGFALSE;
  }
}
void mergeAnsAns2(void *ansInOut, void *ansIn) {
  CanMergeArgs2 a = *(CanMergeArgs2 *)ansInOut;
  CanMergeArgs2 b = *(CanMergeArgs2 *)ansIn;
  if(a.answer == RGFALSE || b.answer == RGFALSE) { a.answer = RGFALSE; }
  else { a.answer = RGTRUE; }
}
void mergeValAns(void *ansInOut, void *val) {
  printf("mergeValAns\n");
  CanMergeArgs args = *(CanMergeArgs *)ansInOut;
  
  if(args.answer == RGTRUE) {
    // All previous nodes in A have been connected to all nodes in B, so determine
    // if this node is connected to all nodes in B
    uint64 node = *(uint64 *)val;
    RoomyList *b = args.b;  
    CanMergeArgs2 args2;
    args2.answer = args.answer;
    args2.node = node;
    args2.g = args.g;
    // see if all nodes in b are attached to node
    RoomyList_reduce(b, &args2, sizeof(CanMergeArgs2), mergeValAns2, mergeAnsAns2);
    
    if(args2.answer == RGTRUE) { args.answer = RGTRUE; } // node is connected to all nodes in B
    else { args.answer = RGFALSE; } // node is NOT connected to all nodes in B
  } else { args.answer = RGFALSE; }
}
void mergeAnsAns(void *ansInOut, void *ansIn) {
  printf("mergeAnsAns\n");
  CanMergeArgs a = *(CanMergeArgs *)ansInOut;
  CanMergeArgs b = *(CanMergeArgs *)ansIn;
  if(a.answer == RGFALSE || b.answer == RGFALSE) { a.answer = RGFALSE; }
  else { a.answer = RGTRUE; }
}
int canMergeCliques(RoomyGraph *g, RoomyList *a, RoomyList *b) {
  CanMergeArgs args;
  args.answer = RGTRUE;
  args.g = g;
  args.b = b;
  
  // We can merge the cliques if all nodes in a are connected to all nodes in b
  RoomyList_reduce(a, &args, sizeof(CanMergeArgs), mergeValAns, mergeAnsAns);
}
void mergeCliques(RoomyList *a, RoomyList *b) {
  // Add all nodes from b into a
  // empty out b
  RoomyList_addAll(a, b);
  RoomyList_removeAll(b, b);
  RoomyList_sync(a);
  RoomyList_sync(b);
}
void RoomyGraph_findCliques(RoomyGraph *g) {
  nextCliqueIndex = 0;
  uint64 nodeCount = RoomyGraph_nodeCount(g);
  cliques = calloc(nodeCount, sizeof(RoomyList));
  
  // make the cliques of k=1
  RoomyHashTable_map(g->graph, makeInitialCliques);
  
  printCliques();
  
  // Right now this is only a single iteration
  uint64 cliqueCount = nextCliqueIndex;
  int i, j;
  for(i = 0; i < cliqueCount; i++) {
    RoomyList *a = cliques[i];
    for(j = i+1; j < cliqueCount; j++) {
      RoomyList *b = cliques[j];
      if(canMergeCliques(g, a, b) == RGTRUE) {
        printf("clique[%i] and clique[%i] can be merged", i, j);
        mergeCliques(a, b);
      }
    }
  }
  printf("Iteration finished\n");
  printCliques();
}

// ================ End Graph Algorithms =======================================

/*
Constructs a RoomyGraph where each Node consists of bytesPerElt bytes
with a maximum outgoing edge count of maxEdges.
*/
RoomyGraph* RoomyGraph_make(char* name, uint64 maxEdges, 
                                 uint64 initialCapacity) {
  RoomyGraph* g = (RoomyGraph *)malloc(sizeof(RoomyGraph));

  uint64 keySize = sizeof(uint64);
  uint64 valueSize = (maxEdges + 1)*keySize;

  g->graph = RoomyHashTable_make(name, keySize, valueSize, initialCapacity);
  g->maxEdges = maxEdges;

	// We must also attach the functions used for searching
	RoomyHashTable_registerUpdateFunc(g->graph, addEdge, sizeof(AddEdgePassed));
	RoomyHashTable_registerAccessFunc(g->graph, createChildren, sizeof(uint64));
	RoomyHashTable_registerAccessFunc(g->graph, computeDegreeCentrality, sizeof(uint64)); // NOTHING passed

  return g;
}

