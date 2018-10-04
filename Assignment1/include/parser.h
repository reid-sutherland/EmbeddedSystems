#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include <stdlib.h>
#include <cytpe.h>		// isdigit
#include "types.h"

//Enum to hold types of operations
typedef enum optype{ADD_OP,SUB_OP,MULT_OP,DIV_OP,PRINT_OP,APPEND_OP,ASSIGN_OP} OPTYPE_e;

// Enum to hold operand types (OP stands for operand here)
typedef enum operand_type {
	// a dictionary variable
	VAR_OP,
	// the rest are literal values
	CHAR_OP,
	INT_OP,
	DOUBLE_OP,
	STRING_OP
	// TODO add list
	// , LIST_OP
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
int processParseResult(PARSE_RESULT_t *result);

#endif // PARSER_H
