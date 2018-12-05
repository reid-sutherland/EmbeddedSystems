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
void (*TaskFuncs[NUM_TASKS])(void *);

// Struct pointers
sensor_t* sensor_data;
alarm_t* alarm_data;
read_t* read_data;
check_t* check_data;

// Global variables
char readBuffer[MAX_BUFFER_LENGTH];		// user input buffer for serial
int taskIter = 1;		// which task is next


// Arduino variables



int main() {

	// setup
	{
		// initialize task list
		int i = 0;
		TaskFuncs[i] = &TaskFunc_Sensor;
		i++;
		TaskFuncs[i] = &TaskFunc_Alarm;
		i++;
		TaskFuncs[i] = &TaskFunc_Read;
		i++;
		TaskFuncs[i] = &TaskFunc_Check;
		i++;

		// initialize struct pointers
		sensor_data = (sensor_t*) calloc(1, sizeof(sensor_t));
		alarm_data = (alarm_t*) calloc(1, sizeof(alarm_t));
		read_data = (read_t*) calloc(1, sizeof(read_t));
		check_data = (check_t*) calloc(1, sizeof(check_t));

		// Initialize sensor
		sensor_data->armed = calloc(1, sizeof(int));
		*sensor_data->armed = 0;	// initially unarmed
		sensor_data->threshold = calloc(1, sizeof(int));
		//TODO initialize Threshold
		//TODO initialize sensor value
		//TODO initialize thresh flag


		// Initialize alarm
		alarm_data->armed = sensor_data->armed;
		alarm_data->threshold = sensor_data->threshold;
		// TODO initialize alarm stuff

		// Initialize read_serial
		read_serial->bufferPtr = readBuffer;
		read_serial->index = 0;
		read_serial->newlineFlag = 0;

		printMenu();
	}
	exit(0);

	// loop
	while (1)
	{
		switch (taskIter) {
			case 0:
				TaskFuncs[taskIter](sensor_data);
				break;

			case 1:
				TaskFuncs[taskIter](alarm_data);
				break;

			case 2:
				TaskFuncs[taskIter](read_data);
				break;

			case 3:
				TaskFuncs[taskIter](check_data);
				break;

			default:
				break;	// error, do nothing
		}
		// increment (or reset) task iterator
		if (++taskIter > 3)
			taskIter = 0;
	}

	// free memory
	free(sensor_data->threshold);
	free(sensor_data->armed);
	free(sensor_data);
	free(alarm_data);
	free(read_data);
	free(check_data);

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
	sensor_t* sensor_s = (sensor_t*) structPtr;

	// if not testing sensor or if system not armed, stop
	if
}

void TaskFunc_Alarm(void * structPtr) {
	alarm_t* alarm_s = (alarm_t*) structPtr;
}

void TaskFunc_Read(void * structPtr) {
	read_t* read_s = (read_t*) structPtr;
}

void TaskFunc_Check(void * structPtr) {
	check_t* check_s = (check_t*) structPtr;
}
