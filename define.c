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
