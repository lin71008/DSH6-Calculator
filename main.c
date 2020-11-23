#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functional.c"

int main(int argc, char const *argv[])
{
	char exp_str[128];
	node* exp_token = NULL;
	node* exp_tree = NULL;
	int result = 0;
	while(scanf("%s", exp_str) != EOF)
	{
		generate_token(&exp_token, exp_str);
		view_token(exp_token);
		syntax_check(exp_token);
		if (error_counter != 0)
		{
			delete_token(&exp_token);
			continue;
		}
		generate_tree(&exp_tree, exp_token);
		result = calculate(exp_tree);
		if (error_counter == 0)
		{
			view_tree_postfix(exp_tree);
			printf("=%d\n", result);
		}
		delete_tree(&exp_tree);
	}
	return 0;
}
