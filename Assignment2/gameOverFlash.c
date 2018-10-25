#include "gameOverFlash.h"

// static enum States { GOF_idle, GOF_flashOff, GOF_flashOn, GOF_waitOff, GOF_waitOn } states;

void TickFunc_GameOverFlash() {
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
				state = GOF_flashOn;


			}
			break;
		case GOF_flashOn:

			break;
		case GOF_waitOn:

			break;
		case GOF_flashOff:

			break;
		case GOF_waitOff:

			break;
		default:
			state = GOF_idle;
			break;
	}
}
