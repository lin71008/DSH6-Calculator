#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functional.c"

#include <conio.h>
#include <unistd.h>

int continue_q()
{
	if (isatty(STDIN_FILENO))
	{
		char c;
		printf("Continue? [Y/n]");
		c = getch();
		if (c == 27 || c == 78 || c == 110 || c == -1)  // ESC, N, n, EOF
		{
			return 1;
		}
		else
		{
			printf("\n");
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int main(int argc, char const *argv[])
{
	char c, exp_str[128];
	node* exp_token = NULL;
	node* exp_tree = NULL;
	float result = 0;
	do
	{
		printf("> ");
		exp_str[0] = '\0';
		scanf("%s", exp_str);
		if (strlen(exp_str) == 0) break;  // EOF
		generate_token(&exp_token, exp_str);
		// view_token(exp_token);
		syntax_check(exp_token);
		if (error_counter != 0)
		{
			delete_token(&exp_token);
			if (continue_q() == 1) break;
			else continue;
		}
		generate_tree(&exp_tree, exp_token);
		result = calculate(exp_tree);
		if (error_counter != 0)
		{
			delete_tree(&exp_tree);
			if (continue_q() == 1) break;
			else continue;
		}
			printf("The postfix expression: ");
			view_tree_postfix(exp_tree);
			printf("\n");
			if ((int) result == (float) result)
			{
				printf("= %.0f\n", result);
			}
			else
			{
				printf("= %.2f\n", result);
			}
		delete_tree(&exp_tree);
		if (continue_q() == 1) break;
	} while (1);
	return 0;
}
