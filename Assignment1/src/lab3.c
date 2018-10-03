#include <stdio.h>
#include <time.h>
#include "linkedList.h"

int main(){
  time_t t;
  /* Intializes random number generator */
  srand((unsigned) time(&t));
  //Create element object
  ELEMENT_t element;
  //Test Integer type
  element.i = -15;
  GENERIC_LIST_ITEM_t myItem = {element,SINT_32,NULL};
  printItem(&myItem);
  //Test Character type
  myItem.element.c = 'A';
  myItem.type=CHAR;
  printItem(&myItem);
  //Test double type
  myItem.element.d = 1.54;
  myItem.type=DOUBLE;
  printItem(&myItem);
  //Test string type
  char* myString = "Hello World!";
  myItem.element.s = myString;
  myItem.type=STRING;
  printItem(&myItem);

  printf("\n");

  //UNCOMMENT THIS FOR TESTING TASKS 6-7

  //Create list object & intialize
  GENERIC_LIST_t myList;
  myList.head = NULL;
  myList.size = 0;
  //Add integer item
  element.i = 5;
  addElement(&myList,SINT_32,element);
  //Add character item
  element.c = 'A';
  addElement(&myList,CHAR,element);
  //Add double item
  element.d = 3.4;
  addElement(&myList,DOUBLE,element);

  GENERIC_LIST_ITEM_t* item = myList.head;

  //Print the list
  printList(&myList);


  //UNCOMMENT HERE TO TEST TASK 8

  //Remove middle item
  removeElement(&myList,1);
  printList(&myList);
  //Remove end item
  removeElement(&myList,1);
  printList(&myList);
  //Remove beginning item
  removeElement(&myList,0);
  printList(&myList);


  //Uncomment here to test all 10 tasks

  //Add 0-99 integers sequentially
  int i = 0;
  for(i=0;i<100;i++){
    element.i = i;
    addElement(&myList,SINT_32,element);
  }
  //Print the list
  printList(&myList);
  //Remove elements at random
  for(i=100;i>0;i--){
    removeElement(&myList,rand()%i);
    //Print list every 10
    if(i%10==0){
      printList(&myList);
    }
  }


  return 0;
}
