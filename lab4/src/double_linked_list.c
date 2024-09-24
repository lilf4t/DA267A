#include<stdlib.h>
#include<limits.h>
#include"double_linked_list.h"


/////////////////////// add element ////////////////////

int addElementDoubleLinkedList(struct doubleLinkedList* list, int value) {
    // Create a new element and allocate memory for it
    struct doubleLinkedListElement* newElement = (struct doubleLinkedListElement*)malloc(sizeof(struct doubleLinkedListElement));
    if (newElement == NULL) {
        // Memory allocation failed
        return INT_MIN;
    }

    // Fill in the data, dont set the next pointer to NULL as opposed to single linked list
    newElement->data = value;

    // If the list is empty, or the new element should be the first
    if (list->first == NULL || value <= list->first->data) {
        newElement->next = list->first;
        newElement->previous = NULL;

        //As opposed to single linked list, point to previous element
        if (list->first != NULL) {
            list->first->previous = newElement;
        } else {
            // If the list was empty, update the last pointer
            list->last = newElement;
        }

        list->first = newElement;
        return value;
    }

    // Find the appropriate position to insert the new element while maintaining the sorted order
    struct doubleLinkedListElement* current = list->first;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }

    // Insert the new element, previous element
    newElement->next = current->next;
    newElement->previous = current;
    
    if (current->next != NULL) {
        current->next->previous = newElement;
    } else {
        // If we're inserting at the end, update the last pointer
        list->last = newElement;
    }

    current->next = newElement;

    return value;
}

/////////////////////// init, empty list ////////////////////

void initDoubleLinkedList(struct doubleLinkedList* list) {
  list->first = NULL;
  list->last = NULL;
}


/////////////////////// remove first element ////////////////////

int removeFirstElementDoubleLinkedList(struct doubleLinkedList* list) {
    if (list->first == NULL) {
        // The list is empty, so nothing to remove
        return INT_MIN;
    }

    // Get the data from the first element
    int dataToRemove = list->first->data;

    // Store a pointer to the first element for later removal
    struct doubleLinkedListElement* elementToRemove = list->first;

    // Update the first pointer to the second element
    list->first = list->first->next;

    // If the list is now empty, update the last pointer to NULL // as opposed to single linked
    if (list->first == NULL) {
        list->last = NULL;
    } else {
        // Update the previous pointer of the new first element
        list->first->previous = NULL;
    }

    // Free the memory associated with the removed element
    free(elementToRemove);

    return dataToRemove;
}

/////////////////////// remove last element ////////////////////

int removeLastElementDoubleLinkedList(struct doubleLinkedList* list) {
    if (list->first == NULL) {
        // The list is empty, so nothing to remove
        return INT_MIN;
    }

    // Get the data from the last element
    int dataToRemove = list->last->data;

    // Store a pointer to the last element for later removal
    struct doubleLinkedListElement* elementToRemove = list->last;

    // Update the last pointer to the second-to-last element
    list->last = list->last->previous;

    // If the list is now empty, update the first pointer to NULL
    if (list->last == NULL) {
        list->first = NULL;
    } else {
        // Update the next pointer of the new last element
        list->last->next = NULL;
    }

    // Free the memory associated with the removed element
    free(elementToRemove);

    return dataToRemove;
}






