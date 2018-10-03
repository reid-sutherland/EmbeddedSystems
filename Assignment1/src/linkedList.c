#include "../include/linkedList.h"

/*
Print items based on their type
*/
void printItem(const GENERIC_LIST_ITEM_t *item){
//TODO TASK 5
	switch (item->type) {
		case CHAR:
			// %c for char
			printf("%c\n", item->element.c);
			break;
		case SINT_32:
			// %ld for signed long
			printf("%li\n", item->element.i);
			break;
		case DOUBLE:
			// %d for double
			printf("%f\n", item->element.d);
			break;
		case STRING:
			// %s for c-string format
			printf("%s\n", item->element.s);
			break;
		default:
			printf("Could not print - Unrecognized type\n");
			break;
	}
}

/*
Add elements to list. Requires a type and an element. Creates dynamic memory
to hold the GENERIC_LIST_ITEM_t structure
*/
void addElement(GENERIC_LIST_t *list, ELEMENT_TYPE_e type, ELEMENT_t element){
//TODO TASK 7
	// create list item object, allocate memory, assign values
	GENERIC_LIST_ITEM_t* item = calloc(1, sizeof(GENERIC_LIST_ITEM_t));
	item->element = element;
	item->type = type;

	// assign item to head of list if list is empty
	if (list->size == 0) {
		// assign head
		list->head = item;
		// set size
		list->size = 1;
	}
	// append the item to the end of list
	else {
		GENERIC_LIST_ITEM_t* current;
		current = list->head;
		// loop to the last item
		while (current->nextItem != NULL) {
			current = current->nextItem;
		}
		// append element
		current->nextItem = item;
		// increment list size
		list->size += 1;
	}
}

/*
Remove item from list at given index
*/
void removeElement(GENERIC_LIST_t *list,int index){
//TODO TASK 8
	// do nothing if out of bounds
	if (index >= list->size) {
		printf("Error: Index out of bounds, nothing to remove.\n");
		return;
	}

	// remove head
	if (index == 0) {
		GENERIC_LIST_ITEM_t *current = list->head;
		// replace head with pointer to next element
		list->head = list->head->nextItem;
		// decrement size
		list->size -= 1;
		// free the memory
		if (current->type == STRING) {
			free(current->element.s);
		}
		free(current);
	}
	// remove other elements
	else {
		GENERIC_LIST_ITEM_t* current = list->head;
		GENERIC_LIST_ITEM_t* prev = NULL;

		// loop to the item with the specified index
		for (int i = 0; i < index; i++) {
			// just in case, check for null
			if (current->nextItem != NULL) {
				prev = current;
				current = current->nextItem;
			}
		}

		// remove current
		if (current->nextItem == NULL) {
			// if current is last, make previous point to null
			prev->nextItem = NULL;
		}
		else {
			// otherwise, make prev point to current next
			prev->nextItem = current->nextItem;
		}

		// free the memory
		if (current->type == STRING) {
			free(current->element.s);
		}
		free(current);

		// both ops require a size decrement
		list->size -= 1;
	}
}

/*
Print the list in sequence
*/
void printList(const GENERIC_LIST_t *list){
//TODO TASK 9
	// if list is empty, error, do nothing
	if (list->size == 0) {
		printf("Error: List is empty, nothing to print.\n");
		return;
	}
	// assign head of list to item
	GENERIC_LIST_ITEM_t* item = list->head;

	int i = 0;
	// loop until last element
	while (1) {
		// print item
		printItem(item);

		if (item->nextItem != NULL) {
			// iterate the pointer
			item = item->nextItem;
		}
		else {
			printf("\n");
			break;
		}
		i++;
	}
}
