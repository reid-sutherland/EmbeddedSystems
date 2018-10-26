#include "global.h"


int TickFunc_GameOverFlash(int p_state) {
	// SM variables
	static enum States { GOF_idle, GOF_flashOff, GOF_flashOn, GOF_waitOff, GOF_waitOn } state;
	static int flashCount;

	state = p_state;

	// Transitions
	switch (state) {
		case -1:
			state = GOF_idle;
			break;
		case GOF_idle:
			if (!game_over_flash_enable) {
				state = GOF_idle;
			}
			else if (game_over_flash_enable) {
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
			else if (flasCount >= 5) {
				game_over_flash_enable = 0;
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

	return state;
}
