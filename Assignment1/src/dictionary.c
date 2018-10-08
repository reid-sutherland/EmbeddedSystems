#include "../include/dictionary.h"


/*
Creates, inits, and returns a new variable pointer with allocated memory
*/
DICT_VAR_t* newVariable(const char *varname, ELEMENT_t element, ELEMENT_TYPE_e type) {

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
	if (dict->size == 0) {
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
int writeVariable(DICT_t *dict, const char *varname, ELEMENT_t element, ELEMENT_TYPE_e type) {

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
		DICT_VAR_t* var = newVariable(varname, element, type);

		// if error in new Variable, var is null, stop
		if (var == NULL) {
			return 0;
		}

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
	if ( strcmp(test->varname, varname) == 0 && test->type == type ) {
		switch (test->type) {
			case CHAR:
				if (test->element.c == element.c)
					return 1;
				break;
			case INT:
				if (test->element.i == element.i)
					return 1;
				break;
			case DOUBLE:
				if (test->element.d == element.d)
					return 1;
				break;
			case STRING:
				if (strcmp(test->element.s, element.s))
					return 1;
				break;
			// TODO add list
			default:
				printf("test switch case reached default\n");
				printf("writeVariable()\n");
				return 0;
				break;
		}
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
		printf("Error: Variable \"%s\" not found\n", varname);
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
		printf("Dict Error: Variable %s not found\n", varname);
		return;
	}

	switch (var->type) {
		case CHAR:
			// %c for char
			printf("%c\n", var->element.c);
			break;

		case INT:
			// %ld for signed long
			printf("%li\n", var->element.i);
			break;

		case DOUBLE:
			// %d for double
			printf("%.5f\n", var->element.d);
			break;

		case STRING:
			// %s for c-string format
			printf("%s\n", var->element.s);
			break;

		// TODO List
		// case LIST:


		default:
			printf("Could not print - Unrecognized type\n");
			break;
	}
}
