#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>
#include <stdio.h>
#include "types.h"

// variable that holds variable name, and element with type and value
struct dictionary_variable{
	char varname[15];
	ELEMENT_t element;
	ELEMENET_TYPE_e type;
	struct dictionary_variable* next;
} DICT_VAR_t;

// dictionary variable that holds a list
struct dictionary_list{
	char varname[15];
	struct generic_list_item* list;
  struct dictionary_variable* next;
} DICT_LIST_t;

// Dictionary
struct dictionary{
	DICT_VAR_t* head;
	int size;
} DICT_t;

// headers
DICT_VAR_t* newVariable(const char *varname, ELEMENT_TYPE_e type, ELEMENT_t element);
DICT_VAR_t* findVariable(DICT_t *dict, const char *varname);
void printVariable(DICT_t *dict, const char *varname);
void writeVariable(DICT_t *dict, const char *varname, ELEMENT_TYPE_e type, ELEMENT_t element);

// void removeElement(GENERIC_LIST_t *list, int index);


#endif // DICTIONARY_H
