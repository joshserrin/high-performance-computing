#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COUNT 10000

int min(int *arr);
int max(int *arr);
int greater(const int a, const int b);
int lesser(const int a, const int b);
int fold(int *arr, int initial,	int (*compare)(const int, const in));

int main(void) {
	printf("\n\n");
	srand( time(NULL) );

	int x[COUNT];
	int y[COUNT];
	int i;
	for(i = 0; i < COUNT; i++)
	{
		x[i] = nextInt();
	}

	printf("min=%d\n", min(x));
	printf("max=%d\n", max(x));

	printf("\n\n");
}

int nextInt() {
	return rand() % COUNT + 1;
}
int min(int *arr) {
	int min = fold(arr, INFINITY, lesser);
	return min;
}
int greater(const int a, const int b) {
	if(a > b) { return a; }
	else return b;
}
int max(int *arr) {
	int max = fold(arr, -INFINITY, greater);
	return max;
}
int lesser(const int a, const int b) {
	if(a < b) { return a; }
	else return b;
}
// arr = the array
// initial = the initial value to compare to
// compare = Take two arguments, the first is the current answer (i.e. total)
// 		and the second is the current value of the array
// return the integer value that satisfies the function compared to all
// 	other elements in the array
int fold(int *arr, int initial,	int (*compare)(const int, const in)) {
	int current = initial;
	int i;
	for(i=0; i<COUNT; i++) {
		current = compare(current, arr[i]);
	}
	return current;
}
