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
        char* operand = calloc(MAX_VAR_LEN,sizeof(char));
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
        char* operand = calloc(MAX_VAR_LEN,sizeof(char));
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
            switch(rest[i++]){
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
        char* operand = calloc(MAX_VAR_LEN,sizeof(char));
        //Find first operand (strtok_r removes spaces, assignment, and other operators, returning next non-token string
        token = strtok_r(rest, " +-*/=", &rest);
		printf("%s", rest)
        strcpy(operand,token);
        //Modify left_operand to point at operand location
        result->left_operand = operand;
        //Do the same process for the first right operand
        operand = calloc(MAX_VAR_LEN,sizeof(char));
        token = strtok_r(rest, " +-*/=", &rest);
        strcpy(operand,token);
        result->right_operand1 = operand;
        //There may or may not be a third operand
        token = strtok_r(rest, " +-*/=", &rest);
        //Check to see if token is NULL, if not, then assign third operand
        if(token!=NULL){
            //If not NULL, set right operand2 to last token
            operand = calloc(MAX_VAR_LEN,sizeof(char));
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


void getTypes(PARSE_RESULT_t *result) {
  // TODO improve error catching for improper inputs

  
}
