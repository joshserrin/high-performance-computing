#include <stdio.h>

char* my_strchr(char *dest, char *str, char c)
{
	while(*str++ != '\0')
	{
		if(*str == c)
		{
			*dest = *str;
			return dest;
		}
	}
	return (char *)-1;
}

int main(void)
{
	char *str = "josh";
	char *d;
	my_strchr(d, str, 'h');
	puts(d);
	printf("Address of my h: %p\n", *(str + 3));
	printf("Address of ret h: %p\n", *d);
}
