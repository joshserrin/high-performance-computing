#include <stdio.h>
#include <roomy.h>
#include <RoomyArray.h>

#define N 10
RoomyArray *x, *y;

void addElement(uint64 i, void *oldVal, void *newValOut);
void printElements(uint64 i, void *val);
void addToY(uint64 i, void *val);
void setInY(uint64 xi, void *arrVal, void *passedValue);
void setYi(uint64 i, void *oldVal, void *updateVal, void *newValOut);
 
void main(int argc, char **argv) {
	printf("---- start ----\n");
	Roomy_init(&argc, &argv);

	x =  RoomyArray_makeBytes("x", sizeof(uint64), N);

	// Populate the RA with values from 9999 to 0	
	RoomyArray_mapAndModify(x, addElement);
	RoomyArray_sync(x);
	//RoomyArray_map(x, printElements);

	y = RoomyArray_makeBytes("y", sizeof(uint64), N);
	RoomyArray_registerAccessFunc(x, setInY, sizeof(uint64));
	RoomyArray_registerUpdateFunc(y, setYi, sizeof(uint64));
	RoomyArray_map(x, addToY);
	RoomyArray_sync(x);
	RoomyArray_sync(y);

	printf("x:\n");
	RoomyArray_map(x, printElements);
	printf("y:\n");
	RoomyArray_map(y, printElements);
	printf("---- done ----\n");
}
void printElements(uint64 i, void *val) {
	printf("%lli = %lli\n", i, *(uint64 *)val);
}
// Set the RoomyArray's i_th element as N-i-1
void addElement(uint64 i, void* oldVal, void* newValOut) {
	*(uint64*)newValOut = N-i-1;
}
// val is x[i]
void addToY(uint64 i, void *val) {
	RoomyArray_access(x, *(uint64 *)val, &i, setInY);
}
// arrVal is x[x[i]] where...
// xi is x[i]
// passedValue is i
void setInY(uint64 xi, void *arrVal, void *passedValue) {
	RoomyArray_update(y, *(uint64 *)passedValue, arrVal, setYi);
}
void setYi(uint64 i, void *oldVal, void *updateVal, void *newValOut) {
	*(uint64 *)newValOut = *(uint64 *)updateVal;
}
