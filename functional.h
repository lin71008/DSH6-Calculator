#ifndef __FUNCTIONAL_H__
#define __FUNCTIONAL_H__

#include <stdlib.h>
#include <string.h>

/**************************************************************
int create_split(
	@destination: a list of string,
				  each of element either operator or numerical.
				  DEFAULT VALUE MUST BE NULL.
	@source: string,
		     the expression need to be split.
	@max: unsigned integer,
		  the default length of the list of string.
)	@length: unsigned integer,
			 return the length of @destination.
**************************************************************/
size_t create_split(char*** destination,
					const char* source,
					const size_t max);

/**************************************************************
void view_split(
	@destination: a list of string,
				  each of element either operator or numerical.
				  DEFAULT VALUE MUST BE NULL.
 	@length: unsigned integer,
			 the length of @destination.
)	no return
**************************************************************/

void view_split(char** source,
				const size_t length);

/**************************************************************
void delete_split(char***, const size_t)
	@source: a list of string,
				  each of element either operator or numerical.
				  DEFAULT VALUE MUST BE NULL.
	@length: unsigned integer,
		  the length of the list of string.
)	no return
**************************************************************/
void delete_split(char*** destination,
				  const size_t length);

/**************************************************************
struct node:
	@type: enum symbol_type,
		   labeled node type is either operator, numerical or
		   unknown.
	union:
		   @opt: enum opt_type
		   @val: integer
	next:
		  @left
		  @right
**************************************************************/
typedef struct node node;
typedef enum symbol_type symt;
typedef enum operator_type optt;

/**************************************************************
virtual stack: node
	@type: enum symbol_type,
		   labeled node type is either operator, numerical or
		   unknown.
	union:
		   @opt: enum opt_type
		   @val: integer
	@next
**************************************************************/
typedef node stack;


/**************************************************************
node* new_node(
	@symbol: string,
			 DEFAULT VALUE IS UNKNOWN_SYM
)	@new: point to node,
		  return a new node.
**************************************************************/

/**************************************************************
void delete_node(
	@destination: point to node,
				  MAYBE EMPTY
)
**************************************************************/

/**************************************************************
**************************************************************/
#endif


// {type, *void}
