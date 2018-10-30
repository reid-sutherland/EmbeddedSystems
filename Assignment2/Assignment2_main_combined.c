//#include "Assignment2_global.h"		// global also includes rims.h and secret_code.h
//#include "Assignment2_debounce.h"
//#include "Assignment2_waitFlash.h"
//#include "Assignment2_gameOverFlash.h"
#include "rims.h"

#define SECRET_CODE 69

// I was GOING to write all of this code from scratch, but since
//  I'll be testing with RIMS, I suppose I need to follow their format to
//  be safe.. Oh well!

// TODO: to myself - if errors, abandon my enum implementation and
//  revert to RIBS' implementation for managing concurrent states
// TODO: specify active high/low button functionality in report!!

/* GLOBAL Function Headers */
int checkGuess();
unsigned char getNumIncorrectBits();
unsigned char getBit(unsigned char x, unsigned char k);
void resetD();
unsigned char getD();
void decrementD();

/* GLOBAL VARIABLES HERE */
unsigned char secret_code = SECRET_CODE;		// THE secret code to be guessed
int winCondition;		// high if game ends in win, low for lose
int correctGuess;		// high when submitted guess is correct
int btnPressed;		// high when guess submission button is pressed
int WF_enable;		// enable the wait flash cycle on B
int GOF_enable;		// enable the game over flash pattern for end game


// Task Struct
typedef struct task {
   int state;
   unsigned long period;
   unsigned long elapsedTime;
   unsigned char flag;
   int (*TickFct)(int);
} task;
// Tasks Array
task tasks[4];
// Periods
const unsigned char tasksNum = 4;
const unsigned long Debounce_Period = 50;
const unsigned long WaitFlash_Period = 50;
const unsigned long GameOverFlash_Period = 50;
const unsigned long Game_Period = 100;
// Period GCD
const unsigned long tasksPeriodGCD = 50;
// Tick Function Declarations
int TickFunc_Debounce(int state);
int TickFunc_GameOverFlash(int state);
int TickFunc_WaitFlash(int state);
int TickFunc_Game(int state);

unsigned char processingRdyTasks = 0;
void TimerISR() {
   unsigned char i;
   if (processingRdyTasks) {
      printf("Period too short to complete tasks\n");
   }
   processingRdyTasks = 1;
   for (i = 0; i < tasksNum; ++i) { // Heart of scheduler code
      if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
		 tasks[i].flag = 1;
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
   tasks[i].flag = 0;
   tasks[i].TickFct = &TickFunc_Debounce;

   ++i;
   tasks[i].state = -1;
   tasks[i].period = WaitFlash_Period;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].flag = 0;
   tasks[i].TickFct = &TickFunc_WaitFlash;

   ++i;
   tasks[i].state = -1;
   tasks[i].period = GameOverFlash_Period;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].flag = 0;
   tasks[i].TickFct = &TickFunc_GameOverFlash;

   ++i;
   tasks[i].state = -1;
   tasks[i].period = Game_Period;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].flag = 0;
   tasks[i].TickFct = &TickFunc_Game;

   ++i;
   TimerSet(tasksPeriodGCD);
   TimerOn();

   while(1) { 
	for (i = 0; i < tasksNum; ++i) { // Heart of scheduler code
      if ( tasks[i].flag ) { // Ready
         tasks[i].state = tasks[i].TickFct(tasks[i].state);
		 tasks[i].flag = 0;
      }
	}
   }

   return 0;
}


// Main Game SM Tick Function
enum Game_States { G_newGame, G_startRound, G_checkGuess, G_incorrectGuess, 
                    G_win, G_lose, G_newGameWait } Game_State;
int TickFunc_Game(int state) {
	// SM Variables
	static int newFlashCount;		// counts the flashes that signal new game
	static int waitTimer;			// timer for waiting after guesses

	// Transitions
	switch (state) {
		case -1:
			B = 0;
			newFlashCount = 0;
			state = G_newGame;
			break;
		case G_newGame:
			if (newFlashCount < 6) {
				if (B == 0) {
					B = 255;
				}
				else if (B == 255) {
					B = 0;
				}
				newFlashCount++;
				state = G_newGame;
			}
			else if (newFlashCount >= 6) {
				resetD();
				B = 0;
				WF_enable = 1;
				state = G_startRound;
			}
			break;

		case G_startRound:
			if (!btnPressed) {
				state = G_startRound;
			}
			else if (btnPressed) {
				WF_enable = 0;
				state = G_checkGuess;
			}
			break;

		case G_checkGuess:
			if (correctGuess) {
				winCondition = 1;
				GOF_enable = 1;
				state = G_win;
			}
			else if (!correctGuess && getD() <= 1) {
				decrementD();
				winCondition = 0;
				GOF_enable = 1;
				state = G_lose;
			}
			else if (!correctGuess && getD() > 1) {
				B = getNumIncorrectBits();
				state = G_incorrectGuess;
			}
			break;

		case G_incorrectGuess:
			if (waitTimer < 20) {
				waitTimer++;
				state = G_incorrectGuess;
			}
			else if (waitTimer >= 20) {
				decrementD();
				B = 0;
				WF_enable	= 1;
				state = G_startRound;
			}
			break;

		case G_win:
			if (GOF_enable) {
				state = G_win;
			}
			else if (!GOF_enable) {
				state = G_newGameWait;
			}
			break;

		case G_lose:
			if (GOF_enable) {
				state = G_lose;
			}
			else if (!GOF_enable) {
				state = G_newGameWait;
			}
			break;

		case G_newGameWait:
			if (waitTimer < 20) {
				waitTimer++;
				state = G_newGameWait;
			}
			else if (waitTimer >= 20) {
				B = 0;
				newFlashCount = 0;
				state = G_newGame;
			}
			break;

		default:
			state = -1;
			break;
	}

	// Actions
	switch (state) {
		case G_newGame:
			break;
		case G_startRound:
			break;
		case G_checkGuess:
			checkGuess();
			btnPressed = 0;
			waitTimer = 0;
			break;
		case G_incorrectGuess:
			break;
		case G_win:
			break;
		case G_lose:
			break;
		case G_newGameWait:
			break;
		default:
			break;
	}
	Game_State = state;
	return state;
}


// Debounce SM Tick Function
enum Debounce_States { D_offLow, D_offHigh, D_onHigh, D_onLow } Debounce_State;
int TickFunc_Debounce(int state) {
	// SM Variables
	static int debTimer;

	// Transitions
	switch(state) {
		case -1:
			state = D_offLow;
			btnPressed = 0;
			break;
		case D_offLow:
			if (!C0) {
				debTimer = 0;
				state = D_offLow;
			}
			else if (C0) {
				debTimer++;
				state = D_offHigh;
			}
			break;
		case D_offHigh:
			if (C0 && debTimer <= 4) {
				debTimer++;
				state = D_offHigh;
			}
			else if (!C0) {
				debTimer = 0;
				state = D_offLow;
			}
			else if (C0 && debTimer > 4) {
				btnPressed = 1;
				debTimer = 0;
				state = D_onHigh;
			}
			break;
		case D_onHigh:
			if (C0) {
				debTimer = 0;
				state = D_onHigh;
			}
			else if (!C0) {
				debTimer++;
				state = D_onLow;
			}
			break;
		case D_onLow:
			if (!C0 && debTimer <= 4) {
				debTimer++;
				state = D_onLow;
			}
			else if (C0) {
				debTimer = 0;
				state = D_onHigh;
			}
			else if (!C0 && debTimer > 4) {
				btnPressed = 0;
				debTimer = 0;
				state = D_offLow;
			}
			break;
		default:
			debTimer = 0;
			state = D_offLow;
			break;
	}

	// Actions
	switch (state) {
		// This is a Mealy-only machine, so no state actions
		default:
			break;
	}
	Debounce_State = state;
	return state;
}


// Wait Flash SM Tick Function
enum WaitFlash_States { WF_S0, WF_S1, WF_S2, WF_S3, WF_S4, 
                         WF_S5, WF_S6, WF_S7, WF_S8, WF_S9 } WaitFlash_State;
int TickFunc_WaitFlash(int state) {

	// Transitions
	switch(state) {
		case -1:
			WF_enable = 0;
			state = WF_S0;
			break;
		case WF_S0:
			if (WF_enable) {
				B = 0;
				state = WF_S1;
			}
			else if (!WF_enable) {
				state = WF_S0;
			}
			break;
		case WF_S1:
			if (!WF_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 1;
				state = WF_S2;
			}
			break;
		case WF_S2:
			if (!WF_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 2;
				state = WF_S3;
			}
			break;
		case WF_S3:
			if (!WF_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 4;
				state = WF_S4;
			}
			break;
		case WF_S4:
			if (!WF_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 8;
				state = WF_S5;
			}
			break;
		case WF_S5:
			if (!WF_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 16;
				state = WF_S6;
			}
			break;
		case WF_S6:
			if (!WF_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 32;
				state = WF_S7;
			}
			break;
		case WF_S7:
			if (!WF_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 64;
				state = WF_S8;
			}
			break;
		case WF_S8:
			if (!WF_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 128;
				state = WF_S9;
			}
			break;
		case WF_S9:
			B = 0;
			state = WF_S0;
			break;
		default:
			state = WF_S0;
			break;
	}

	// Actions
	switch (state) {
		// This is a Mealy-only machine, so no state actions
		default:
			break;
	}
	WaitFlash_State = state;
	return state;
}


// Game Over Flash SM Tick Function
enum GameOverFlash_States { GOF_idle, GOF_flashOff, GOF_flashOn, GOF_waitOff, GOF_waitOn } GameOverFlash_State;
int TickFunc_GameOverFlash(int state) {
	// SM variables
	static int flashCount;
	
	// Transitions
	switch (state) {
		case -1:
			GOF_enable = 0;
			state = GOF_idle;
			break;
		case GOF_idle:
			if (!GOF_enable) {
				state = GOF_idle;
			}
			else if (GOF_enable) {
				B = 0;
				flashCount = 0;
				state = GOF_flashOn;
			}
			break;
		case GOF_flashOn:
			flashCount++;
			state = GOF_waitOn;
			break;
		case GOF_waitOn:
			state = GOF_flashOff;
			break;
		case GOF_flashOff:
			if (flashCount < 5) {
				state = GOF_waitOff;
			}
			else if (flashCount >= 5) {
				GOF_enable = 0;
				state = GOF_idle;
			}
			break;
		case GOF_waitOff:
			state = GOF_flashOn;
			break;
		default:
			state = GOF_idle;
			break;
	}

	// Actions
	switch (state) {
		case GOF_idle:
			break;
		case GOF_flashOn:
			if (winCondition) {
				B = 15;
			}
			else {
				B = 240;
			}
			break;
		case GOF_waitOn:
			break;
		case GOF_flashOff:
			B = 0;
			break;
		case GOF_waitOff:
			break;
		default:
			break;
	}
	GameOverFlash_State = state;
	return state;
}



/* GLOBAL FUNCTIONS HERE */

// Checks the guess on A for correctness with the secret code
// Returns 0 for incorrect, 1 for correct
int checkGuess() {
	if (A == secret_code) {
		correctGuess = 1;
		return 1;
	}
	// return lose if A does not match secret code
	correctGuess = 0;
	return 0;
}

// Returns the number of incorrect bits on A
unsigned char getNumIncorrectBits() {
	unsigned char num = 0;
	// compute A XOR secret_code
	unsigned char xor_result = A ^ secret_code;
	// count number of incorrect bits - each 1 is a mismatched bit
    int i;
	for (i = 0; i < 8; i++) {
		if (getBit(xor_result, i) == 1) {
			num++;
		}
	}
	return num;
}

// getBit function - borrowed from ZyBooks
unsigned char getBit(unsigned char x, unsigned char k) {
   return ((x & (0x01 << k)) != 0);
}

// *** I am bad at pin math, so I am essentially taking some shortcuts to overcome
//  the D pin bug
// Resets D back to 10 for a new game
void resetD() {
	// set D = 10, bit by boring bit
	D0 = 0;
	D1 = 1;
	D2 = 0;
	D3 = 1;
	D4 = 0;
	D5 = 0;
	D6 = 0;
	//D7 = 0;
}

// Returns the value of D as an integer
unsigned char getD() {
	unsigned char Dval = 0;

	Dval += (D0 * 1);
	Dval += (D1 * 2);
	Dval += (D2 * 4);
	Dval += (D3 * 8);
	Dval += (D4 * 16);
	Dval += (D5 * 32);
	Dval += (D6 * 64);
	//Dval += (D7 * 128);

	return Dval;
}

// Decrements D, 1 bit at a time
void decrementD() {
	// if D = 0, stop
	if (!D0 && !D1 && !D2 && !D3 && !D4 && !D5 && !D6) {
		return;
	}

	// starting with least significant bit, flip each 0 bit to 1 until you get
	//  to the first 1. Flip that bit, then stop
	if (D0 == 0) {
		D0 = 1;		// flip each low bit
	}
	else if (D0 == 1) {
		D0 = 0;		// flip the first high bit, then stop
		return;
	}

	if (D1 == 0) {
		D1 = 1;		// flip each low bit
	}
	else if (D1 == 1) {
		D1 = 0;		// flip the first high bit, then stop
		return;
	}

	if (D2 == 0) {
		D2 = 1;		// flip each low bit
	}
	else if (D2 == 1) {
		D2 = 0;		// flip the first high bit, then stop
		return;
	}

	if (D3 == 0) {
		D3 = 1;		// flip each low bit
	}
	else if (D3 == 1) {
		D3 = 0;		// flip the first high bit, then stop
		return;
	}

	if (D4 == 0) {
		D4 = 1;		// flip each low bit
	}
	else if (D4 == 1) {
		D4 = 0;		// flip the first high bit, then stop
		return;
	}

	if (D5 == 0) {
		D5 = 1;		// flip each low bit
	}
	else if (D5 == 1) {
		D5 = 0;		// flip the first high bit, then stop
		return;
	}

	if (D6 == 0) {
		D6 = 1;		// flip each low bit
	}
	else if (D6 == 1) {
		D6 = 0;		// flip the first high bit, then stop
		return;
	}

	//if (D7 == 0) {
	//	D7 = 1;		// flip each low bit
	//}
	//else if (D7 == 1) {
	//	D7 = 0;		// flip the first high bit, then stop
	//	return;
	//}

	return;
}
