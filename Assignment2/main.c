#include "global.h"


unsigned char WaitFlash_Clk;
unsigned char GameOverFlash_Clk;
unsigned char Debounce_Clk;
unsigned char Game_Clk;


// Timer function
void TimerISR() {
   SM1_Clk = 1;
}

int main() {

   const unsigned int WaitFlash_Period = 50;
	 const unsigned int GameOverFlash_Period = 50;
	 const unsigned int Debounce_Period = 20;
	 const unsigned int Game_Period = 100;

	 // Skip the math here, but should come out to a universal period of 10 ms
	 const unsigned int Concurrent_Period = 10;
   TimerSet(Concurrent_Period);
   TimerOn();

   SM1_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
      while(!SM1_Clk);
      SM1_Clk = 0;
   } // while (1)
} // Main
