#include <stdio.h>

int main()
{
	int i, k;
	i = 1;
	k = 2;
	printf("i = %d  iaddr = %d\n", i, &i);
	printf("k = %d  kaddr = %d\n", k, &k);
	i = k;
	printf("i = %d  iaddr = %d\n", i, &i);
	printf("k = %d  kaddr = %d\n", k, &k);
	i = &k;
	printf("i = %d  iaddr = %d\n", i, &i);
}
