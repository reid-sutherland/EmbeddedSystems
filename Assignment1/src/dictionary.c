#include "../include/dictionary.h"
#include "../include/linkedList.h"


/*
Creates, inits, and returns a new variable pointer with allocated memory
*/
DICT_VAR_t* newVariable(const char *varname, ELEMENT_TYPE_e type, ELEMENT_t element) {

		// TODO enforce variable name rules here

		// create variable object, allocate memory and init
		DICT_VAR_t* newVar = calloc(1, sizeof(DICT_VAR_t));
		newVar->varname = varname;
		newVar->element = element;
		newVar->type = type;
		newVar->next = NULL;

		return newVar;
}

// TODO
// DICT_LIST_t* newList()



/*
Find a variable in the dictionary
returns null if variable not found
*/
DICT_VAR_t* findVariable(DICT_t *dict, const char *varname) {

	// check for empty dict
	if (dict->head == NULL) {
		return NULL;
	}

	DICT_VAR_t* var = dict->head;

	while (1) {
		// check for match
		if (strcmp(varname, var->varname) == 0) {
			return var;
		}

		// iterate if not at the end
		if (var->next != NULL) {
			var = var->next;
		}
		// end of list, var not found
		else {
			return NULL;
		}
	}
}



/*
Add a variable to the dictionary.
Returns 1 if write was successful
returns 0 or -1 otherwise
*/
int writeVariable(DICT_t *dict, const char *varname, ELEMENT_TYPE_e type, ELEMENT_t element) {

	// check if variable exists
	DICT_VAR_t* old = findVariable(dict, varname);

	// variable exists, overwrite
	if (old != NULL) {
		old->varname = varname;
		old->type = type;
		old->element = element;
		// do not change next - var is still at same place in the list
	}
	// variable does not exist, create new
	else {
		// create new Var
		DICT_VAR_t* var = newVariable(dict, varname, type, element);

		// empty dict
		if (dict->size == 0) {
			// assign var to head and initialize size
			dict->head = var;
			dict->size = 1;
		}
		// insert var into the dict
		else {
			DICT_VAR_t* iter;
			iter = dict->head;

			// loop to end of dict list
			while (iter->next != NULL) {
				iter = iter->next;
			}

			//TODO remove this
			// double check that iter is the last item in the list
			if (iter->next != NULL) {
				printf("Weird Error: Iter is not at the end of the list\n");
				printf("Stopping - don't write new var over existing variable\n");
				printf("writeVariable()\n");
				return 0;
			}

			// append var to end of dict list, increment size
			iter->next = var;
			dict->size += 1;
		}
	}


	// test whether the write was successful
	//	code should not reach this point if null values were encountered
	DICT_VAR_t* test = findVariable(dict, varname);
	if ( test->varname == varname && test->type == type && test->element == element ) {
		return 1;
	}
	else {
		printf("Write Error: variable \'%s\' was not written successfully\n", varname);
		printf("writeVariable()\n");
		return 0;
	}

}



/*
Read a variable and return the dict_var pointer
Unnecessary function (does nothing but)
*/
DICT_VAR_t* readVariable(DICT_t *dict, const char *varname) {

	// search for variable by name
	DICT_VAR_t* var = findVariable(dict, varname);

	// not found: stop with error
	if (var == NULL) {
		printf("Error: No such variable in dict\n");
		printf("readVariable()\n");
		return NULL;
	}
	else {
		return var;
	}
}



/*
Print a variable
*/
void printVariable(DICT_t *dict, const char *varname) {

	// find the variable in the dictionary
	DICT_VAR_t* var = findVariable(dict, varname);

	// var not found
	if (var == NULL) {
		printf("DictError: Variable %s not found\n", varname);
		printf("printVariable()\n");
		return;
	}

	switch (var->type) {
		case CHAR:
			// %c for char
			printf("%c\n", item->element.c);
			break;

		case INT:
			// %ld for signed long
			printf("%li\n", item->element.i);
			break;

		case DOUBLE:
			// %d for double
			printf("%.5f\n", item->element.d);
			break;

		case STRING:
			// %s for c-string format
			printf("%s\n", item->element.s);
			break;

		// TODO List
		// case LIST:


		default:
			printf("Could not print - Unrecognized type\n");
			break;
	}
}
