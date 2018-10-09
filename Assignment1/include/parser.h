#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>		// isdigit
#include "types.h"
#include "dictionary.h"

//Enum to hold types of operations
typedef enum optype{
	ADD_OP,
	SUB_OP,
	MULT_OP,
	DIV_OP,
	PRINT_OP,
	APPEND_OP,
	ASSIGN_OP
} OPTYPE_e;

// Enum to hold operand types (OP stands for operand here)
typedef enum operand_type {
	// error bit
	ERROR_OP,	// 0
	// the rest are literal values
	CHAR_OP,	// 1
	INT_OP,		// 2
	DOUBLE_OP,	// 3
	STRING_OP,	// 4
	LIST_OP,	// 5 - dictionary list variable
	VAR_OP		// 6 - dictionary variable
} OPERAND_TYPE_e;

//Result structure to hold operands and operation type
typedef struct parse_result{
	char* left_operand;
	char* right_operand1;
	char* right_operand2;
	OPERAND_TYPE_e right_type1;
	OPERAND_TYPE_e right_type2;
	OPTYPE_e optype;
} PARSE_RESULT_t;

//Command Types
#define PRINT_COMMAND "print("
#define APPEND_COMMAND "append("
//Max length of string input, and max length of variable
#define MAX_BUFFER_LEN 100+1
#define MAX_OPERAND_LEN 52+1

// parse_string function
PARSE_RESULT_t* parse_string(const char *input_buffer);
void processParseResult(DICT_t* dict, PARSE_RESULT_t* result);
void processOperand(DICT_t* dict, PARSE_RESULT_t* result, char* operand, int whichOper);
void printResult(PARSE_RESULT_t* result);

// util
char* removeQuotes(char* str);

#endif // PARSER_H
