#include <stdio.h>
#include "RoomyGraph.h"
#include <assert.h>

#define TRUE 1
#define FALSE 0
#define PASSED 1
#define FAILED 0

int main(int argc, char ** argv) {
  Roomy_init(&argc,&argv);
  
	if(createGraph() &&
		 destroyGraph() && 
		 addNode() &&
		 containsNodeTest() &&
		 addMultipleNodes() &&
		 ableToAddMoreThanInitialSize() &&
		 addingSameNodeDoesntIncreaseCount() &&
		 addEdgeTest() &&
		 loadFromDigraph() 
		 && loadFromFigure71()
		 && loadFromLargeDigraph()
     && runSimpleCliqueFinding() 
//		&& getChildren())
			)
		{
    printf("All tests passed\n");
  } else {
    printf("ERROR OCCURRED... SEE LOGS\n");
  }
	
	Roomy_finalize();
}
// This graph is taken from 'Social Network Analysis: Methods and Application'
// by Wasserman et al page 255
RoomyGraph* createFig71() {
	RoomyGraph *g = RoomyGraph_make("fig 7.1", 5, 7);
	
	FILE *fp = fopen("./datasets/fig71.dot", "r");
	RoomyGraph_addNodesFromDigraph(g, fp);
	fclose(fp);
	FILE *fp2 = fopen("./datasets/fig71.dot", "r");
	RoomyGraph_addEdgesFromDigraph(g, fp2);
	fclose(fp2);
	return g;
}
int loadFromFigure71() {
  //printf("loadFromFigure71 started...\n");
  RoomyGraph *g = createFig71();
  //RoomyGraph_print(g);
  RoomyGraph_destroy(g);
  printf("loadFromFigure71 completed successfully\n");
  return PASSED;
}
int runSimpleCliqueFinding() {
  //printf("runSimpleCliqueFinding start...\n");
	RoomyGraph *g = createFig71();
	//printf("The graph...\n");
	//RoomyGraph_print(g);

	/*
		The cliques for this graph are
		[1,2,3], [1,3,5] and [3,4,5,6]
	 */
	RoomyGraph_findCliques(g);

	printf("runSimpleCliqueFinding completed successfully\n");
	return PASSED;
}
/*
	This is a VERY long test and will take a VERY long time.  Be careful!
*/
int loadFromLargeDigraph() {
	uint64 maxEdges = 12710;
	uint64 initialCapacity = 999;
	RoomyGraph *g = RoomyGraph_make("large-do", maxEdges, initialCapacity);
	
	printf("Loading Wiki-Vote.  This may take a long time (~5min)\n");
	printf("Started %ld\n", time(NULL));
	FILE *fp = fopen("./datasets/Wiki-Vote.dot", "r");
	RoomyGraph_addNodesFromDigraph(g, fp);
	fclose(fp);
	FILE *fp2 = fopen("./datasets/Wiki-Vote.dot", "r");
	RoomyGraph_addEdgesFromDigraph(g, fp2);
	fclose(fp2);
	printf("Ended %ld\n", time(NULL));

	printf("loadFromLargeDigraph() completed successfully.\n");
	return PASSED;
}
int loadFromDigraph() {
	uint64 maxEdges = 2;
	uint64 initialCapacity = 3;
	RoomyGraph *g = RoomyGraph_make("wiki-vote", maxEdges, initialCapacity);	

	FILE *fp = fopen("./datasets/simple.dot", "r");
	RoomyGraph_addNodesFromDigraph(g, fp);
	fclose(fp);
	FILE *fp2 = fopen("./datasets/simple.dot", "r");
	RoomyGraph_addEdgesFromDigraph(g, fp2);
	fclose(fp2);
	assert(TRUE == RoomyGraph_containsNode(g, 10));
	assert(TRUE == RoomyGraph_containsNode(g, 2));
	assert(TRUE == RoomyGraph_containsNode(g, 3));
	assert(TRUE == RoomyGraph_containsEdge(g, 10, 2));
	assert(TRUE == RoomyGraph_containsEdge(g, 10, 3));
	assert(TRUE == RoomyGraph_containsEdge(g, 2, 3));
	printf("loadFromDigraph completed successfully.\n");
	return PASSED;
}
int getChildren() {
	RoomyGraph *g = RoomyGraph_make("getChildren", 4, 4);
	RoomyGraph_addNode(g, 1);
	RoomyGraph_addNode(g, 2);
	RoomyGraph_addNode(g, 3);
	RoomyGraph_addNode(g, 4);
	RoomyGraph_addEdge(g, 1, 2);
	RoomyGraph_addEdge(g, 1, 3);
	RoomyGraph_addEdge(g, 2, 4);
	RoomyGraph_sync(g);

	printf("graph created\n");

	Children *children = RoomyGraph_getChildren(g, 1);
	assert(children->count == 2);
	assert(children->child[0] == 2);
	assert(children->child[1] == 3);

	children = RoomyGraph_getChildren(g, 2);
	assert(children->count == 1);
	assert(children->child[0] == 4);

	children = RoomyGraph_getChildren(g, 3);
	assert(children->count == 0);

	RoomyGraph_destroy(g);
	printf("getChildren completed successfully.\n");
	return PASSED;
}
int addEdgeTest() {
	RoomyGraph *g = RoomyGraph_make("addEdgeTest", 2, 2);
	uint64 from = 2;
	uint64 to = 4;
	RoomyGraph_addNode(g, from);
	RoomyGraph_addNode(g, to);
	RoomyGraph_addEdge(g, from, to);
	RoomyGraph_sync(g);

//	RoomyGraph_print(g);

	assert(TRUE == RoomyGraph_containsEdge(g, from, to));
	assert(FALSE == RoomyGraph_containsEdge(g, to, from));

	RoomyGraph_destroy(g);
	printf("addEdgeTest completed successfully.\n");
	return PASSED;
}
int addingSameNodeDoesntIncreaseCount() {
	RoomyGraph *g = RoomyGraph_make("multiAdds", 2, 2);
	uint64 node = 653;
	RoomyGraph_addNode(g, node);
	RoomyGraph_sync(g);
	assert(1 == RoomyGraph_nodeCount(g));
	
	RoomyGraph_addNode(g, node);
	RoomyGraph_sync(g);
	assert(1 == RoomyGraph_nodeCount(g));

	uint64 node2 = 1127;
	RoomyGraph_addNode(g, node2);
	RoomyGraph_sync(g);
	assert(2 == RoomyGraph_nodeCount(g));
	
	RoomyGraph_destroy(g);

	printf("addingSameNodeDoesntIncreaseCount completed successfully.\n");
	return PASSED;
}
int ableToAddMoreThanInitialSize() {
	RoomyGraph *g = RoomyGraph_make("increaseCapacity", 2, 2);
	uint64 count = 0;
	while(count < 4) {
		count++;
		RoomyGraph_addNode(g, count);	
	}
	RoomyGraph_sync(g);
	assert(4 == RoomyGraph_nodeCount(g));
	//printf("Contents of RoomyGraph that has been expanded from size 2 to 4...\n");
	//RoomyGraph_print(g);
	RoomyGraph_destroy(g);
	printf("ableToAddMoreThanInitialSize completed successfully.\n");
	return PASSED;
}
// Failing functions should return 1, otherwise 0.
int createGraph() {
  uint64 maxEdges = 10;
  uint64 initialCapacity = 10;
	RoomyGraph *g = RoomyGraph_make("g", maxEdges, initialCapacity);

  assert(NULL != g);
  assert(g->maxEdges == 10);
  assert(NULL != g->graph);

	printf("createGraph completed successfully.\n");
	RoomyGraph_destroy(g);
  return PASSED;
}
int destroyGraph() {
	RoomyGraph *g = RoomyGraph_make("destroy", 1, 1);
	assert(NULL != g);
	RoomyGraph_destroy(g);
	printf("destroyGraph completed successfully.\n");
	return PASSED;
}
int addNode() {
	// Create a very simple RG so we can add a single node to it.
	RoomyGraph *g = RoomyGraph_make("addNode", 2, 2);
	uint64 node = 3;
	RoomyGraph_addNode(g, node);
	RoomyGraph_sync(g);
	assert(TRUE == RoomyGraph_containsNode(g, node));
	RoomyGraph_destroy(g);
	printf("addNode completed successfully.\n");
	return PASSED;
}
int containsNodeTest() {
	RoomyGraph *g = RoomyGraph_make("containsNodeTest", 2, 2);
	uint64 node = 3;
	RoomyGraph_addNode(g, node);
	RoomyGraph_sync(g);
	uint64 notIn = 4;
	assert(TRUE == RoomyGraph_containsNode(g, node));
	assert(FALSE == RoomyGraph_containsNode(g, notIn));
	RoomyGraph_destroy(g);
	printf("containsNodeTest completed successfully.\n");
	return PASSED;
}
int addMultipleNodes() {
	RoomyGraph *g = RoomyGraph_make("multiNode", 2, 4);
	uint64 first = 1;
	uint64 second = 2;
	RoomyGraph_addNode(g, first);
	RoomyGraph_addNode(g, second);
	RoomyGraph_sync(g);
	
	assert(TRUE == RoomyGraph_containsNode(g, first));
	assert(TRUE == RoomyGraph_containsNode(g, second));

	RoomyGraph_destroy(g);
	printf("addMultipleNodes completed successfully.\n");
	return PASSED;
}

