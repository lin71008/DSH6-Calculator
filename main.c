#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functional.c"

int main(void)
{
	char str[] = "1+2*3(-4)";
	// scanf("%s", str);

	int top = 0;
	node* token;
	top = create_token(&token, str, 100);
	view_token(token, top);
	delete_token(&token, top);

	// int top = 0;
	// char** stack;
	// top = create_split(&stack, str, 100);
	// view_split(stack, top);
	// delete_split(&stack, top);

    return 0;
}
