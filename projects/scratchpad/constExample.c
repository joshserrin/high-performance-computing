#include <stdio.h>

char a[20] = "hello world";
char b[20];

char *my_strcpy(char *dest, const char *src)
{
	char *p = dest;
	while(*src != '\0')
	{
		*p++ = *src++;
	}
	*p = '\0';
	return dest;
}

int main(void)
{
	printf("location of a: %p\n", a);
	my_strcpy(b,a);
	puts(b);
	printf("location of a after copy: %p\n", a);
}
