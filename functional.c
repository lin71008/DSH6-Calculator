#include <stdlib.h>
#include <string.h>

#include "functional.h"

optt atoptt(const char* opt)
{
	if (!strcmp(opt, "+")) return ADD_OPT;
	else if (!strcmp(opt, "-")) return SUB_OPT;
	else if (!strcmp(opt, "*")) return MUL_OPT;
	else if (!strcmp(opt, "/")) return DIV_OPT;
	else if (!strcmp(opt, "(")) return LPAR_OPT;
	else if (!strcmp(opt, ")")) return RPAR_OPT;
	else return UNKNOWN_OPT;
}

char* opttta(const optt opt)
{
	if (opt == ADD_OPT) return "+";
	else if (opt == SUB_OPT) return "-";
	else if (opt == MUL_OPT) return "*";
	else if (opt == DIV_OPT) return "/";
	else if (opt == LPAR_OPT) return "(";
	else if (opt == RPAR_OPT) return ")";
	else return "?";
}

int opttcmp(const optt opt_1, const optt opt_2)
{
	return !!((int) opt_1 >= (int) opt_2);
}

node* new_node()
{
	node* ret = (node*) malloc(sizeof(node));
	ret->type = UNKNOWN_SYM;
	ret->left = NULL;
	ret->right = NULL;
	return ret;
}

node* rightest_node(node* source)
{
	if (source == NULL) return source;
	else if (source->right == NULL) return source;
	else return rightest_node(source->right);
}

void set_opt(node* dest, const char* sym)
{
	dest->opt = atoptt(sym);
	if (dest->opt != UNKNOWN_OPT)
	{
		dest->type = OPT_SYM;
	}
}

void set_val(node* dest, const char* sym)
{
	dest->val = atoi(sym);
	dest->type = VAL_SYM;
}

void generate_token(node** dest, const char* source)
{
	char temp[128];
	node* ntok = NULL;
	for (size_t clen, head = 0; head < strlen(source); head += clen)
	{
		// create new token node
		if (ntok == NULL)
		{
			ntok = new_node();
			(*dest) = ntok;
		}
		else
		{
			ntok->right = new_node();
			ntok = ntok->right;
		}

		// numerical node
		clen = strspn(source+head, "0123456789");
		if (clen != 0)
		{
			strncpy(temp, source+head, clen);
			temp[clen]='\0';
			set_val(ntok, temp);
			continue;
		}
		// function name node
		clen = strspn(source+head, "abcdefghijklmnopqrstuvwxyz_0123456790");
		if (clen != 0)
		{
			strncpy(temp, source+head, clen);
			temp[clen]='\0';
			set_opt(ntok, temp);
			continue;
		}
		// else... operator node
		clen = 1;
		strncpy(temp, source+head, clen);
		temp[clen]='\0';
		set_opt(ntok, temp);
	}
}

void view_token(node* source)
{
	node* curr_tok = source;
	while (curr_tok != NULL)
	{
		if (curr_tok->type == VAL_SYM) printf("%d, ", curr_tok->val);
		else if (curr_tok->type == OPT_SYM) printf("%s, ", opttta(curr_tok->opt));
		else printf("?, ");
		curr_tok = curr_tok->right;
	}
	printf("\n");
}

void delete_token(node** dest)
{
	if ((*dest) != NULL)
	{
		delete_token(&(*dest)->right);
		free((*dest));
	}
	(*dest) = NULL;
}

void raise_error(const char* err_msg)
{
	printf("%s\n", err_msg);
	error_counter++;
}

void syntax_check(node* source)
{
	// reset error flags
	error_counter = 0;

	size_t pa_level = 0;
	node* prev_tok = NULL;
	node* curr_tok = source;
	// FOPT check
	if (curr_tok->type == OPT_SYM && curr_tok->opt != LPAR_OPT)
	{
		raise_error("FOPT");
	}
	// UMPA check
	while(curr_tok != NULL)
	{
		if (curr_tok->type == UNKNOWN_SYM)
		{
			raise_error("UN");
		}
		else if (curr_tok->type == OPT_SYM)
		{
			if (curr_tok->opt == LPAR_OPT)
			{
				pa_level++;
				if (prev_tok != NULL)
				{
					if (prev_tok->type == VAL_SYM)
					{
						raise_error("MISMUL_L");
					}
					else if (prev_tok->type == OPT_SYM && prev_tok->opt == RPAR_OPT)
					{
						raise_error("MSIMUL");
					}
				}
			}
			else if (curr_tok->opt == RPAR_OPT)
			{
				if (pa_level > 0)
				{
					pa_level--;
					if (prev_tok != NULL)
					{
						if (prev_tok->type == OPT_SYM)
						{
							if (prev_tok->opt == LPAR_OPT)
							{
								raise_error("EMPTY");
							}
							else
							{
								raise_error("LOPT_R");
							}
						}
					}
				}
				else
				{
					raise_error("UMPA_R");
				}
			}
			else if (prev_tok != NULL && prev_tok->type == OPT_SYM)
			{
				if (prev_tok->opt == LPAR_OPT)
				{
					raise_error("FOPT_L");
				}
				else if (prev_tok->opt != RPAR_OPT)
				{
					raise_error("OPTOPT");
				}
			}
		}
		else if (prev_tok != NULL && prev_tok->type == OPT_SYM && prev_tok->opt == RPAR_OPT)
		{
			raise_error("MISMUL_R");
		}
		// update
		prev_tok = curr_tok;
		curr_tok = curr_tok->right;
	}
	if (pa_level != 0)
	{
		raise_error("UMPA_L");
	}
	if (prev_tok->type == OPT_SYM && prev_tok->opt != RPAR_OPT)
	{
		raise_error("LOPT");
	}
}

int simplest_calculate(const optt opt, const int val_1, const int val_2)
{
	if (opt == ADD_OPT)
	{
		return val_1 + val_2;
	}
	else if (opt == SUB_OPT)
	{
		return val_1 - val_2;
	}
	else if (opt == MUL_OPT)
	{
		return val_1 * val_2;
	}
	else if (opt == DIV_OPT)
	{
		if (val_2 == 0)
		{
			raise_error("Divide By Zero.");
			return 0;
		}
		else
		{
			return val_1 / val_2;
		}
	}
	else
	{
		raise_error("Undefine operator method.");
		return 0;
	}
}

int calculate(node* source)
{
	if (source == NULL)
	{
		raise_error("Empty expression node.");
		return 0;
	}
	else
	{
		if (source->type == VAL_SYM)
		{
			return source->val;
		}
		else if (source->type == OPT_SYM)
		{
			return simplest_calculate(source->opt, calculate(source->left), calculate(source->right));
		}
		else
		{
			raise_error("Expression contain unknown symbol.");
			return 0;
		}
	}
}

void delete_tree(node** dest)
{
	if ((*dest) != NULL)
	{
		if ((*dest)->left != NULL)
		{
			delete_tree(&(*dest)->left);
		}
		if ((*dest)->right != NULL)
		{
			delete_tree(&(*dest)->right);
		}
		free((*dest));
	}
	(*dest) = NULL;
}

void generate_tree(node** dest, node** source)
{
	size_t top = 0;
	node* stack[100];

	node *curr_head = (*source), *next_head = NULL;
	while (curr_head != NULL)
	{
		if (curr_head != NULL) next_head = curr_head->right;
		if (curr_head->type == VAL_SYM)
		{
			curr_head->left = NULL;
			curr_head->right = NULL;
			if ((*dest) == NULL)
			{
				curr_head->left = NULL;
				curr_head->right = NULL;
				(*dest) = curr_head;
			}
			else
			{
				rightest_node((*dest))->right = curr_head;
			}
		}
		if (curr_head->type == OPT_SYM)
		{
			if (curr_head->opt == LPAR_OPT)
			{
				stack[top+1] = stack[top]->right;
				top++;
			}
			else if (curr_head->opt == RPAR_OPT)
			{
				top--;
			}
			else
			{
				if ((*dest)->type == OPT_SYM)
				{
					if ((*dest)->opt < curr_head->opt)
					{
						curr_head->left = (*dest);
						curr_head->right = NULL;
						(*dest) = curr_head;
					}
					else
					{
						curr_head->left = (*dest);
						curr_head->right = NULL;
						(*dest) = curr_head;
					}
				}
				else
				{
					curr_head->left = (*dest);
					curr_head->right = NULL;
					(*dest) = curr_head;
				}
			}
		}
		curr_head = next_head;
	}
}
