#include "global.h"		// global also includes rims.h and secret_code.h
#include "debounce.h"
#include "waitFlash.h"
#include "gameOverFlash.h"

// I was GOING to write all of this code from scratch, but since
//  I'll be testing with RIMS, I suppose I need to follow their format to
//  be safe.. Oh well!

// TODO: to myself - if errors, abandon my enum implementation and
//  revert to RIBS' implementation for managing concurrent states
// TODO: specify active high/low button functionality in report!!

// Task Struct
typedef struct task {
   int state;
   unsigned long period;
   unsigned long elapsedTime;
   int (*TickFct)(int);
} task;
// Tasks Array
task tasks[4];
// Periods
const unsigned char tasksNum = 4;
const unsigned long Debounce_Period = 20;
const unsigned long WaitFlash_Period = 50;
const unsigned long GameOverFlash_Period = 50;
const unsigned long Game_Period = 100;
// Period GCD
const unsigned long tasksPeriodGCD = 10;
// Tick Function Declarations
int TickFunc_Game(int p_state);

// Secret Code variable
const int secret_code = SECRET_CODE;

unsigned char processingRdyTasks = 0;
void TimerISR() {
   unsigned char i;
   if (processingRdyTasks) {int
      printf("Period too short to complete tasks\n");
   }
   processingRdyTasks = 1;
   for (i = 0; i < tasksNum; ++i) { // Heart of scheduler code
      if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
         tasks[i].state = tasks[i].TickFct(tasks[i].state);
         tasks[i].elapsedTime = 0;
      }
      tasks[i].elapsedTime += tasksPeriodGCD;
   }
   processingRdyTasks = 0;
}
int main() {
   // Priority assigned to lower position tasks in array
   unsigned char i=0;
   tasks[i].state = -1;
   tasks[i].period = Debounce_Period;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct = &TickFunc_Debounce;

   ++i;
   tasks[i].state = -1;
   tasks[i].period = WaitFlash_Period;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct = &TickFunc_WaitFlash;

   ++i;
   tasks[i].state = -1;
   tasks[i].period = GameOverFlash_Period;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct = &TickFunc_GameOverFlash;

   ++i;
   tasks[i].state = -1;
   tasks[i].period = Game_Period;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct = &TickFunc_Game;

   ++i;
   TimerSet(tasksPeriodGCD);
   TimerOn();

   while(1) { Sleep(); }

   return 0;
}



int TickFunc_Game(int p_state) {
	// SM Variables
	static enum States { G_newGame,  } state;
	static int secret_code = SECRET_CODE;
	static int newFlashCount;		// counts the flashes that signal new game
	static int waitTimer;			// timer for waiting after guesses

	state = p_state;




	return state;
}




// unsigned char WaitFlash_Clk;
// unsigned char GameOverFlash_Clk;
// unsigned char Debounce_Clk;
// unsigned char Game_Clk;
//
// // Timer function
// void TimerISR() {
//
// }
//
// int main() {
//
// 	const unsigned int WaitFlash_Period = 50;
// 	const unsigned int GameOverFlash_Period = 50;
// 	const unsigned int Debounce_Period = 20;
// 	const unsigned int Game_Period = 100;
//
// 	// Skip the math here, but should come out to a universal period of 10 ms
// 	const unsigned int Concurrent_Period = 10;
// 	TimerSet(Concurrent_Period);
// 	TimerOn();
//
// 	SM1_State = -1; // Initial state
// 	B = 0; // Init outputs
//
// 	while(1) {
// 		TickFct_State_machine_1();
// 		while(!SM1_Clk);
// 		SM1_Clk = 0;
// 	} // while (1)
// } // Main
