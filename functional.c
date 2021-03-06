#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functional.h"

static optt atoptt(const char* opt)
{
	if (!strcmp(opt, "+")) return ADD_OPT;
	else if (!strcmp(opt, "-")) return SUB_OPT;
	else if (!strcmp(opt, "*")) return MUL_OPT;
	else if (!strcmp(opt, "/")) return DIV_OPT;
	else if (!strcmp(opt, "(")) return LPAR_OPT;
	else if (!strcmp(opt, ")")) return RPAR_OPT;
	else return UNKNOWN_OPT;
}

static char* opttta(const optt opt)
{
	if (opt == ADD_OPT) return "+";
	else if (opt == SUB_OPT) return "-";
	else if (opt == MUL_OPT) return "*";
	else if (opt == DIV_OPT) return "/";
	else if (opt == LPAR_OPT) return "(";
	else if (opt == RPAR_OPT) return ")";
	else return "?";
}
static int priority(optt opt)
{
	if (opt == ADD_OPT) return 1;
	else if (opt == SUB_OPT) return 1;
	else if (opt == MUL_OPT) return 2;
	else if (opt == DIV_OPT) return 2;
	else if (opt == LPAR_OPT) return 3;
	else if (opt == RPAR_OPT) return 3;
	else return 0;
}
static node* new_node()
{
	node* ret = (node*) malloc(sizeof(node));
	ret->type = UNKNOWN_SYM;
	ret->left = NULL;
	ret->right = NULL;
	return ret;
}

static void set_opt(node* dest, const char* sym)
{
	dest->opt = atoptt(sym);
	if (dest->opt != UNKNOWN_OPT)
	{
		dest->type = OPT_SYM;
	}
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
		clen = strspn(source+head, ".0123456789");
		if (clen != 0)
		{
			strncpy(temp, source+head, clen);
			temp[clen]='\0';
			ntok->val = atof(temp);
			ntok->type = VAL_SYM;
			continue;
		}
		// function name node
		clen = strspn(source+head, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_012345679");
		if (clen != 0)
		{
			strncpy(temp, source+head, clen);
			temp[clen]='\0';
			// TODO: FUNCTION
			continue;
		}
		// else... operator node
		clen = 1;
		strncpy(temp, source+head, clen);
		temp[clen]='\0';

		set_opt(ntok, temp);
	}
	for (node** head = dest; (*head) != NULL; head = &(*head)->next)
	{
		if ((*head)->next != NULL && (*head)->next->next != NULL && (*head)->next->next != NULL)
		{
			node *a = (*head), *b = (*head)->next, *c = (*head)->next->next, *d = (*head)->next->next->next;
			if (a->type == OPT_SYM && a->opt == LPAR_OPT &&
				b->type == OPT_SYM && b->opt == SUB_OPT &&
				c->type == VAL_SYM &&
				d->type == OPT_SYM && d->opt == RPAR_OPT)
			{
				(*head)->type = VAL_SYM;
				(*head)->val = -1.0*c->val;
				(*head)->next = d->next;
				free(b);
				free(c);
				free(d);
			}
		}
	}
}

void view_token(node* source)
{
	for (node* head = source; head != NULL; head = head->next)
	{
		if (head->type == VAL_SYM)
		{
			if (head->val < 0) printf("(");
			if ((int) head->val == (float) head->val)
			{
				printf("%.0f", head->val);
			}
			else
			{
				printf("%.2f", head->val);
			}
			if (head->val < 0) printf(")");
		}
		else if (head->type == OPT_SYM)
		{
			printf("%s", opttta(head->opt));
		}
		else
		{
			printf("?");
		}

		if (head->next != NULL)
		{
			printf(", ");
		}
		else
		{
			printf("\n");
		}
	}
}

void delete_token(node** dest)
{
	if ((*dest) != NULL)
	{
		delete_token(&(*dest)->right);
		free((*dest));
		(*dest) = NULL;
	}
}

static void raise_error(const char* err_msg)
{
	printf("%s\n", err_msg);
	error_counter++;
}

void syntax_check(node* source)
{
	// reset error flags
	error_counter = 0;
	if (source == NULL)
	{
		raise_error("Empty expression string");
		return;
	}
	size_t pa_level = 0;
	node* prev_tok = NULL;
	node* curr_tok = source;
	// FOPT check
	if (curr_tok->type == OPT_SYM && curr_tok->opt != LPAR_OPT && curr_tok->opt != RPAR_OPT)
	{
		raise_error("First character an operator");
	}
	// UMPA check
	while(curr_tok != NULL)
	{
		if (curr_tok->type == UNKNOWN_SYM)
		{
			raise_error("Illegal character");
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
						raise_error("Identifier followed by a left parenthesis");
					}
					else if (prev_tok->type == OPT_SYM && prev_tok->opt == RPAR_OPT)
					{
						raise_error("Right parenthesis followed by a left parenthesis");
					}
				}
			}
			else if (curr_tok->opt == RPAR_OPT)
			{
				if (pa_level > 0)
				{
					pa_level--;
				}
				else
				{
					raise_error("Unmatched right parenthesis");
				}
				if (prev_tok != NULL)
				{
					if (prev_tok->type == OPT_SYM)
					{
						if (prev_tok->opt == LPAR_OPT)
						{
							raise_error("Left parenthesis followed by a right parenthesis");
						}
						else if (prev_tok->opt != RPAR_OPT)
						{
							raise_error("Operator followed by a right parenthesis");
						}
					}
				}
			}
			else if (prev_tok != NULL && prev_tok->type == OPT_SYM)
			{
				if (prev_tok->opt == LPAR_OPT)
				{
					raise_error("Left parenthesis followed by an operator");
				}
				else if (prev_tok->opt != RPAR_OPT)
				{
					raise_error("Operator followed by an operator");
				}
			}
		}
		else if (prev_tok != NULL && prev_tok->type == OPT_SYM && prev_tok->opt == RPAR_OPT)
		{
			raise_error("Right parenthesis followed by an identifier");
		}
		// update
		prev_tok = curr_tok;
		curr_tok = curr_tok->right;
	}
	if (pa_level != 0)
	{
		raise_error("Unmatched left parenthesis");
	}
	if (prev_tok->type == OPT_SYM && prev_tok->opt != RPAR_OPT && prev_tok->opt != LPAR_OPT)
	{
		raise_error("Last character an operator");
	}
}

static float simplest_calculate(const optt opt, const float val_1, const float val_2)
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
			raise_error("Divide By Zero");
			return 0;
		}
		else
		{
			return val_1 / val_2;
		}
	}
	else
	{
		raise_error("Undefined operator method");
		return 0;
	}
}


float calculate(node* source)
{
	if (source == NULL)
	{
		raise_error("Empty expression node");
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
			if (source->opt == LPAR_OPT)
			{
				return calculate(source->left);
			}
			else
			{
				return simplest_calculate(source->opt, calculate(source->left), calculate(source->right));
			}
		}
		else
		{
			raise_error("Expression contain unknown symbol");
			return 0;
		}
	}
}

static node** rightest_node(node** source)
{
	if ((*source) == NULL) return source;
	else if ((*source)->right == NULL) return source;
	else return rightest_node(&(*source)->right);
}

static void tree_remove_LPAR_OPT(node** dest)
{
	if ((*dest)->type == VAL_SYM)
	{
		return;
	}
	if ((*dest)->type == OPT_SYM)
	{
		if ((*dest)->opt != LPAR_OPT)
		{
			tree_remove_LPAR_OPT(&(*dest)->left);
			tree_remove_LPAR_OPT(&(*dest)->right);
		}
		else
		{
			node* temp = (*dest)->left;
			if (temp->type == VAL_SYM)
			{
				(*dest)->type = VAL_SYM;
				(*dest)->val = temp->val;
				(*dest)->left = NULL;
				free(temp);
			}
			else
			{
				(*dest)->type = OPT_SYM;
				(*dest)->opt = temp->opt;
				(*dest)->left = temp->left;
				(*dest)->right = temp->right;
				free(temp);
			}
		}
	}
}

void generate_tree(node** dest, node* source)
{
	size_t top = 0;
	node** stack[64];
	stack[0] = dest;
	for (node *head = source, *next_head = NULL; head != NULL; head = next_head)
	{
		next_head = head->next;

		head->left = NULL;
		head->right = NULL;
		if (head->type == VAL_SYM)
		{
			if (*stack[top] == NULL)
			{
				(*stack[top]) = head;
			}
			else
			{
				(*rightest_node(stack[top]))->right = head;
			}
		}
		else if (head->type == OPT_SYM)
		{
			if (head->opt == LPAR_OPT)
			{
				if ((*stack[top]) == NULL)
				{
					(*stack[top]) = head;
				}
				else
				{
					(*rightest_node(stack[top]))->right = head;
				}
				stack[top+1] = &(*rightest_node(stack[top]))->left;
				top++;
			}
			else if (head->opt == RPAR_OPT)
			{
				top--;
			}
			else
			{
				if ((*stack[top])->type == VAL_SYM)
				{
					head->left = (*stack[top]);
					(*stack[top]) = head;
				}
				else
				{
					if (priority((*stack[top])->opt) < priority(head->opt))
					{
						head->left = (*stack[top])->right;
						(*stack[top])->right = head;
					}
					else
					{
						head->left = (*stack[top]);
						(*stack[top]) = head;
					}
				}
			}
		}
		else
		{
			raise_error("Expression contain unknown symbol");
			return;
		}
	}
	tree_remove_LPAR_OPT(dest);
}

void delete_tree(node** dest)
{
	if ((*dest) != NULL)
	{
		delete_tree(&(*dest)->left);
		delete_tree(&(*dest)->right);
		free((*dest));
		(*dest) = NULL;
	}
}

void view_tree_postfix(node* source)
{
	if (source == NULL) return;
	view_tree_postfix(source->left);
	view_tree_postfix(source->right);
	if (source->type == VAL_SYM)
	{
		if (source->val < 0) printf("(");
		if ((int) source->val == (float) source->val)
			{
				printf("%.0f", source->val);
			}
			else
			{
				printf("%.2f", source->val);
			}
		if (source->val < 0) printf(")");
	}
	else if (source->type == OPT_SYM)
	{
		if (source->opt != LPAR_OPT)
		{
			printf("%s", opttta(source->opt));
		}
	}
	else
	{
		printf("?");
	}
}

