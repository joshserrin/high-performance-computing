#include <stdio.h>

void *int_cpy(int *dest, int *src, int num)
{
	int *p = dest;
	int i = 0;
	while(i++ < num)
	{
		*p++ = *src++;
	}
	return dest;
}

int printInts(const int *arr, int n)
{
	int *temp = arr;
	int i = 0;
	while(i++ < n)
	{
		printf("%d,", *temp++);
	}
	printf("\n");
	return 0;
}
int main(void)
{
	int a[10] = {1,2,3,4,5};
	int b[10];
	int_cpy(b, a, 6);
	printInts(b, 6);
}
