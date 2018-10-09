#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "types.h"

//Item in a list; holds element, element type, and the address of the next item
typedef struct generic_list_item{
	ELEMENT_t element;
	ELEMENT_TYPE_e type;
	struct generic_list_item* next;
} GENERIC_LIST_ITEM_t;

//List structure; Has pointer to first item and a size
typedef struct generic_list{
	GENERIC_LIST_ITEM_t* head;
	int size;
} GENERIC_LIST_t;

void printItem(const GENERIC_LIST_ITEM_t *item);
void addElement(GENERIC_LIST_t *list, ELEMENT_TYPE_e type, ELEMENT_t element);
void removeElement(GENERIC_LIST_t *list, int index);
void printList(const GENERIC_LIST_t *list);

#endif // LINKEDLIST_H
