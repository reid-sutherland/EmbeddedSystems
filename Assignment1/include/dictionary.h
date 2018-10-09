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
DICT_VAR_t* newVariable(char *varname, ELEMENT_t element, ELEMENT_TYPE_e type);
DICT_VAR_t* findVariable(DICT_t *dict, const char *varname);
DICT_VAR_t* readVariable(DICT_t *dict, const char *varname);
int writeVariable(DICT_t *dict, const char *varname, ELEMENT_t element, ELEMENT_TYPE_e type);
void removeVariable(DICT_t *dict, const char *varname);
void printVariable(DICT_t *dict, const char *varname);

DICT_LIST_t* newList(char *listname, ELEMENT_t element, ELEMENT_TYPE_e type);
DICT_LIST_t* findList(DICT_t *dict, const char *listname);
DICT_LIST_t* readList(DICT_t *dict, const char *listname);
int writeList(DICT_t *dict, const char *listname, ELEMENT_t element, ELEMENT_TYPE_e type);
void removeList(DICT_t *dict, const char *listname);
DICT_VAR_t* readListVar(DICT_t *dict, const char *listname, int index);
int writeListVar(DICT_t *dict, const char *listname, int index);
void removeListVar(DICT_t *dict, const char *listname, int index);


#endif // DICTIONARY_H
