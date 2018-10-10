#include "../include/parser.h"

/*
parse_string function
Input -- Input buffer from string to be parsed
Output -- PARSE_RESULT_t pointer holding strings of operands, and an operation type
PARSE_RESULT_t will have pointers to strings representing operands that will need to be freed
*/
/*
PARSE_RESULT_t* parse_string(const char* input_buffer){
    //Create a PARSE_RESULT_t structure in dynamic memory, and hold pointer in result
    PARSE_RESULT_t* result = (PARSE_RESULT_t*)calloc(1,sizeof(PARSE_RESULT_t));
    //Create a copy of the input buffer string into variable command
    char* command = (char*)calloc(MAX_BUFFER_LEN,sizeof(char));
    strcpy(command, input_buffer);
    //Create two pointers. Token points at each new token, rest holds memory location
    //for strtok_r so it can be reentrant
    char *token, *rest;
    rest = command;
    //If the command starts with "print("
    if(strncmp(command,PRINT_COMMAND,6)==0){
        //Set optype to PRINT_OP
        result->optype = PRINT_OP;
        //Ignore first token (says print)
        strtok_r(rest, "()", &rest);
        //Make a string in dynamic memory to hold the variable operand
        char* operand = calloc(MAX_OPERAND_LEN,sizeof(char));
        token = strtok_r(rest, "()", &rest);
        //Copy the token onto the operand
        strcpy(operand,token);
        //Set the operand to the left operand field
        result->left_operand = operand;
        //Free the copied buffer
        free(command);
        return result;
    }
    else if(strncmp(command,APPEND_COMMAND,7)==0){
        //Set optype to APPEND_OP
        result->optype = APPEND_OP;
        //Ignore first token (says append)
        strtok_r(rest, "()", &rest);
        //Make a string in dynamic memory to hold the variable operand
        char* operand = calloc(MAX_OPERAND_LEN,sizeof(char));
        token = strtok_r(rest, ", ", &rest);
		strcpy(operand,token);
		// modify left operand
		result->left_operand = operand;
		token = strtok_r(rest, "()", &rest);
		if (token != NULL) {
			operand = calloc(MAX_OPERAND_LEN, sizeof(char));
			strcpy(operand, token);
			result->right_operand1 = operand;
		} else {
			result->right_operand1 = NULL;
		}
        //Free the copied buffer
        free(command);
        return result;
    }
    else{ //Assignment operation, may have arithmetic
        //By default, assume it only assigns unless an arithmetic operator is found
        result->optype = ASSIGN_OP;
        int i = 0;
        //Iterate through the command string until NULL is found
        while(command[i] != '\0'){
            //Switch on the character
            switch(rest[i++]) {
                case '+': // If add, set optype to ADD_OP
                    result->optype = ADD_OP;
                    break;
                case '-': // If sub, set optype to SUB_OP
                    result->optype = SUB_OP;
                    break;
                case '*': // If multiply, set optype to MULT_OP
                    result->optype = MULT_OP;
                    break;
                case '/': // If divide, set optype to DIV_OP
                    result->optype = DIV_OP;
                    break;
                default: // Do nothing if character is anything else
                    break;
            }
        }
        //Optype is now set to correct OPTYPE, time to find the operands
        //Get default memory to hold the operand strings
        char* operand = calloc(MAX_OPERAND_LEN,sizeof(char));
        //Find first operand (strtok_r removes spaces, assignment, and other operators, returning next non-token string
        token = strtok_r(rest, " +-/*=", &rest);
        strcpy(operand,token);
        //Modify left_operand to point at operand location
        result->left_operand = operand;
        //Do the same process for the first right operand
		// Don't assume there are 2 operands
        token = strtok_r(rest, " +-/*=", &rest);
		if (token != NULL) {
			operand = calloc(MAX_OPERAND_LEN,sizeof(char));
			strcpy(operand,token);
	        result->right_operand1 = operand;
		} else {
			result->right_operand1 = NULL;
		}
        //There may or may not be a third operand
        token = strtok_r(rest, " +-/*=", &rest);
        //Check to see if token is NULL, if not, then assign third operand
        if(token!=NULL){
            //If not NULL, set right operand2 to last token
            operand = calloc(MAX_OPERAND_LEN,sizeof(char));
            strcpy(operand,token);
            result->right_operand2 = operand;
        }else{
            result->right_operand2 = NULL;
        }
        //Free dynamic memory for the copied command string
        free(command);
        return result;
    }
}
*/

/*
Alternative parse_string function

Apparently Ubuntu18 has a huge problem with strtok_r, and I don't feel like
figuring it out. The use of the function causes an immediate segfault.
So this is an alternative parse_result function that works
on my bullpen computer.
*/
PARSE_RESULT_t* parse_string(const char* input_buffer){
	//Create a PARSE_RESULT_t structure in dynamic memory, and hold pointer in result
	PARSE_RESULT_t* result = (PARSE_RESULT_t*)calloc(1,sizeof(PARSE_RESULT_t));
	//Create a copy of the input buffer string into variable command
	char* command = (char*)calloc(MAX_BUFFER_LEN,sizeof(char));
	strcpy(command, input_buffer);
	//Create two pointers. Token points at each new token, rest holds memory location
	//for strtok_r so it can be reentrant
	char *token, *rest;
	rest = command;
	//If the command starts with "print("
	if(strncmp(command,PRINT_COMMAND,6)==0){
    //Set optype to PRINT_OP
    result->optype = PRINT_OP;
    //Ignore first token (says print)
    // strtok_r(rest, "()", &rest);
		strtok(rest, "()");
    //Make a string in dynamic memory to hold the variable operand
    char* operand = calloc(MAX_OPERAND_LEN,sizeof(char));
    // token = strtok_r(rest, "()", &rest);
		token = strtok(NULL, "()");
    //Copy the token onto the operand
    strcpy(operand,token);
    //Set the operand to the left operand field
    result->left_operand = operand;
    //Free the copied buffer
    free(command);
    return result;
	}
  else if(strncmp(command,APPEND_COMMAND,7)==0){
    //Set optype to APPEND_OP
    result->optype = APPEND_OP;
    //Ignore first token (says append)
    // strtok_r(rest, "()", &rest);
		strtok(rest, "()");
    //Make a string in dynamic memory to hold the variable operand
    char* operand = calloc(MAX_OPERAND_LEN,sizeof(char));
    // token = strtok_r(rest, ", ", &rest);
		token = strtok(NULL, ", ");
		strcpy(operand,token);
		// modify left operand
		result->left_operand = operand;
		// token = strtok_r(rest, "()", &rest);
		token = strtok(NULL, "()");
		if (token != NULL) {
			operand = calloc(MAX_OPERAND_LEN, sizeof(char));
			strcpy(operand, token);
			result->right_operand1 = operand;
		}
		else {
			result->right_operand1 = NULL;
		}
    //Free the copied buffer
    free(command);
    return result;
  }
  else{ //Assignment operation, may have arithmetic
		//By default, assume it only assigns unless an arithmetic operator is found
		result->optype = ASSIGN_OP;
		int i = 0;
		//Iterate through the command string until NULL is found
		while(command[i] != '\0'){
	    //Switch on the character
	    switch(rest[i++]) {
        case '+': // If add, set optype to ADD_OP
          result->optype = ADD_OP;
          break;
        case '-': // If sub, set optype to SUB_OP
          result->optype = SUB_OP;
          break;
        case '*': // If multiply, set optype to MULT_OP
          result->optype = MULT_OP;
          break;
        case '/': // If divide, set optype to DIV_OP
          result->optype = DIV_OP;
          break;
        default: // Do nothing if character is anything else
          break;
	    }
		}
		//Optype is now set to correct OPTYPE, time to find the operands
		//Get default memory to hold the operand strings
		char* operand = calloc(MAX_OPERAND_LEN,sizeof(char));
		//Find first operand (strtok_r removes spaces, assignment, and other operators, returning next non-token string
		// token = strtok_r(rest, " +-/*=", &rest);
		token = strtok(rest, " +-/*=");
		strcpy(operand,token);
		//Modify left_operand to point at operand location
		result->left_operand = operand;
		//Do the same process for the first right operand
		// Don't assume there are 2 operands
		// token = strtok_r(rest, " +-/*=", &rest);
		token = strtok(NULL, " +-/*=");
		if (token != NULL) {
			operand = calloc(MAX_OPERAND_LEN,sizeof(char));
			strcpy(operand,token);
			result->right_operand1 = operand;
		}
		else{
			result->right_operand1 = NULL;
		}
    //There may or may not be a third operand
    // token = strtok_r(rest, " +-/*=", &rest);
		token = strtok(NULL, " +-/*=");
    //Check to see if token is NULL, if not, then assign third operand
    if(token!=NULL){
      //If not NULL, set right operand2 to last token
      operand = calloc(MAX_OPERAND_LEN,sizeof(char));
      strcpy(operand,token);
      result->right_operand2 = operand;
    }
		else{
      result->right_operand2 = NULL;
    }
    //Free dynamic memory for the copied command string
    free(command);
    return result;
  }
}



/*
Determine the types of each operand
Check for errors infree parse results
NOPE -> Removes '' and "".
NOPE -> "Examp"le" is returned as Examp for now

Returns 1 if no issues
Returns 0 if errors (parse result should be discarded elsewhere)
  (i.e. right operand is null)
*/
void processParseResult(DICT_t* dict, PARSE_RESULT_t *result) {
	if (result->right_operand1 != NULL) {
		result->right_type1 = processOperand(dict, result->right_operand1);
	}
	if (result->right_operand2 != NULL) {
		result->right_type2 = processOperand(dict, result->right_operand2);
	}
}

OPERAND_TYPE_e processOperand(DICT_t* dict, char* operand) {
	// TODO improve error catching for improper inputs ()

	// check for char, string, and list
	//  much be at least length 2
	if (strlen(operand) > 1) {

		// CHAR
		if (operand[0] == '\'' && operand[strlen(operand)-1] == '\'') {
			// char operand ('a') should only be length 2 or 3
			if (strlen(operand) == 2) {
				*operand = NULL;
			}
			else if (strlen(operand) == 3) {
				// char c = operand[1];
				*operand = operand[1];
				operand[1] = '\0';
				return CHAR_OP;
			}
			else if (strlen(operand) > 3) {
				printf("  -> %s\n\n", operand);
				printf("Syntax Error: char should be a single ASCII character\n");
				return ERROR_OP;
			}
			// TODO check for 'exam'ple'
			// TODO optionally replace error with this if Nelson approves
			// treat 'example' as a string for convenience
			// else if (strlen(operand) > 3) {
			// 	oper_type = STRING_OP;
			// }
		}
		// STRING
		else if (operand[0] == '\"' && operand[strlen(operand)-1] == '\"') {
			// enforce 50 character string limit
			if (strlen(operand) > 52) {	// 50 + "" = 52
				// terminate 53rd char with null character
				operand[51] = '\"';
				operand[52] = '\0';
			}
			// TODO check for "exam"ple"
			// remove the quotations from the operand
			removeQuotes(operand);
			return STRING_OP;
		}
		// LIST
		else if (operand[0] == '[' && operand[strlen(operand)-1] == ']') {
			// for now, only kind of list operand should be initialization "[]"
			if (strcmp(operand, "[]") == 0) {
				return LIST_OP;
			} else {
				printf("  -> %s\n\n", operand);
				printf("Syntax Error: Improper list syntax\n");
				return ERROR_OP;
			}
		}
		// INDEX
		else if (operand[0] != '[' && strchr(operand, '[') != NULL && strchr(operand, ']') != NULL) {
			// parsing the index symbol will have to be done elsewhere
			// for now just make sure its valid and return if valid

			// should only be one of each bracket
			int i = 0, leftCount = 0, rightCount = 0;
			while (operand[i] != '\0') {
				if (operand[i] == '[') {
					leftCount++;
				} else if (operand[i] == ']') {
					rightCount++;
				}
				// right bracket came first, error
				if (rightCount > leftCount) {
					printf("  -> %s\n\n", operand);
					printf("Syntax Error: Brackets out of order\n");
					return ERROR_OP;
				}
				i++;
			}
			if (leftCount != 1 || rightCount != 1) {
				printf("  -> %s\n\n", operand);
				printf("Syntax Error: Too many/too few brackets\n");
				return ERROR_OP;
			}
			else {
				// pointer for parsing
				char* pch;
				// get variable name
				pch = strtok(operand, "[");
				// make sure variable exists and is a list
				DICT_VAR_t* var = readVariable(dict, pch);
				if (var != NULL) {
					// not a list
					if (var->type != LIST) {
						printf("  -> %s\n\n", operand);
						printf("Syntax Error: Variable %s is not a list\n", var->varname);
						return ERROR_OP;
					}
				}
				// does not exist
				else {
					printf("  -> %s\n\n", operand);
					printf("Error: Variable not found in dictionary\n");
					return ERROR_OP;
				}
				// check that index is int
				pch = strtok(NULL, "]");
				char* pch_iter = pch;
				while (pch_iter != '\0' && (int) *pch_iter != 0) {
					if (!isdigit(*pch_iter)) {
						printf("  -> %s\n\n", operand);
						printf("Syntax Error: Non-integer in index\n");
						return ERROR_OP;
					}
					pch_iter++;
				}

				// check that the index is inbounds
				int index = atoi(pch);
				GENERIC_LIST_t* list = var->element.l;
				if (index > list->size - 1) {
					printf("  -> [%i]\n\n", list->size);
					printf("Out of Bounds Error: Index is larger than size of list\n");
					return ERROR_OP;
				}

				return INDEX_OP;
			}
		}
	}

	// check for int/double
	int i = 0, isInt = 1, isDouble = 1, decimalCount = 0;
	while (operand[i] != '\0') {
		if (!isdigit(operand[i])) {
			isInt = 0;
			if (operand[i] == '.') {
				decimalCount++;
			}
			// if other character besides . or number,
			// it is not int or double
			else {
				isDouble = 0;
				break;
			}
		}
		i++;
	}

	// INT
	if (isInt) {
		isDouble = 0;
		return INT_OP;
	}
	// DOUBLE
	else if (isDouble) {
		// too many decimals, throw error
		if (decimalCount > 1) {
			printf("  -> %s\n\n", operand);
			printf("Syntax error: Too many decimals in type double\n");
			return ERROR_OP;
		} else if (decimalCount < 1) {
			//TODO remove prints
			// if we have a double with no decimals somehow, debug
			printf("Programmer error: Somehow we have no decimals in type double, ");
			printf("check processParseResult()\n");
			return ERROR_OP;
		} else {
			return DOUBLE_OP;
		}
	}
	// operand is either variable, or something else (not expected)
	else {
		// VARIABLE
		DICT_VAR_t* var = findVariable(dict, operand);
		if (var != NULL) {
			return VAR_OP;
		}
		// at this point, treat it as a variable typo
		else {
			printf("  -> %s\n\n", operand);
			printf("Operand Error: Unable to identify the operand/variable\n");
			return ERROR_OP;
		}
	}

	// if we somehow got to the end, return error
	return ERROR_OP;
}

int processListIndex(DICT_t* dict, char* operand, int silence) {
	// TODO improve error catching for improper inputs ()

	// check for char, string, and list
	//  much be at least length 2
	if (strlen(operand) > 1) {
		// INDEX
		if (strchr(operand, '[') != NULL && strchr(operand, ']') != NULL) {
			// parsing the index symbol will have to be done elsewhere
			// for now just make sure its valid and return if valid
			// should only be one of each bracket
			int i = 0, leftCount = 0, rightCount = 0;
			while (operand[i] != '\0') {
				if (operand[i] == '[') {
					leftCount++;
				} else if (operand[i] == ']') {
					rightCount++;
				}
				// right bracket came first, error
				if (rightCount > leftCount) {
					if (silence == 0) {
						printf("  -> %s\n\n", operand);
						printf("Syntax Error: Brackets are out of order\n");
					}
					return -1;
				}
				i++;
			}
			if (leftCount != 1 || rightCount != 1) {
				if (silence == 0) {
					printf("  -> %s\n\n", operand);
					printf("Syntax Error: Too many/too few brackets\n");
				}
				return -1;
			}
			else {
				// pointer for parsing
				char* pch;
				// get variable name
				pch = strtok(operand, "[");
				// make sure variable exists and is a list
				DICT_VAR_t* var = readVariable(dict, pch);
				if (var != NULL) {
					// not a list
					if (var->type != LIST) {
						if (silence == 0) {
							printf("  -> %s\n\n", operand);
							printf("Syntax Error: Variable %s is not a list\n", var->varname);
						}
						return -1;
					}
				}
				// does not exist
				else {
					if (silence == 0) {
						printf("  -> %s\n\n", operand);
						printf("Error: Variable not found in dictionary\n");
					}
					return -1;
				}
				// check that index is int
				pch = strtok(NULL, "]");
				char* pch_iter = pch;
				while (pch_iter != '\0' && (int) *pch_iter != 0) {
					if (!isdigit(*pch_iter)) {
						if (silence == 0) {
							printf("  -> %s\n\n", operand);
							printf("Syntax Error: Non-integer in index\n");
						}
						return -1;
					}
					pch_iter++;
				}

				// check that the index is inbounds
				int index = atoi(pch);
				GENERIC_LIST_t* list = var->element.l;
				if (index > list->size - 1) {
					if (silence == 0) {
						printf("  -> [%i]\n\n", list->size);
						printf("Out of Bounds Error: Index is larger than size of list\n");
					}
					return -1;
				}
				return index;
			}
		}
	}
	return -1;
}


/*
Print the parse result
for debugging and testing
*/
void printResult(PARSE_RESULT_t *result) {
	printf("Print Result ---\n");
	printf("Left Operand: \t%s\n", result->left_operand);
	if (result->right_operand1 != NULL) {
		printf("Right Operand1: \t%s\n", result->right_operand1);
		printf("Right Type1: \t%d\n", result->right_type1);
	}
	if (result->right_operand2 != NULL) {
		printf("Right Operand2: \t%s\n", result->right_operand2);
		printf("Right Type2: \t%d\n", result->right_type2);
	}
	printf("Operation Type: \t%d\n", result->optype);
	printf("\n");
}

/*
Remove the quotations from the outside of the char buffer
*/
void removeQuotes(char* str) {
	char* newStr = (char*) calloc(strlen(str), sizeof(char));
	strcpy(newStr, str);

	for (int i = 1; i < strlen(str)-1; i++) {
		str[i-1] = newStr[i];
	}
	str[strlen(newStr)-2] = '\0';
	free(newStr);
}

/*
Gets index from a list index char*
i.e. list[1]
*/
int parseIndex() {
	return 0;
}
