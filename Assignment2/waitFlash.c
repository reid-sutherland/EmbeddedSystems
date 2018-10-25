#include "waitFlash.h"

void TickFunc_WaitFlash() {
	// Transitions
	switch(state) {
		case -1:
			state = WF_S0;
			break;
		case WF_S0:
			if (wait_flash_enable) {
				B = 0;
				state = WF_S1;
			}
			else if (!wait_flash_enable) {
				state = WF_S0;
			}
			break;
		case WF_S1:
			if (!wait_flash_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 1;
				state = WF_S2;
			}
			break;
		case WF_S2:
			if (!wait_flash_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 2;
				state = WF_S3;
			}
			break;
		case WF_S3:
			if (!wait_flash_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 4;
				state = WF_S4;
			}
			break;
		case WF_S4:
			if (!wait_flash_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 8;
				state = WF_S5;
			}
			break;
		case WF_S5:
			if (!wait_flash_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 16;
				state = WF_S6;
			}
			break;
		case WF_S6:
			if (!wait_flash_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 32;
				state = WF_S7
			}
		case WF_S7:
			if (!wait_flash_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 64;
				state = WF_S8;
			}
			break;
		case WF_S8:
			if (!wait_flash_enable) {
				B = 0;
				state = WF_S0;
			}
			else {
				B = 128;
				state = WF_S9;
			}
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
}