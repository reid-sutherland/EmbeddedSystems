#include "rims.h"

static enum States { WF_S0, WF_S1, WF_S2, WF_S3, WF_S4, WF_S5, WF_S6, WF_S7, WF_S8, WF_S9 } state;

void TickFunc_WaitFlash();
