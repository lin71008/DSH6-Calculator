#include "expression_to_token.h"

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
