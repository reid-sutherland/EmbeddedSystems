#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include <stdlib.h>
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
	ERROR,		// 0
	// a dictionary variable
	VAR_OP,		// 1
	// the rest are literal values
	CHAR_OP,	// 2
	INT_OP,		// 3
	DOUBLE_OP,	// 4
	STRING_OP,	// 5
	LIST_OP		// 6
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
#define MAX_VAR_LEN 15+1

// parse_string function
PARSE_RESULT_t* parse_string(const char *input_buffer);
void processParseResult(DICT_t* dict, PARSE_RESULT_t *result);
void processOperand(DICT_t* dict, char* operand);
void printResult(PARSE_RESULT_t *result);

#endif // PARSER_H
