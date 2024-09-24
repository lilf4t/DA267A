#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include"single_linked_list.h"



//////////////////////////////add element///////////////////////////////////////////

int addElementSingleLinkedList(struct singleLinkedList* list, int value) {
    // Create a new element and allocate memory for it
    struct singleLinkedListElement* newElement = (struct singleLinkedListElement*)malloc(sizeof(struct singleLinkedListElement));
    if (newElement == NULL) {
        // Memory allocation failed
        return INT_MIN;
    }

    // Fill in the data and set the next pointer to NULL because its a single linked list
    newElement->data = value;
    newElement->next = NULL;

    // If the list is empty or the new element should be the first
    if (list->first == NULL || value <= list->first->data) {
        newElement->next = list->first;
        list->first = newElement;
        return value;
    }

    // Find the appropriate position to insert the new element while maintaining the sorted order
    struct singleLinkedListElement* current = list->first;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }

    // Insert the new element
    newElement->next = current->next;
    current->next = newElement;

    return value;
}

//////////////////////////////init single linked list///////////////////////////////////////////

void initSingleLinkedList(struct singleLinkedList* list) {
  list->first = NULL;
}

//////////////////////////////remove first element///////////////////////////////////////////

int removeFirstElementSingleLinkedList(struct singleLinkedList* list) {
    if (list->first == NULL) {
        // The list is empty, so nothing to remove
        return INT_MIN;
    }

    // Get the data from the first element
    int dataToRemove = list->first->data;

    // Store a pointer to the first element for later removal
    struct singleLinkedListElement* elementToRemove = list->first;

    // Update the first pointer to point to the second element
    list->first = list->first->next;

    // Free the memory associated with the removed element
    free(elementToRemove);

    return dataToRemove;
}

//////////////////////////////remove last element///////////////////////////////////////////

int removeLastElementSingleLinkedList(struct singleLinkedList* list) {
    if (list->first == NULL) {
        // The list is empty, so nothing to remove
        return INT_MIN;
    }

    // If there's only one element in the list, remove it
    if (list->first->next == NULL) {
        int dataToRemove = list->first->data;
        free(list->first);
        list->first = NULL;  // Set list to empty
        return dataToRemove;
    }

    // Find the second-to-last element in the list
    struct singleLinkedListElement* secondToLast = list->first;
    while (secondToLast->next->next != NULL) {
        secondToLast = secondToLast->next;
    }

    // Get the data from the last element
    int dataToRemove = secondToLast->next->data;

    // Free the memory associated with the last element
    free(secondToLast->next);

    // Update the second-to-last element's next pointer to NULL
    secondToLast->next = NULL;

    return dataToRemove;
}


