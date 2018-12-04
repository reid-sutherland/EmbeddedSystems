// Periods
const unsigned long Game_Period = 100;
// Period GCD
const unsigned long tasksPeriodGCD = 50;

// Task Control Block
typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	unsigned char flag;
	void (*TaskFunc)(void *);
} task_t;

// Structs for task functions
typedef struct light_sensor {
	int* threshold;
	int sensorValue;
	int threshFlag;		// 1 when threshold is broken, 0 otherwise
} sensor_t;

typedef struct alarm_state {
	int halfPeriodMicroseconds;
	int numIterations;		// number of iterations remaining of the current note
	int note;		// current note
} alarm_state_t;

typedef struct alarm {
	alarm_state_t prevState;	// previous state of alarm
	int* threshold;		// threshold for the light sensor
} alarm_t;

typedef struct read_serial {
	char* bufferPtr;
	int index;
	int newlineFlag;		// high if most recent value was newline character
} read_t;

typedef struct check_serial {
	int input;		// either menu choice, or keycode for disarming
	int ret;		// return input value ~~ -1 means invalid input
} check_t;
