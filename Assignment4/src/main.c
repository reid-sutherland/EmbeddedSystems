#include <stdlib.h>		// malloc/calloc
#include <stdio.h>
#include <stdint.h>		// include uint8_t typedefs, not needed for arduino
#include <ctype.h>		// isspace(), tolower()
#include <string.h>
#include "../include/structs.h"
// #include "functions.h"

#define MAX_BUFFER_LENGTH 128+1		// Max buffer length
#define NUM_TASKS 4		// Number of tasks

// Task Function Headers
void TaskFunc_Sensor(void*);
void TaskFunc_Alarm(void*);
void TaskFunc_Read(void*);
void TaskFunc_Check(void*);
// Function Headers
void PrintMenu();

// Task Control Block

// Struct pointers
sensor_t* sensor_data;
alarm_t* alarm_data;
read_t* read_data;
check_t* check_data;

// Global variables
char readBuffer[MAX_BUFFER_LENGTH];		// user input buffer for serial
int currentTask = 1;		// which task is next


// Arduino variables



int main() {

	// setup
	{
		// initialize task list
		int i = 0;
		tasks[i].TaskFunc = &TaskFunc_Sensor;
		i++;
		tasks[i].TaskFunc = &TaskFunc_Alarm;
		i++;
		tasks[i].TaskFunc = &TaskFunc_Read;
		i++;
		tasks[i].TaskFunc = &TaskFunc_Check;
		i++;

		// initialize struct pointers
		sensor_data = (sensor_t*) calloc(1, sizeof(sensor_t));
		alarm_data = (alarm_t*) calloc(1, sizeof(alarm_t));
		read_data = (read_t*) calloc(1, sizeof(read_t));
		check_data = (check_t*) calloc(1, sizeof(check_t));

		// link sensor threshold to alarm threshold
		sensor_data->threshold = calloc(1, sizeof(int));
		alarm_data->threshold = sensor_data->threshold;


	}
	exit(0);

	// loop
	while (1)
	{
		switch (currentTask) {
			case 1:
				break;

			case 2:

				break;

			case 3:

				break;

			case 4:

				break;

			default:
				break;	// error, do nothing
		}
		// increment (or reset) task iterator
		if (++currentTask > 4)
			currentTask = 1;
	}


	return 0;
}


// Print the menu
void PrintMenu() {
	printf("==========Main Menu==========\n");
	printf("1)  Arm Security System\n");
	printf("2)  Test Light Sensor\n");
	printf("3)  Test Alarm\n");
	printf("4)  Set Light Sensor Threshold\n");
	printf("5)  Set Keycode\n");
	printf("=============================\n");
}


// ****************
// Task Functions
// ****************
void TaskFunc_Sensor(void * structPtr) {

}

void TaskFunc_Alarm(void * structPtr) {

}

void TaskFunc_Read(void * structPtr) {

}

void TaskFunc_Check(void * structPtr) {

}
