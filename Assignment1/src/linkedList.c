#include "../include/linkedList.h"


/*
Add elements to list. Requires a type and an element. Creates dynamic memory
to hold the GENERIC_LIST_ITEM_t structure
*/
void addElement(GENERIC_LIST_t *list, ELEMENT_t element, ELEMENT_TYPE_e type) {
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
		while (current->next != NULL) {
			current = current->next;
		}
		// append element
		current->next = item;
		// increment list size
		list->size += 1;
	}
}

/*
Remove item from list at given index
*/
void removeElement(GENERIC_LIST_t *list,int index){
	// do nothing if out of bounds
	if (index >= list->size) {
		printf("Error: Index out of bounds, nothing to remove.\n");
		return;
	}

	// remove head
	if (index == 0) {
		GENERIC_LIST_ITEM_t *current = list->head;
		// replace head with pointer to next element
		list->head = list->head->next;
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
			if (current->next != NULL) {
				prev = current;
				current = current->next;
			}
		}

		// remove current
		if (current->next == NULL) {
			// if current is last, make previous point to null
			prev->next = NULL;
		}
		else {
			// otherwise, make prev point to current next
			prev->next = current->next;
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
Get a list item from the list
*/
GENERIC_LIST_ITEM_t* getItem(GENERIC_LIST_t *list, int index) {
	// do nothing if out of bounds
	if (index >= list->size) {
		printf("Error: Index out of bounds.\n");
		return NULL;
	}

	GENERIC_LIST_ITEM_t* current = list->head;

	// loop to the item with the specified index
	for (int i = 0; i < index; i++) {
		// just in case, check for null
		if (current->next != NULL) {
			current = current->next;
		}
	}

	return current;
}

/*
Print items based on their type
*/
void printItem(const GENERIC_LIST_ITEM_t *item) {
	switch (item->type) {
		case CHAR:
			// %c for char
			printf("\'%c\'", item->element.c);
			break;
		case INT:
			// %ld for signed long
			printf("%li", item->element.i);
			break;
		case DOUBLE:
			// %d for double
			printf("%.5f", item->element.d);
			break;
		case STRING:
			// %s for c-string format
			printf("\"%s\"", item->element.s);
			break;
		case LIST:
			// print embedded list
			printList(item->element.l, 0);
			break;
		default:
			printf("Could not print - Unrecognized type\n");
			break;
	}
}

/*
Print the list in sequence

topList should be 1 if this is the top level list (it is not a member of another list)
0 otherwise
*/
void printList(const GENERIC_LIST_t *list, int topList) {
	// print empty list and return
	if (list->size == 0) {
		printf("[]\n");
		return;
	}
	// assign head of list to item
	GENERIC_LIST_ITEM_t* item = list->head;

	printf("[");
	// loop until last element
	while (1) {
		// print item without newlines
		printItem(item);

		if (item->next != NULL) {
			// iterate the pointer
			item = item->next;
			printf(", ");
		}
		else if (topList) {
			printf("]\n");
			break;
		}
		else {
			printf("]");
			break;
		}
	}
}
