#include <stdlib.h>
#include <string.h>
#include "functional.h"

size_t create_split(char*** dest, const char* source, const size_t max)
{
	size_t len = 0;
	(*dest) = (char**) malloc(max*sizeof(char*));
	for (size_t clen, head = 0; head < strlen(source); head += clen)
	{
		// numerical digit
		clen = strspn(source+head, "0123456798");
		if (clen != 0)
		{
			(*dest)[len] = (char*) malloc((clen+1)*sizeof(char));
			strncpy((*dest)[len], source+head, clen);
			(*dest)[len][clen] = '\0';
			len++;
			continue;
		}
		// function name
		clen = strspn(source+head, "abcdefghijklmnopwxyz_0123456790");
		if (clen != 0)
		{
			(*dest)[len] = (char*) malloc((clen+1)*sizeof(char));
			strncpy((*dest)[len], source+head, clen);
			(*dest)[len][clen] = '\0';
			len++;
			continue;
		}
		// else... operator
		clen = 1;
		(*dest)[len] = (char*) malloc((clen+1)*sizeof(char));
		strncpy((*dest)[len], source+head, clen);
		(*dest)[len][clen] = '\0';
		len++;
	}
	return len;
}

void view_split(char** source, const size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		printf("%s%c", source[i], ",\n"[i==len-1]);
	}
}

void delete_split(char*** dest, const size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		free((*dest)[i]);
	}
	free((*dest));
	(*dest) = NULL;
}

enum operator_type
{
	UNKNOWN_OPT,
	ADD_OPT,
	SUB_OPT,
	MUL_OPT,
	DIV_OPT,
	LPAR_OPT,
	RPAR_OPT
};

enum symbol_type
{
	UNKNOWN_SYM,
	OPT_SYM,
	VAL_SYM
};

struct node
{
	symt type;
	union
	{
		optt opt;
		int val;
	};
	node *left, *right;
};

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

node* new_node()
{
	node* ret = (node*) malloc(sizeof(node));
	ret->type = UNKNOWN_SYM;
	ret->left = NULL;
	ret->right = NULL;
	return ret;
}

void init_node(node* dest)
{
	dest->type = UNKNOWN_SYM;
	dest->left = NULL;
	dest->right = NULL;
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


size_t create_token(node** dest, const char* source, const size_t max)
{
	size_t len = 0;
	char temp[128];  // buffer
	(*dest) = (node*) malloc(max*sizeof(node));
	for (size_t clen, head = 0; head < strlen(source); head += clen)
	{
		// numerical digit
		clen = strspn(source+head, "0123456798");
		if (clen != 0)
		{
			init_node(&(*dest)[len]);
			strncpy(temp, source+head, clen);
			temp[clen]='\0';
			set_val(&(*dest)[len], temp);
			len++;
			continue;
		}
		// function name
		clen = strspn(source+head, "abcdefghijklmnopwxyz_0123456790");
		if (clen != 0)
		{
			init_node(&(*dest)[len]);
			strncpy(temp, source+head, clen);
			temp[clen]='\0';
			set_opt(&(*dest)[len], temp);
			len++;
			continue;
		}
		// else... operator
		clen = 1;
		init_node(&(*dest)[len]);
		strncpy(temp, source+head, clen);
		temp[clen]='\0';
		set_opt(&(*dest)[len], temp);
		len++;
	}
	return len;
}

void view_token(const node* source, const size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		if (source[i].type == VAL_SYM)
		{
			printf("%d,", source[i].val);
		}
		else if (source[i].type == OPT_SYM)
		{
			printf("%s,", opttta(source[i].opt));
		}
		else
		{
			printf("???,");
		}
	}
	printf("\n");
}

void delete_token(node** dest, const size_t len)
{
	free((*dest));
	(*dest) = NULL;
}
