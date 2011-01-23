#include <stdio.h>

int my_method(int *arrB)
{
	printf("Address of arrB: %p\n", arrB);
}

int main(void)
{
	int arr[5] = {1,2,3,4,5};
	printf("Address of arr: %p\n", &arr);
	my_method(arr);
	return 0;
}
