/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 10/21/2018 14:19:42 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/
int btnPressed = 0;
typedef struct task {
   int state;
   unsigned long period;
   unsigned long elapsedTime;
   int (*TickFct)(int);
} task;

task tasks[2];

const unsigned char tasksNum = 2;
const unsigned long periodState_machine_1 = 40;
const unsigned long periodState_machine_2 = 200;

const unsigned long tasksPeriodGCD = 40;

int TickFct_State_machine_1(int state);
int TickFct_State_machine_2(int state);

unsigned char processingRdyTasks = 0;
void TimerISR() {
   unsigned char i;
   if (processingRdyTasks) {
      printf("Period too short to complete tasks\n");
   }
   processingRdyTasks = 1;
   for (i = 0; i < tasksNum; ++i) { // Heart of scheduler code
      if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
         tasks[i].state = tasks[i].TickFct(tasks[i].state);
         tasks[i].elapsedTime = 0;
      }
      tasks[i].elapsedTime += tasksPeriodGCD;
   }
   processingRdyTasks = 0;
}
int main() {
   // Priority assigned to lower position tasks in array
   unsigned char i=0;
   tasks[i].state = -1;
   tasks[i].period = periodState_machine_1;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct = &TickFct_State_machine_1;

   ++i;
   tasks[i].state = -1;
   tasks[i].period = periodState_machine_2;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct = &TickFct_State_machine_2;

   ++i;
   TimerSet(tasksPeriodGCD);
   TimerOn();

   while(1) { Sleep(); }

   return 0;
}

enum SM1_States { SM1_debLow, SM1_debHigh, SM1_btnPressed } SM1_State;
int TickFct_State_machine_1(int state) {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
static int debCount = 0;
static int debTarget = 3;
   switch(state) { // Transitions
      case -1:
         B = 0;
         state = SM1_debLow;
         break;
      case SM1_debLow:
         if (!A0) {
            state = SM1_debLow;
         }
         else if (A0) {
            state = SM1_debHigh;
            debCount = 0;
         }
         break;
      case SM1_debHigh:
         if (A0 && debCount < debTarget) {
            state = SM1_debHigh;
            debCount++;
         }
         else if (!A0) {
            state = SM1_debLow;
         }
         else if (A0 && debCount >= debTarget) {
            state = SM1_btnPressed;
            btnPressed = 1;
         }
         break;
      case SM1_btnPressed:
         if (!A0) {
            state = SM1_debLow;
            btnPressed = 0;
         }
         else if (A0) {
            state = SM1_btnPressed;
         }
         break;
      default:
         state = -1;
      } // Transitions

   switch(state) { // State actions
      case SM1_debLow:
         break;
      case SM1_debHigh:
         break;
      case SM1_btnPressed:
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
   SM1_State = state;
   return state;
}


enum SM2_States { SM2_s1, SM2_s2 } SM2_State;
int TickFct_State_machine_2(int state) {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(state) { // Transitions
      case -1:
         state = SM2_s1;
         break;
      case SM2_s1:
         if (btnPressed) {
            state = SM2_s2;
         }
         else if (!btnPressed) {
            state = SM2_s1;
         }
         break;
      case SM2_s2:
         if (!btnPressed) {
            state = SM2_s1;
         }
         else if (btnPressed) {
            state = SM2_s2;
         }
         break;
      default:
         state = -1;
      } // Transitions

   switch(state) { // State actions
      case SM2_s1:
         B = (A >> 1);
         break;
      case SM2_s2:
         if (B7 || B == 0)
           B7 = 1;
         else
           B7 = 0;
         if (!B7)
           B = B - 1;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
   SM2_State = state;
   return state;
}
