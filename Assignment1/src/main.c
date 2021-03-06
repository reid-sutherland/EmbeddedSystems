#include <string.h>
#include <stdio.h>
#include <time.h>
#include "../include/linkedList.h"
#include "../include/parser.h"

// function headers
void mathOp(DICT_t* dict, PARSE_RESULT_t* parse_result, OPTYPE_e op);
void testParser();

int main() {

	// SETUP
	DICT_t dict;
	dict.size = 0;
	dict.head = NULL;

	char input_buffer[MAX_BUFFER_LEN];

	printf("=============================================\n");
	printf("  Welcome to Reid's Python Interpreter!\n");
	printf("   Type \"exit\" or \"exit()\" to quit.\n");
	printf("=============================================\n");
	printf("\n");

	// LOOP
	while (1) {

		// print >>>
		printf(">>> ");

		// read input
		fgets(input_buffer, MAX_BUFFER_LEN-1, stdin);

		// let's get rid of that newline if it's there
		int last = strlen(input_buffer) - 1;
		if (input_buffer[last] == '\n') {
			input_buffer[last] = '\0';
		}

		// check for whitespace
		char* pch = input_buffer;
		int isSpace = 1;
		while (*pch != '\0') {
			if (!isspace((unsigned char) *pch)) {
				isSpace = 0;
			}
			pch++;
		}

		// if whitespace, do nothing
		if (isSpace == 1 || strcmp(input_buffer, "") == 0) {
			continue;
		}
		// check for exit
		if (strcmp(input_buffer, "exit()") == 0 || strcmp(input_buffer, "exit") == 0) {
			break;
		}

		// parse the input string
		PARSE_RESULT_t* result = parse_string(input_buffer);
		// process the result to determine type of each operand, and errors if present
		processParseResult(&dict, result);

		// take action based result's optype
		switch (result->optype) {

			// *****************
			// APPEND
			// *****************
			case APPEND_OP: {
				// empty list argument
				if (result->left_operand == NULL) {
					printf("\n\n");
					printf("Syntax Error: No list specified\n");
					break;
				}
				// empty/error append argument
				else if (result->right_operand1 == NULL || result->right_type1 == ERROR_OP) {
					printf("\n\n");
					printf("Syntax Error: Nothing to append\n");
					break;
				}

				// append the element to the list
				DICT_VAR_t* var = readVariable(&dict, result->left_operand);
				ELEMENT_t element;
				ELEMENT_TYPE_e element_type;
				if (var != NULL && var->type == LIST) {

					switch (result->right_type1) {
						case CHAR_OP: {
							// TODO figure out how to print '' instead of 0
							char *c = result->right_operand1;
							element.c = *c;
							element_type = CHAR;
							break;
						}
						case INT_OP:
							element.i = atoi(result->right_operand1);
							element_type = INT;
							break;
						case DOUBLE_OP:
							element.d = atof(result->right_operand1);
							element_type = DOUBLE;
							break;
						case STRING_OP:
							element.s = result->right_operand1;
							element_type = STRING;
							break;
						case LIST_OP: {
							// TODO change this if I allow more than [] (initialization) as operand
							// allocate space for and initialize list
							GENERIC_LIST_t* list = (GENERIC_LIST_t*) calloc(1, sizeof(GENERIC_LIST_t));
							list->head = NULL;
							list->size = 0;
							element.l = list;
							element_type = LIST;
							break;
						}
						case VAR_OP: {
							DICT_VAR_t* opVar = readVariable(&dict, result->right_operand1);
							// break if variable not found
							if (opVar == NULL) {
								break;
							} else {
								element = opVar->element;
								element_type = opVar->type;
							}
							break;
						}
						default:
							break;
					}
					// append the element to the list
					addElement(var->element.l, element, element_type);
				}
				else {
					printf("  -> %s\n\n", var->varname);
					printf("Append Error: Variable was not a list or not found in dictionary\n");
					break;
				}

				break;
			}

			// *****************
			// PRINT
			// *****************
			case PRINT_OP:
				if (result->left_operand != NULL) {
					printVariable(&dict, result->left_operand);
				}
				else {
					printf("Syntax Error: print() field is empty, nothing to print\n");
				}
				break;

			// *****************
			// ASSIGN
			// *****************
			case ASSIGN_OP: {
				// only one operand
				if (result->right_operand1 == NULL) {
					// print the left operand variable if it exists
					if (readVariable(&dict, result->left_operand) != NULL) {
						printVariable(&dict, result->left_operand);
					}
					break;
				}
				// should be only two operands on basic assignment
				if (result->right_operand2 != NULL) {
					printf("Syntax Error: Too many operands for assignment\n");
					break;
				}

				// parse the operand to be assigned
				// variables
				ELEMENT_t element;
				ELEMENT_TYPE_e element_type;

				// write flag
				int write = 1;
				switch (result->right_type1) {
					case CHAR_OP: {
						// TODO figure out how to print '' instead of 0
						char *c = result->right_operand1;
						element.c = *c;
						element_type = CHAR;
						break;
					}
					case INT_OP:
						element.i = atoi(result->right_operand1);
						element_type = INT;
						break;
					case DOUBLE_OP:
						element.d = atof(result->right_operand1);
						element_type = DOUBLE;
						break;
					case STRING_OP:
						element.s = result->right_operand1;
						element_type = STRING;
						break;
					case LIST_OP: {
						// TODO change this if I allow more than [] (initialization) as operand
						// allocate space for and initialize list
						GENERIC_LIST_t* list = (GENERIC_LIST_t*) calloc(1, sizeof(GENERIC_LIST_t));
						list->head = NULL;
						list->size = 0;
						element.l = list;
						element_type = LIST;
						break;
					}
					case VAR_OP: {
						DICT_VAR_t* opVar = readVariable(&dict, result->right_operand1);
						// break if variable not found
						if (opVar == NULL) {
							write = 0;
							break;
						} else {
							element = opVar->element;
							element_type = opVar->type;
						}
						break;
					}
					case INDEX_OP: {
						int index = processListIndex(&dict, result->right_operand1, 0);
						if (index == -1) {
							write = 0;
							break;
						}
						else {
							// pointer for parsing
							char* pch;
							// get variable name
							pch = strtok(result->right_operand1, "[");
							// get variable from name
							DICT_VAR_t* var = readVariable(&dict, pch);
							if (var == NULL) {
								break;
							}
							if (var->type != LIST) {
								printf("  -> %s\n\n", result->right_operand1);
								printf("Assign Error: Cannot access index of a non-list variable\n");
							}
							GENERIC_LIST_ITEM_t* item = getItem(var->element.l, index);
							element = item->element;
							element_type = item->type;
						}
						break;
					}
					default:
						// if we get here, we do not need to update an element value
						write = 0;
						break;
				}
				// update variable
				if (write) {
					// check if left operand is a list
					int index = processListIndex(&dict, result->left_operand, 1);
					// not a list
					if (index == -1) {
						writeVariable(&dict, result->left_operand, element, element_type);
					}
					// is a list
					else {
						// pointer for parsing
						char* pch;
						// get variable name
						pch = strtok(result->left_operand, "[");
						// get variable from name
						DICT_VAR_t* var = readVariable(&dict, pch);
						if (var == NULL) {
							break;
						}
						if (var->type != LIST) {
							printf("  -> %s\n\n", result->left_operand);
							printf("Assign Error: Cannot access index of a non-list variable\n");
						}
						GENERIC_LIST_ITEM_t* item = getItem(var->element.l, index);
						item->element = element;
						item->type = element_type;
					}
				}
				break;
			}

			case ADD_OP:
				mathOp(&dict, result, ADD_OP);
				break;

			case SUB_OP:
				mathOp(&dict, result, SUB_OP);
				break;

			case MULT_OP:
				mathOp(&dict, result, MULT_OP);
				break;

			case DIV_OP:
				mathOp(&dict, result, DIV_OP);
				break;

			default:
				break;
		}

		// printf("%s\n", input_buffer);
	}

	return 0;
}


void mathOp(DICT_t* dict, PARSE_RESULT_t* parse_result, OPTYPE_e op) {
	// operands must not be null (this should not be reached)
	if (parse_result->right_operand1 == NULL || parse_result->right_operand2 == NULL) {
		printf("Math Error: Not enough operands in expression\n");
		return;
	}

	// if operand(s) have errors, stop
	if (parse_result->right_type1 == ERROR_OP || parse_result->right_type2 == ERROR_OP) {
		return;
	}

	// variables
	DICT_VAR_t *var1, *var2, *result_var;
	const char *varname = parse_result->left_operand;
	ELEMENT_t element1, element2, result_element;
	ELEMENT_TYPE_e type1, type2, result_type;

	// parse operands into elements and element types
	// convert from vars if necessary
	// operand1
	if (parse_result->right_type1 == VAR_OP) {
		var1 = readVariable(dict, parse_result->right_operand1);
		element1 = var1->element;
		type1 = var1->type;
	}
	else if (parse_result->right_type1 == LIST_OP) {
		//TODO set the type based on list item
	} else if (parse_result->right_type1 == INT_OP) {
		element1.i = atoi(parse_result->right_operand1);
		type1 = INT;
	} else if (parse_result->right_type1 == DOUBLE_OP) {
		element1.d = atof(parse_result->right_operand1);
		type1 = DOUBLE;
	}
	// operand2
	if (parse_result->right_type2 == VAR_OP) {
		var2 = readVariable(dict, parse_result->right_operand2);
		element2 = var2->element;
		type2 = var2->type;
	}
	else if (parse_result->right_type2 == LIST_OP) {
		//TODO set the type based on list item
	} else if (parse_result->right_type2 == INT_OP) {
		element2.i = atoi(parse_result->right_operand2);
		type2 = INT;
	} else if (parse_result->right_type2 == DOUBLE_OP) {
		element2.d = atof(parse_result->right_operand2);
		type2 = DOUBLE;
	}

	// operands must be the same type
	if ((type1 != INT && type1 != DOUBLE)
			|| (type2 != INT && type2 != DOUBLE)) {
		printf("Math Error: Math operations only permitted on types int and double\n");
		return;
	} else if (type1 != type2) {
		printf("Math Error: Both right operands must be the same type\n");
		return;
	}


	// int and int
	if (type1 == INT && type2 == INT) {
		result_type = INT;
		switch (op) {
			case ADD_OP:
				result_element.i = element1.i + element2.i;
			 	break;
			case SUB_OP:
				result_element.i = element1.i - element2.i;
				break;
			case MULT_OP:
				result_element.i = element1.i * element2.i;
				break;
			case DIV_OP:
				// catch divide by 0
				if (element2.i == 0) {
					printf("Math Error: Cannot divide by 0\n");
					return;
				}
				else
					result_element.i = element1.i / element2.i;
				break;
			default:
				return;
				break;
		 }
	}
	// double and double
	else if (type1 == DOUBLE && type2 == DOUBLE) {
		result_type = DOUBLE;
		switch (op) {
			case ADD_OP:
				result_element.d = element1.d + element2.d;
			 	break;
			case SUB_OP:
				result_element.d = element1.d - element2.d;
				break;
			case MULT_OP:
				result_element.d = element1.d * element2.d;
				break;
			case DIV_OP:
				// catch divide by 0
				if (element2.d == 0.0f) {
					printf("Math Error: Cannot divide by 0\n");
					return;
				}
				else
					result_element.d = element1.d / element2.d;
				break;
			default:
				return;
				break;
		 }
	}

	// write variable
	writeVariable(dict, parse_result->left_operand, result_element, result_type);
	// print result to console
	printVariable(dict, parse_result->left_operand);
}



void testParser() {
	PARSE_RESULT_t* result;
    //Modify this string literal to test the parser.
    //The parser does not handle improperly formatted strings
    result = parse_string("a = b + c");
    //Test PRINT_OP parsing
    if(result->optype == PRINT_OP){
        printf("Print Variable: %s",result->left_operand);
    }
    //Test APPEND_OP parsing
    else if(result->optype == APPEND_OP){
        printf("Append Variable: %s",result->left_operand);
    }
    //Test other parsing
    else{
        //If there are two operands, there must be some arithmetic
        if(result->right_operand2 != NULL){
            switch(result->optype){
                case ADD_OP: //Test add operation
                    printf("%s = %s + %s",result->left_operand,result->right_operand1,result->right_operand2);
                    break;
                case SUB_OP: // Test sub operation
                    printf("%s = %s - %s",result->left_operand,result->right_operand1,result->right_operand2);
                    break;
                case MULT_OP: // Test mult operation
                    printf("%s = %s * %s",result->left_operand,result->right_operand1,result->right_operand2);
                    break;
                case DIV_OP: // Test div operation
                    printf("%s = %s / %s",result->left_operand,result->right_operand1,result->right_operand2);
                    break;
                default:
                    break;
            }
        }
        //If no second operand, just print left_operand and right_operand1
        else{
            printf("%s = %s",result->left_operand,result->right_operand1);
        }
    }

    //We should free the parse_string result here.
	return;
}
