#ifndef __EXPRESSION_TO_TOKEN_H__
#define __EXPRESSION_TO_TOKEN_H__

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

#endif
