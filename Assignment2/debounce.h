#include "global.h"
#include "rims.h"


int TickFunc_Debounce(int p_state) {
	// SM Variables
	static enum States { D_offLow, D_offHigh, D_onHigh, D_onLow } state;
	static int debTimer;

	state = p_state;

	// Transitions
	switch(state) {
		case -1:
			state = D_offLow;
			btnPressed = 0;
			break;
		case D_offLow;
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
			btnPressed = 0;
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

	return state;
}
