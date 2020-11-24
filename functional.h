#ifndef __FUNCTIONAL_H__
#define __FUNCTIONAL_H__

#include <stdlib.h>
#include <string.h>

/**************************************************************
enum operator_type:
	UNKNOWN_OPT: ?
	ADD_OPT: +
	SUB_OPT: -
	MUL_OPT: *
	DIV_OPT: /
	LPAR_OPT: (
	RPAR_OPT: )
**************************************************************/
typedef enum operator_type
{
	UNKNOWN_OPT,
	ADD_OPT,
	SUB_OPT,
	MUL_OPT,
	DIV_OPT,
	LPAR_OPT,
	RPAR_OPT
} optt;

/**************************************************************
enum symbol_type:
	UNKNOWN_SYM: unknown
	OPT_SYM: operator
	VAL_SYM: operand
**************************************************************/
typedef enum symbol_type
{
	UNKNOWN_SYM,
	OPT_SYM,
	VAL_SYM
} symt;

/**************************************************************
struct node:
	@type: enum symbol_type,
		   labeled node type is either operator, numerical or
		   unknown.
		   DEFAULT VALUE: UNKNOWN (UNKNOWN_SYM)
	union:
		   @opt: enum operator_type
		   @val: 32-bit floating point number
	next:
		  @left (previous): a pointer to structure node,
		  					DEFAULT VALUE: EMPTY (NULL)
		  @right (next): a pointer to structure node,
		  				 DEFAULT VALUE: EMPTY (NULL)
**************************************************************/
typedef struct node
{
	symt type;
	union
	{
		optt opt;
		float val;
	};
	union
	{
		struct node *left;
		struct node *prev;
	};
	union
	{
		struct node *right;
		struct node *next;
	};
} node;

int error_counter;

/**************************************************************
generate_token (
	@destination: a pointer to a pointer to structure node,
				  a pointer to expression token list,
				  DEFAULT VALUE MUST BE EMPTY (NULL)
	@source: string (a pointer to a character list),
			 expression in string waiting to analyze
) no return
**************************************************************/
extern void generate_token (
	node** destination,
	const char* source
);

/**************************************************************
delete_token (
	@destination: a pointer to a pointer to structure node,
				  a pointer to expression token list
) no return, let @destination be EMPTY (NULL)
**************************************************************/
extern void delete_token (
	node** destination
);

/**************************************************************
view_token (
	@source: a pointer to structure node,
			 expression token list
) no return, print @source data in one line.
**************************************************************/
extern void view_token (
	node* source
);

/**************************************************************
syntax_check (
	@source: a pointer to structure node,
			 expression token list
) no return
**************************************************************/
extern void syntax_check (
	node* source
);

/**************************************************************
generate_tree (
	@destination: a pointer to a pointer to structure node,
				  a pointer to expression tree,
				  DEFALUT VALUE MUST BE EMPTY (NULL)
	@source: a pointer to structure node,
			 expression token list
) no return
**************************************************************/
extern void generate_tree (
	node** destination,
	node* source
);

/**************************************************************
delete_tree (
	@destination: a pointer to a pointer to structure node,
				  a pointer to expression tree
) no return, let @destination be EMPTY (NULL)
**************************************************************/
extern void delete_tree (
	node** destination
);

/**************************************************************
view_tree_postfix (
	@source: a pointer to structure node,
			 expression tree
) no return, print @source structure in postfix
**************************************************************/
extern void view_tree_postfix (
	node* source
);

/**************************************************************
**************************************************************/
extern float calculate(node* source);
#endif
