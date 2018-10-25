#include "rims.h"

static enum States { GOF_idle, GOF_flashOff, GOF_flashOn, GOF_waitOff, GOF_waitOn } states;

void TickFunc_GameOverFlash();
