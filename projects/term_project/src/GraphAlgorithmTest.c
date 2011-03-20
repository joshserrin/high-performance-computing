#include <stdio.h>
#include "RoomyGraph.h"
#include <assert.h>

#define PASSED 1 
#define FAILED 0
#define n1 1
#define n2 2
#define n3 3
#define n4 4

int main(int argc, char **argv) {
	Roomy_init(&argc, &argv);

	if(
		starGraphDegreeCentrality()
		&& circularDegreeCentrality()
		&& starGraphDegreePrestige()
		) {
		printf("All tests passed!\n");
	} else {
		printf("A test has failed check logs....\n");
	}

	Roomy_finalize();
}
RoomyGraph* createBasicGraph(char *name) {
  // makes a graph with no edges.  don't forget to call sync!
	RoomyGraph *g = RoomyGraph_make(name, 3, 4);
	RoomyGraph_addNode(g, n1);
	RoomyGraph_addNode(g, n2);
	RoomyGraph_addNode(g, n3);
	RoomyGraph_addNode(g, n4);
	return g;
}
RoomyGraph* createStarGraph() {
  RoomyGraph *g = createBasicGraph("star-graph");
	RoomyGraph_addEdge(g, n1, n2);
	RoomyGraph_addEdge(g, n1, n3);
	RoomyGraph_addEdge(g, n1, n4);
	RoomyGraph_sync(g);
	return g;
}
RoomyGraph* createCircularGraph() {
  RoomyGraph *g = createBasicGraph("circular-graph");
  RoomyGraph_addEdge(g, n1, n2);
  RoomyGraph_addEdge(g, n2, n3);
  RoomyGraph_addEdge(g, n3, n4);
  RoomyGraph_addEdge(g, n4, n1);
  RoomyGraph_sync(g);
  return g;
}
void assertFunc(RoomyGraph *g, uint64 node, uint64 expected, uint64 (*f)(RoomyGraph*, uint64)) {
  uint64 actual = f(g, node);
  assert(actual == expected);
}
void assertStandFunc(RoomyGraph *g, uint64 node, double expected, double (*f)(RoomyGraph*, uint64)) {
  double actual = f(g, node);
  assert(actual == expected);
}
void assertCentrality(RoomyGraph *g, uint64 node, uint64 expected) {
  assertFunc(g, node, expected, RoomyGraphAlg_degreeCentrality);
}
void assertPrestige(RoomyGraph *g, uint64 node, uint64 expected) {
  assertFunc(g, node, expected, RoomyGraphAlg_degreePrestige);
}
void assertCentralityStand(RoomyGraph *g, uint64 node, double expected) {
  assertStandFunc(g, node, expected, RoomyGraphAlg_degreeCentralityStandardized);
}
void assertPrestigeStand(RoomyGraph *g, uint64 node, double expected) {
  assertStandFunc(g, node, expected, RoomyGraphAlg_degreePrestigeStandardized);
}
int starGraphDegreeCentrality() {
	RoomyGraph *g = createStarGraph();
	assertCentrality(g, n1, 3);
	assertCentralityStand(g, n1, (3.0 / (4.0-1.0)));
	assertCentrality(g, n2, 0);
	assertCentrality(g, n3, 0);
	
	printf("startGraphDegreeCentrality done\n");
	return PASSED; 
}
int circularDegreeCentrality() {
  RoomyGraph *g = createCircularGraph();
  assertCentrality(g, n1, 1);
  assertCentrality(g, n2, 1);
  assertCentralityStand(g, n1, (1.0 / (4.0-1.0)));
  printf("circularDegreeCentrality done\n");
  return PASSED;
}
int starGraphDegreePrestige() {
  RoomyGraph *g = createStarGraph();
  assertPrestige(g, n1, 0);
  assertPrestige(g, n2, 1);
  assertPrestigeStand(g, n1, 0);
  assertPrestigeStand(g, n2, (1.0 / (4.0-1.0)));
  printf("starGraphDegreePrestige done\n");
  return PASSED;
}
