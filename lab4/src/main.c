#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "single_linked_list.h"
#include "double_linked_list.h"


void singleLinkedListTest()
{
    struct singleLinkedList myList;
    initSingleLinkedList(&myList);

    ///////////////////Testing to remove element from empty list//////////////////////
    int removedData = removeLastElementSingleLinkedList(&myList);

    printf("Printing list (initial state):\n");

    // Print the list elements if any exist
    struct singleLinkedListElement *current = myList.first;
    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");

    printf("\nTest to remove element from empty list\n");
    printf("____________________________________\n");
    if (removedData == sizeof(myList))
    {
        printf("Successfully removed the last element from an empty list.\n");
    }
    else
    {
        printf("Failed to remove the last element from an empty list.\n");
    }

    printf(" \n");

    ////////////////////// adding random elements to the list ////////////////////////////
    addElementSingleLinkedList(&myList, 3);
    addElementSingleLinkedList(&myList, 2);
    addElementSingleLinkedList(&myList, 1);

    printf("Test to add elements to single linked list\n");
    printf("____________________________________");
    printf(" \n");
    printf("List after adding elements: ");
    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
    printf(" \n");

    ////////////////////// removing the first element /////////////////////////////
    int firstElement = removeFirstElementSingleLinkedList(&myList);
    printf("Test to remove the first element\n");
    printf("____________________________________");
    printf(" \n");
    printf("Removed the first element: %d\n", firstElement);

    // Print the list after removing the first element
    current = myList.first;
    printf("List after removing the first element: ");
    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
    printf(" \n");

    ////////////////////// removing the last element /////////////////////////////
    int removeLastElement = removeLastElementSingleLinkedList(&myList);
    printf("Test to remove the last element\n");
    printf("____________________________________");
    printf(" \n");
    printf("Removed the last element: %d\n", removeLastElement);

    // Print the list after removing the last element
    current = myList.first;
    printf("List after removing the last element: ");
    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
    printf(" \n");
}

void doubleLinkedListTest()
{
    struct doubleLinkedList myList;
    initDoubleLinkedList(&myList);
    struct doubleLinkedListElement *current = myList.first;

    // Print the list elements if any exist
    printf("Printing list (initial state):\n");
    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");

    ////// Test adding elements/////////
    addElementDoubleLinkedList(&myList, 3);
    addElementDoubleLinkedList(&myList, 2);
    addElementDoubleLinkedList(&myList, 4);
    addElementDoubleLinkedList(&myList, 1);

    // Print the list after adding elements
    current = myList.first;
    printf("\nTest to add elements to double linked list\n");
    printf("____________________________________");
    printf(" \n");
    printf("List after adding elements: ");

    current = myList.first;
    while (current != NULL)
    {
        printf("%d <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");

    ///////Traversing backwards to see if previous elements works ////////
    struct doubleLinkedListElement *current1 = myList.last;
    printf("Test to traverse backwards in double linked list\n");
    printf("____________________________________");
    printf(" \n");
    printf("List traversal backward: ");
    while (current1 != NULL)
    {
        printf("%d <-> ", current1->data);
        current1 = current1->previous;
    }
    printf("NULL\n");
    printf(" \n");

    // Test removing the first element
    int dataToRemoveFirst = removeFirstElementDoubleLinkedList(&myList);
    printf("Test to remove first element\n");
    printf("____________________________________");
    printf(" \n");
    printf("Removed first element with data: %d\n", dataToRemoveFirst);

    // Print the list after removing the first element
    current = myList.first;
    printf("List after removing the first element: ");
    while (current != NULL)
    {
        printf("%d <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
    printf(" \n");

    // Test removing the last element
    int dataToRemoveLast = removeLastElementDoubleLinkedList(&myList);
    printf("Test to remove the last element\n");
    printf("____________________________________");
    printf(" \n");
    printf("Removed last element: %d\n", dataToRemoveLast);

    // Print the list after removing the last element
    current = myList.first;
    printf("List after removing the last element: ");
    while (current != NULL)
    {
        printf("%d <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
    printf(" \n");
}

void app_main()
{
    printf("\nNow testing singlelinked list\n");
    singleLinkedListTest();

    printf("\nNow testing double linked list\n");
    doubleLinkedListTest();
}