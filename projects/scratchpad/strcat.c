#include <stdio.h>

char* my_strcat(char *dest, char *a, char *b)
{
	char *temp = dest;
	while(*a != '\0')
	{
		*temp++ = *a++;
	}
	while(*b != '\0')
	{
		*temp++ = *b++;
	}
	*temp = '\0';
	return dest;
}

int main(void)
{
	char a[10] = "hello ";
	char b[10] = "world";
	char d[20];
	my_strcat(d, a, b);
	puts(d);
}
