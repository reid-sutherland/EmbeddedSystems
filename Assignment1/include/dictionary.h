#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "linkedList.h"

// variable that holds variable name, and element with type and value
typedef struct dictionary_variable{
	char* varname;
	ELEMENT_t element;
	ELEMENT_TYPE_e type;
	struct dictionary_variable* next;
} DICT_VAR_t;

// dictionary variable that holds a list
typedef struct dictionary_list{
	char* varname;
	struct generic_list_item* list;
	DICT_VAR_t* next;
} DICT_LIST_t;

// Dictionary
typedef struct dictionary{
	DICT_VAR_t* head;
	int size;
} DICT_t;

// headers
DICT_VAR_t* newVariable(const char *varname, ELEMENT_t element, ELEMENT_TYPE_e type);
DICT_VAR_t* findVariable(DICT_t *dict, const char *varname);
int writeVariable(DICT_t *dict, const char *varname, ELEMENT_t element, ELEMENT_TYPE_e type);
DICT_VAR_t* readVariable(DICT_t *dict, const char *varname);
void printVariable(DICT_t *dict, const char *varname);

// void removeElement(GENERIC_LIST_t *list, int index);


#endif // DICTIONARY_H
