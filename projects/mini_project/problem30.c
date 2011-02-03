#include <stdio.h>
#include <RoomyArray.h>
#include <roomy.h>

#define N 10

RoomyArray *x, *y, *z;
// sets x[i]=N-i-1
void populateX(uint64 i, void *oldVal, void *newValOut);
// prints element i of any RoomyArray 
void printElements(uint64 i, void *val);
// Used to map over x, and update y[i]=x[i]
void addToY(uint64 i, void *val);
// set y[i]=*updateVal
void setYi(uint64 i, void *oldVal, void *updateVal, void *newValOut);
void setZ(uint64 yi, void *arrayVal, void *passedVal);
void computeZ(uint64 i, void *val);
void updateZi(uint64 i, void *oldVal, void *updateVal, void *newValOut); 

void main(int argc, char **argv) {
	printf("---- start ----\n");
	
	Roomy_init(&argc, &argv);

	x = RoomyArray_makeBytes("x", sizeof(uint64), N);
	RoomyArray_mapAndModify(x, populateX);
	RoomyArray_sync(x);
	//RoomyArray_map(x, printElements);

	y = RoomyArray_makeBytes("y", sizeof(uint64), N);
	RoomyArray_registerUpdateFunc(y, setYi, sizeof(uint64));
	RoomyArray_map(x, addToY);
	RoomyArray_sync(y);
	z = RoomyArray_makeBytes("z", sizeof(uint64), N);
	RoomyArray_registerAccessFunc(x, setZ, sizeof(uint64));
	RoomyArray_registerUpdateFunc(z, updateZi, sizeof(uint64));
	RoomyArray_map(y, computeZ);
	RoomyArray_sync(x);
	RoomyArray_sync(y);
	RoomyArray_sync(z);
	
	printf("x:\n");
	RoomyArray_map(x, printElements);
	printf("y:\n");
	RoomyArray_map(y, printElements);
	printf("z:\n");
	RoomyArray_map(z, printElements);

	printf("---- done ----\n");
}
void populateX(uint64 i, void *oldVal, void *newValOut) {
	*(uint64 *)newValOut = N-i-1;
}
void printElements(uint64 i, void *val) {
	printf("%lli = %lli\n", i, *(uint64 *)val);
}
void addToY(uint64 i, void *val) {
	// Add *val to y[i]
	RoomyArray_update(y, i, val, setYi);
}
void setYi(uint64 i, void *oldVal, void *updateVal, void *newValOut) {
	*(uint64 *)newValOut = *(uint64 *)updateVal;
}
void computeZ(uint64 i, void *val) {
	// Now that we have y[i] in val, we need to set z[i] as x[val].
	// We need to preserve the i in this case so we must use i
	// as the passed value.
	RoomyArray_access(x, *(uint64 *)val, &i, setZ);
}
// Accesses x[y[i]] where y[i] is yi.  Sets z[i] as x[y[i]].  Note, by
// performing this assignment we would lose [the original] i if we didn't
// pass it.  So, passedVal is i in y[i]
void setZ(uint64 yi, void *arrayVal, void *passedVal) {
	RoomyArray_update(z, *(uint64 *)passedVal, arrayVal, updateZi);
}
// Sets z[i] = x[y[i]] where updateVal is x[y[i]]
void updateZi(uint64 i, void *oldVal, void *updateVal, void *newValOut) {
	*(uint64 *)newValOut = *(uint64 *)updateVal;
}
