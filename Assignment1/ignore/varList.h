#ifndef VARLIST_H
#define VARLIST_H

#include "types.h"

//Item in a list; holds element, element type, and the address of the next item
typedef struct var_list_item{
	ELEMENT_t element;
	ELEMENT_TYPE_e type;
	struct var_list* list;
	struct var_list_item* next;
} GENERIC_LIST_ITEM_t;

//List structure; Has pointer to first item and a size
typedef struct var_list {
	GENERIC_LIST_ITEM_t* head;
	int size;
} GENERIC_LIST_t;

void printItem(const GENERIC_LIST_ITEM_t *item);
void addElement(GENERIC_LIST_t *list, ELEMENT_t element, ELEMENT_TYPE_e type);
// void addList(GENERIC_LIST_t *list);
void removeElement(GENERIC_LIST_t *list, int index);
GENERIC_LIST_ITEM_t* getListItem(GENERIC_LIST_t *list, int index);
void printList(const GENERIC_LIST_t *list, int topList);

#endif // VARLIST_H
