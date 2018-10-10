#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <ctype.h>		// isdigit
#include "types.h"
#include "linkedList.h"

// variable that holds variable name, and element with type and value
typedef struct dictionary_variable{
	char* varname;
	ELEMENT_t element;
	ELEMENT_TYPE_e type;
	struct dictionary_variable* next;
} DICT_VAR_t;

// Dictionary
typedef struct dictionary{
	DICT_VAR_t* head;
	int size;
} DICT_t;

// headers
DICT_VAR_t* newVariable(char *varname, ELEMENT_t element, ELEMENT_TYPE_e type);
DICT_VAR_t* findVariable(DICT_t *dict, const char *varname);
DICT_VAR_t* readVariable(DICT_t *dict, const char *varname);
int writeVariable(DICT_t *dict, const char *varname, ELEMENT_t element, ELEMENT_TYPE_e type);
void removeVariable(DICT_t *dict, const char *varname);
void printVariable(DICT_t *dict, const char *varname);


#endif // DICTIONARY_H
