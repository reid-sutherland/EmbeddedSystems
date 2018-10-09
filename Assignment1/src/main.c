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

		// lets get rid of that newline if it's there
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
		// parse_string assumes assignment if no operator is found, otherwise sets op to last op found
		// parse_string grabs text up until " +-*/=", stores in left/right1/right2 (in order) each time
		// right2 is null only if a third operand token is not found, function assumes there are at least 2 operands
		PARSE_RESULT_t* result = parse_string(input_buffer);

		// take action based result's optype
		switch (result->optype) {
			case APPEND_OP:
				break;

			case PRINT_OP:
				if (result->left_operand != NULL) {
					printVariable(&dict, result->left_operand);
				}
				else {
					printf("Syntax Error: print() field is empty, nothing to print\n");
				}
				break;

			case ASSIGN_OP:
				// process the operands if it is not a print or append statement
				processParseResult(&dict, result);
				// debug
				// printResult(result);

				// only one operand
				if (result->right_operand1 == NULL) {
					// print the left operand variable if it exists
					printVariable(&dict, result->left_operand);
					break;
				}

				// should be only two operands on basic assignment
				if (result->right_operand2 != NULL) {
					printf("Syntax Error: Too many operands for assignment\n");
					break;
				}

				// parse the operand to be assigned
				// variables
				DICT_VAR_t* opVar;
				const char* varname = result->left_operand;
				ELEMENT_t element;
				ELEMENT_TYPE_e element_type;
				char* c = result->right_operand1;

				// write flag
				int write = 1;
				switch (result->right_type1) {
					case CHAR_OP:
						// index 1 should be the char between ''
						element.c = c[1];
						element_type = CHAR;
						break;

					case INT_OP:
						element.i = atoi(result->right_operand1);
						element_type = INT;
						break;

					case DOUBLE_OP:
						element.d = atof(result->right_operand1);
						element_type = DOUBLE;
						break;

					case STRING_OP:
						element.s = removeQuotes(result->right_operand1);
						element_type = STRING;
						break;

					// TODO list

					case VAR_OP:
						opVar = readVariable(&dict, result->right_operand1);
						// break if variable not found
						if (opVar == NULL) {
							break;
						} else {
							element = opVar->element;
							element_type = opVar->type;
						}
						break;

					default:
						// if we get here, we do not need to update an element value
						write = 0;
						break;
				}
				// update variable
				if (write)
					writeVariable(&dict, result->left_operand, element, element_type);
				break;

			case ADD_OP:
				// TODO OPTIONAL: change the parse result process to handle two operand math (i.e. "1 + 2" print)
				// process the operands if it is not a print or append statement
				processParseResult(&dict, result);
				// debug
				// printResult(result);

				mathOp(&dict, result, ADD_OP);
				break;

			case SUB_OP:
				// process the operands if it is not a print or append statement
				processParseResult(&dict, result);
				// debug
				// printResult(result);

				mathOp(&dict, result, SUB_OP);
				break;

			case MULT_OP:
				// process the operands if it is not a print or append statement
				processParseResult(&dict, result);
				// debug
				// printResult(result);

				mathOp(&dict, result, MULT_OP);
				break;

			case DIV_OP:
				// process the operands if it is not a print or append statement
				processParseResult(&dict, result);
				// debug
				// printResult(result);

				mathOp(&dict, result, DIV_OP);
				break;

			default:
				// TODO remove this
				printf("Error: reached default of optype switch statement\n");
				printf("main()\n");
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
