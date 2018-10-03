#include <stdio.h>
#include <time.h>
#include "../include/linkedList.h"
#include "../include/parser.h"

// function headers
void mathOp(PARSE_RESULT_t* parse_result, OPTYPE_e);
void testParser();


// global pointer to main dictionary
DICT_t * DICT_MAIN;


int main() {

	// SETUP
	char input_buffer[101];


	// LOOP
	while (1) {

		// print >>>
		printf(">>> ");

		// read input
		scanf("%100s", input_buffer);

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
				break;

			case ASSIGN_OP:
				// ensure there is something to assign
				if (result->right_operand1 != NULL) {
					printf("Error: Nothing to assign\n");
					printf("---- Right operand1 null\n");
					printf("main result->optype switch\n");
					break;
				}

				// parse the parse result
				const char* varname = result->left_operand;
				ELEMENT_t element;

				// assignment successful
				if (writeVariable(DICT_MAIN, result->left_operand, , ))


				break;

			case ADD_OP:
				mathOp(result, ADD_OP);

				break;

			case SUB_OP:
				mathOp(result, SUB_OP);
				break;

			case MULT_OP:
				mathOp(result, MULT_OP);
				break;

			case DIV_OP:
				mathOp(result, DIV_OP);
				break;

			default:
				break;
		}

		// printf("%s\n", input_buffer);
	}


	return 0;
}


void mathOp(PARSE_RESULT_t* parse_result, OPTYPE_e op) {
	printf("MathOp\n");


	switch (op) {
		case ADD_OP:
		 	break;

		case SUB_OP:
			break;

		case MULT_OP:
			break;

		case DIV_OP:
			break;
	 }
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
