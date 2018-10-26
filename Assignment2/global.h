#ifndef GLOBAL_H
#define GLOBAL_H		// Header Guarding

#include "rims.h"
#include "secret_code.h"

/* GLOBAL VARIABLES HERE */
int secret_code = SECRET_CODE;		// THE secret code to be guessed
int winCondition;		// high if game ends in win, low for lose
int btnPressed;			// high when guess submission button is pressed

int WF_enable;	// enable the wait flash cycle on B
int GOF_enable;		// enable the game over flash pattern for end game


/* GLOBAL FUNCTIONS HERE */

// Checks the guess on A for correctness with the secret code
// Returns 0 for incorrect, 1 for correct
int checkGuess() {
	if (A == secret_code) {

	}
}

// Returns the number of incorrect bits on A
int getNumIncorrectBits() {

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
	D7 = 0;
}

// Returns the value of D as an integer
int getD() {
	unsigned int Dval = 0;

	Dval += ((int)D0 * 1);
	Dval += ((int)D1 * 2);
	Dval += ((int)D2 * 4);
	Dval += ((int)D3 * 8);
	Dval += ((int)D4 * 16);
	Dval += ((int)D5 * 32);
	Dval += ((int)D6 * 64);
	Dval += ((int)D7 * 128);

	return Dval;
}

// Decrements D, 1 bit at a time
void decrementD() {
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
	}int
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

	if (D7 == 0) {
		D7 = 1;		// flip each low bit
	}
	else if (D7 == 1) {
		D7 = 0;		// flip the first high bit, then stop
		return;
	}

	return;
}



#endif // GLOBAL_H
