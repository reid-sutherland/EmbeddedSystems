#include "../include/dictionary.h"


/*
Creates, inits, and returns a new variable pointer with allocated memory
*/
DICT_VAR_t* newVariable(char *varname, ELEMENT_t element, ELEMENT_TYPE_e type) {

		// automatically shorten a variable name to 15 chars
		if (strlen(varname) > 15) {
			varname[15] = '\0';
		}
		// first character must be alpha...
		if (!isalpha(varname[0])) {
			printf("Syntax Error: Variable names must start with an alphabetic character\n");
			printf("  -> %s\n", varname);
			return NULL;
		}
		// ... and the rest must be alphanumeric
		int i = 0;
		while (varname[i] != '\0') {
			if (!isalnum(varname[i])) {
				printf("Syntax Error: Variable names must be alphanumeric\n");
				printf("  -> %s\n", varname);
				return NULL;
			}
			i++;
		}

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
Add a variable to the dictionary.
Returns 1 if write was successful
returns 0 or -1 otherwise
*/
int writeVariable(DICT_t *dict, const char *varname, ELEMENT_t element, ELEMENT_TYPE_e element_type, GENERIC_LIST_t* list) {

	// check if variable exists
	DICT_VAR_t* old = findVariable(dict, varname);

	// variable exists, overwrite
	if (old != NULL) {
		old->varname = varname;
		old->type = element_type;
		// update element value
		switch (element_type) {
			case CHAR:
				old->element.c = element.c;
				break;
			case INT:
				old->element.i = element.i;
				break;
			case DOUBLE:
				old->element.d = element.d;
				break;
			case STRING:
				old->element.s = element.s;
				break;
			case LIST:
				// TODO
				// old->list = list;
				break;
			default:
				break;
		}
		// do not change next - var is still at same place in the list
	}
	// variable does not exist, create new
	else {
		DICT_VAR_t* var;
		var = newVariable(varname, element, element_type);

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

			// append var to end of dict list, increment size
			iter->next = var;
			dict->size += 1;
		}
	}


	// test whether the write was successful
	//	code should not reach this point if null values were encountered
	DICT_VAR_t* test = findVariable(dict, varname);
	if ( test == NULL) {
		// do nothing, if findVariable did not return anything then
		// the error will be caught somewhere else
	}
	if ( strcmp(test->varname, varname) == 0 && test->type == element_type ) {
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
			case LIST:
				return 1;
				break;
			default:
				//TODO remove this
				printf("test switch case reached default\n");
				printf("  writeVariable()\n");
				return 0;
				break;
		}
	}
	else {
		printf("Write Error: variable \'%s\' was not written successfully\n", varname);
		return 0;
	}
}

void removeVariable(DICT_t *dict, const char *varname) {

}



/*
Print a variable
*/
void printVariable(DICT_t *dict, const char *varname) {

	// find the variable in the dictionary
	DICT_VAR_t* var = findVariable(dict, varname);

	// var not found
	if (var == NULL) {
		printf("Print Error: Variable %s not found\n", varname);
		return;
	}

	switch (var->type) {
		case CHAR:
			// %c for char
			printf("\'%c\'\n", var->element.c);
			break;
		case INT:
			// %li for signed long
			printf("%li\n", var->element.i);
			break;
		case DOUBLE:
			// %d for double
			// TODO fix decimals - convert to string, remove trailing zeros, convert back to double
			printf("%.5f\n", var->element.d);
			break;
		case STRING:
			// %s for c-string format
			printf("\"%s\"\n", var->element.s);
			break;
		case LIST:
			// printList(var->list, 1);
			break;
		default:
			printf("Could not print - Unrecognized type\n");
			break;
	}
}



// **********************************
//         LIST FUNCTIONS
// **********************************
// DICT_VAR_t* newListVar(char *varname, GENERIC_LIST_t* list) {
// 	// automatically shorten a variable name to 15 chars
// 	if (strlen(varname) > 15) {
// 		varname[15] = '\0';
// 	}
// 	// first character must be alpha...
// 	if (!isalpha(varname[0])) {
// 		printf("Syntax Error: Variable names must start with an alphabetic character\n");
// 		printf("  -> %s\n", varname);
// 		return NULL;
// 	}
// 	// ... and the rest must be alphanumeric
// 	int i = 0;
// 	while (varname[i] != '\0') {
// 		if (!isalnum(varname[i])) {
// 			printf("Syntax Error: Variable names must be alphanumeric\n");
// 			printf("  -> %s\n", varname);
// 			return NULL;
// 		}
// 		i++;
// 	}
//
// 	// create variable object, allocate memory and init
// 	DICT_VAR_t* newVar = calloc(1, sizeof(DICT_VAR_t));
// 	newVar->varname = varname;
// 	newVar->type = LIST;
// 	newVar->next = NULL;
// 	newVar->list = calloc(1, sizeof(GENERIC_LIST_t));
// 	if (list == NULL) {
// 		newVar->list->head = NULL;
// 		newVar->list->size = 0;
// 	}
// 	else {
//
// 	}
//
// 	return newVar;
// }
//
//
// void removeList(DICT_t *dict, const char *varname) {
//
// }
//
//
// DICT_VAR_t* readListVar(DICT_t *dict, const char *varname, int index) {
//
// }
//
//
// int writeListVar(DICT_t *dict, const char *varname, int index);
//
//
// void removeListVar(DICT_t *dict, const char *varname, int index);
