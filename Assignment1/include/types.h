#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Define list struct here so it can be referenced in element
// struct generic_list;

//Union holds only one of these element types
typedef union element{
	char c;
	signed long int i;
	double d;
	char *s;
	struct generic_list *l;
} ELEMENT_t;

//Use this enum type to specify which element type is being used
typedef enum element_type{
	ERROR, 		//place holder so the enum matches the operand type enum
	CHAR,
	INT,
	DOUBLE,
	STRING,
	LIST
} ELEMENT_TYPE_e;


#endif // TYPES_H
