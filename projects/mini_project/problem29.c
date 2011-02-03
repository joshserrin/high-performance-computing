#include <stdio.h>
#include <roomy.h>
#include <RoomyArray.h>
#include <RoomyList.h>

RoomyArray *x;
RoomyList *y;

void addElement(uint64 i, void* oldVal, void* newValOut) {
	*(uint64*)newValOut = 9999-i;
}
void addToY(uint64 i, void *arrVal, void *passedVal) {
	// arrVal needs to be added to y
	RoomyList_add(y, arrVal);
}
void getXsubXi(uint64 i, void *val) {
	// Add x[val] to y
	// val is already x[i]
	RoomyArray_access(x, *(uint64 *)val, NULL, addToY);
	
}
void main(int argc, char **argv) {
	Roomy_init(&argc, &argv);

	uint64 numElts = 10000;
	x =  RoomyArray_makeBytes("x", sizeof(uint64), numElts);

	// Populate the RA with values from 9999 to 0	
	RoomyArray_mapAndModify(x, addElement);

	y = RoomyList_make("y", sizeof(uint64));

	// Populate the RL with x[x[i]]
	// 0 indicated NULL is passed
	RoomyArray_registerAccessFunc(x, addToY, 0);
	RoomyArray_map(x, getXsubXi);

	RoomyArray_sync(x);
	RoomyList_sync(y);

	printf("Num elts in x: %llo\n", RoomyArray_size(x));
	printf("Num elts in y: %llo\n", RoomyList_size(y));
	printf("\n");
}
