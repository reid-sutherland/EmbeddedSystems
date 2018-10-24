#include "rims.h"

// Global variables/functions for project here
//  all clocks and timer functions here,
//  and state machines for both blinks as well
unsigned char SM1_Clk;


// Timer function
void TimerISR() {
   SM1_Clk = 1;
}
