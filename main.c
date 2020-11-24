#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Detect EOF, ESC, STDIO.redirect
#include <conio.h>
#include <unistd.h>
// expression analyze library
#include "functional.h"

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
	char exp_str[20+1];
	node* exp_token = NULL;
	node* exp_tree = NULL;
	float result;
	while (scanf("%20s", exp_str) != EOF)
	{
		char c = getchar();
		if (c != '\0' && c != '\n' && c != '\t' && c != ' ')
		{
			printf("%s\n", "Error -line  contains  more characters than allowed.");
			exp_token = NULL;
			exp_tree = NULL;
			continue;
		}
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
			if ((int) result == (float) result)
			{
				printf("\n=%.0f\n", result);
			}
			else
			{
				printf("\n=%.2f\n", result);
			}
		delete_tree(&exp_tree);
		if (continue_q() == 1) break;
	}
	return 0;
}
