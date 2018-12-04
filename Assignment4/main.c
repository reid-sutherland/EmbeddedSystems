#include <stdlib.h>		// malloc/calloc
#include <stdio.h>
#include <stdint.h>		// include uint8_t typedefs, not needed for arduino
#include <ctype.h>		// isspace(), tolower()
#include <string.h>
#include "structs.h"
// #include "functions.h"

#define MAX_BUFFER_LENGTH 128+1		// Max buffer length
#define NUM_TASKS 4		// Number of tasks

// Menus
const char menuMain[] = "Main Menu\nArm Security System\nTest Light Sensor\nTest Alarm\nSet Light Sensor Threshold\nSet Keycode\n";

// Function Headers
void DisplayMenu(const char menu[]);

// Global variables
char readBuffer[MAX_BUFFER_LENGTH];		// user input buffer for serial
task_t tasks[NUM_TASKS];		// Tasks array


// Arduino variables


int main() {


	return 0;
}


// *** Display Menu ***
void DisplayMenu(const char menu[]) {
	// create a new char array for tokenizing the menu string
	char* menuCopy = (char*)calloc(strlen(menu), sizeof(char));
	strcpy(menuCopy, menu);
	char* token;

	// Print menu
	// print menu title
	printf("\n==========");
	token = strtok(menuCopy, "\n");
	printf("%s", token);
	printf("==========\n");
	int numEquals = strlen(token) + 20
	;
	// print each list element
	int optCount = 0;
	token = strtok(NULL, "\n");
	while (token != NULL) {
		optCount++;
		printf("%d:  %s\n", optCount, token);
		// get the next option if it exists
		token = strtok(NULL, "\n");
	}
	// print end of menu
	for (int i = 0; i < numEquals; i++)
		printf("=");
	printf("\n");
}
