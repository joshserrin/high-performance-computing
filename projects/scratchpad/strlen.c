#include <stdio.h>


int my_strlen(char *str)
{
	int i = 0;
	while(*str++ != '\0')
	{
		i++;
	}	
	return i;
}

int main(void)
{
	char *str = "1234567";
	int len = my_strlen(str);
	printf("Length is %d\n", len);
}
