#include <stdio.h>

void main(int argc, char *argv[]) {
	printf("First arg: %d\n", argc);
	int i;
	for(i = 0; i < argc; i++) {
		printf("  arg[%d]: ", i);
		puts(argv[i]);
	}
}
