#include "../include/parser.h"

/*
parse_string function
Input -- Input buffer from string to be parsed
Output -- PARSE_RESULT_t pointer holding strings of operands, and an operation type
PARSE_RESULT_t will have pointers to strings representing operands that will need to be freed
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
        token = strtok_r(rest, " +-*/=", &rest);
        strcpy(operand,token);
        //Modify left_operand to point at operand location
        result->left_operand = operand;
        //Do the same process for the first right operand
		// Don't assume there are 2 operands
        token = strtok_r(rest, " +-*/=", &rest);
		if (token != NULL) {
			operand = calloc(MAX_OPERAND_LEN,sizeof(char));
			strcpy(operand,token);
	        result->right_operand1 = operand;
		} else {
			result->right_operand1 = NULL;
		}
        //There may or may not be a third operand
        token = strtok_r(rest, " +-*/=", &rest);
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

	// don't process (or assign) a null operand
	if (result->right_operand1 != NULL) {
		int whichOper = 1;
		processOperand(dict, result, result->right_operand1, whichOper);
	}
	if (result->right_operand2 != NULL) {
		int whichOper = 2;
		processOperand(dict, result, result->right_operand2, whichOper);
	}
}

void processOperand(DICT_t* dict, PARSE_RESULT_t* result, char* operand, int whichOper) {
	// TODO improve error catching for improper inputs ()
	// TODO check for inputs like 'ex'ample' and "ex"ample" and throw error

	// operand type
	OPERAND_TYPE_e oper_type;

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
			}
		}
		i++;
	}

	// int
	if (isInt) {
		isDouble = 0;
		oper_type = INT_OP;
	}

	// double
	else if (isDouble) {
		// too many decimals, throw error
		if (decimalCount > 1) {
			printf("Syntax error: Too many decimals in type double\n");
			printf("%s\n", operand);
			oper_type = ERROR_OP;
			return;
		} else if (decimalCount < 1) {
			//TODO remove prints
			// if we have a double with no decimals somehow, debug
			printf("Programmer error: Somehow we have no decimals in type double, ");
			printf("check processParseResult()\n");
			oper_type = ERROR_OP;
			return;
		} else {
			oper_type = DOUBLE_OP;
		}
	}

	// char
	else if (operand[0] == '\'' && operand[strlen(operand)-1] == '\'') {

		// char operand ('a') should only be length 3
		if (strlen(operand) == 3) {
			oper_type = CHAR_OP;
		}
		else if (strlen(operand) > 3) {
			printf("Operand Error: char should be a single ASCII character\n");
			printf("  -> %s\n", operand);

			oper_type = ERROR_OP;
			return;
		}
		// TODO optionally replace error with this if Nelson approves
		// treat 'example' as a string for convenience
		// else if (strlen(operand) > 3) {
		// 	oper_type = STRING_OP;
		// }
	}

	// string
	else if (operand[0] == '\"' && operand[strlen(operand)-1] == '\"') {
		oper_type = STRING_OP;
		// enforce 50 character string limit
		if (strlen(operand) > 52) {	// 50 + "" = 52
			// terminate 53rd char with null character
			operand[51] = '\"';
			operand[52] = '\0';
		}
	}

	// operand is either variable, list, or something else (not expected)
	else {
		// variable
		DICT_VAR_t* var = findVariable(dict, operand);
		if (var != NULL) {
			oper_type = VAR_OP;
		}

		// TODO add list

		// at this point, treat it as a variable typo
		else {
			printf("Operand Error: Unable to identify the operand/variable\n");
			printf("  -> %s\n", operand);
			oper_type = ERROR_OP;
			return;
		}
	}

	// assign the oper_type to the appropriate struct value
	if (whichOper == 1) {
		result->right_type1 = oper_type;
	} else if (whichOper == 2) {
		result->right_type2 = oper_type;
	}
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


char* removeQuotes(char* str) {
	char* newStr = (char*) calloc(strlen(str), sizeof(char));

	for (int i = 1; i < strlen(str) - 1; i++) {
		newStr[i-1] = str[i];
	}
	newStr[strlen(str) - 1] = '\0';

	return newStr;
}
