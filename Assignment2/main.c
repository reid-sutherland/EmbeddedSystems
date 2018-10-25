#include "global.h"

// Variables for main here

enum SM1_States { SM1_init, SM1_coinIn, SM1_waitRise, SM1_waitChoice, SM1_choice, SM1_dispense } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_init;
         break;
         case SM1_init:
         if (!A0) {
            SM1_State = SM1_waitRise;
         }
         break;
      case SM1_coinIn:
         if (!A0) {
            SM1_State = SM1_waitRise;
         }
         else if (coinCount == 4) {
            SM1_State = SM1_waitChoice;
         }
         break;
      case SM1_waitRise:
         if (A0) {
            SM1_State = SM1_coinIn;
            coinCount++;
         }
         break;
      case SM1_waitChoice:
         if (A == 0x02 || A == 0x04 || A == 0x08 || A == 0x10 || A == 0x03 || A == 0x05 || A == 0x09 || A == 0x11) {
            SM1_State = SM1_choice;
         }
         break;
      case SM1_choice:
         if (1) {
            SM1_State = SM1_dispense;
         }
         break;
      case SM1_dispense:
         if (wait < 20) {
            SM1_State = SM1_dispense;
            wait++;
         }
         else if (wait == 20) {
            SM1_State = SM1_init;
            B = 0x00;
         }
         break;
      default:
         SM1_State = SM1_init;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_init:
         coinCount = 0;
         wait = 0;
         break;
      case SM1_coinIn:
         break;
      case SM1_waitRise:
         break;
      case SM1_waitChoice:
         B0 = 1;
         break;
      case SM1_choice:
         if (A1) {
             B1 = 1;
         }
         else if (A2) {
             B2 = 1;
         }
         else if (A3) {
             B3 = 1;
         }
         else if (A4) {
             B4 = 1;
         }
         break;
      case SM1_dispense:
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 50;
   TimerSet(periodState_machine_1);
   TimerOn();

   SM1_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
      while(!SM1_Clk);
      SM1_Clk = 0;
   } // while (1)
} // Main
