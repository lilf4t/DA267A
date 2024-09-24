#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>
#include "circular_buffer.h"
#include <limits.h>

#define BUFFER_SIZE 4

void blackboxTest1()
{
    struct circularBuffer buffer;
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);

    // Add an element to the circular buffer
    int addedElement = addElement(&buffer, 1);

    // Print the buffer content
    printf("Buffer after adding an element:\n");
    printBuffer(&buffer);

    // Remove one element from the circular buffer
    int removedElement = removeHead(&buffer);

    // Print the buffer content after removal
    printf("\nBuffer after removing an element:\n");
    printBuffer(&buffer);

    // Check if the added and removed elements are the same
    if (addedElement == removedElement)
    {
        printf("\nAdded and removed elements are the same: %d\n", addedElement);
    }
    else
    {
        printf("\nAdded and removed elements are different.\n");
    }
    // Free allocated memory
    free(buffer_data);
}

void blackboxTest2()
{

    struct circularBuffer buffer;
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);

    // Add an element to the circular buffer
    int addedElement = addElement(&buffer, 1);
    int addedElement2 = addElement(&buffer, 2);

    // Print the buffer content
    ets_delay_us(2000000ul);
    printf("Buffer after adding an element:\n ");
    printBuffer(&buffer);

    // Remove one element from the circular buffer
    int removedElement = removeHead(&buffer);
    int removedElement2 = removeHead(&buffer);

    // Print the buffer content after removal
    ets_delay_us(2000000ul);
    printf("\nBuffer after removing an element:\n");
    printBuffer(&buffer);

    // Check if added and removed elements are in the same order
    if (addedElement == removedElement && addedElement2 == removedElement2)
    {
        printf("\nAdded and removed elements are in the same order.\n");
    }
    else
    {
        printf("\nAdded and removed elements are NOT in the same order.\n");
    }

    // Free allocated memory
    free(buffer_data);
}

void blackboxTest3()
{
    struct circularBuffer buffer;
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);

    int addedElement = addElement(&buffer, 25);
    int addedElement2 = addElement(&buffer, 36);
    int addedElement3 = addElement(&buffer, 48);

    // Print the buffer content
    ets_delay_us(2000000ul);
    printf("Buffer after adding an element:\n ");
    printBuffer(&buffer);

    int removedElement = removeHead(&buffer);
    printf("After removing 1 element");
    printBuffer(&buffer);
    int removedElement2 = removeHead(&buffer);
    printf("After removing 2 element");
    printBuffer(&buffer);
    int removedElement3 = removeHead(&buffer);
    printf("After removing 3 element");
    printBuffer(&buffer);

    // Print the buffer content
    ets_delay_us(2000000ul);
    printf("Buffer after removing an element:\n ");
    printBuffer(&buffer);

    // Check if added and removed elements are in the same order
    if (addedElement == removedElement && addedElement2 == removedElement2 && addedElement3 == removedElement3)
    {
        printf("\nAdded and removed elements are in the same order.\n");
    }
    else
    {
        printf("\nAdded and removed elements are NOT in the same order.\n");
    }

    // Free allocated memory
    free(buffer_data);
}

void blackboxTest4()
{
    struct circularBuffer buffer;
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);

    int addedElement = addElement(&buffer, 25);
    int addedElement2 = addElement(&buffer, 36);
    int addedElement3 = addElement(&buffer, 48);
    int addedElement4 = addElement(&buffer, 500);

    // Print the buffer content
    ets_delay_us(2000000ul);
    printf("Buffer after adding an element:\n ");
    printBuffer(&buffer);

    // Check if the last added element overwrote the first added element
    if (addedElement == addedElement4)
    {
        printf("\nThe last added element overwrote the first added element.\n");
    }
    else
    {
        printf("\nThe last added element did not overwrite the first added element.\n");
    }

    // Free allocated memory
    free(buffer_data);
}

void blackboxTest5()
{
    struct circularBuffer buffer;
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);

    int addedElement = addElement(&buffer, 25);
    int addedElement2 = addElement(&buffer, 36);
    int addedElement3 = addElement(&buffer, 48);
    int addedElement4 = addElement(&buffer, 500);

    // Print the buffer content
    ets_delay_us(2000000ul);
    printf("Buffer after adding an element:\n ");
    printBuffer(&buffer);

    int removedElement = removeHead(&buffer);
    printf("After removing 1 element");
    printBuffer(&buffer);
    int removedElement2 = removeHead(&buffer);
    printf("After removing 2 element");
    printBuffer(&buffer);
    int removedElement3 = removeHead(&buffer);
    printf("After removing 3 element");
    printBuffer(&buffer);
    int removedElement4 = removeHead(&buffer);
    printf("After removing 4 element");
    printBuffer(&buffer);

    // Print the buffer content
    ets_delay_us(2000000ul);
    printf("Buffer after removing an element:\n ");
    printBuffer(&buffer);

    // Check if added and removed elements are in the same order
    if (addedElement == removedElement && addedElement2 == removedElement2 && addedElement3 == removedElement3 && addedElement4 == removedElement4)
    {
        printf("\nAdded and removed elements are in the same order.\n");
    }
    else
    {
        printf("\nAdded and removed elements are NOT in the same order.\n");
    }

    // Free allocated memory
    free(buffer_data);
}

void blackboxTest6()
{
    struct circularBuffer buffer;
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);

    // Call contains() on an empty buffer
    int result = contains(&buffer, 0);

    // Check if the result is INT_MIN
    if (result == INT_MIN)
    {
        printf("contains() on an empty buffer returned INT_MIN as expected.\n");
    }
    else
    {
        printf("contains() on an empty buffer did not return INT_MIN as expected.\n");
    }

    // Print the buffer status
    printBuffer(&buffer);

    // Free allocated memory
    free(buffer_data);
}

void blackboxTest7()
{
    struct circularBuffer buffer;
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);

    int element = 25;

    int addedElement = addElement(&buffer, element);
    printf("Added element %d to the buffer.\n", addedElement);
    // printf(&buffer);

    int containsResult = contains(&buffer, element);
    if (containsResult == INT_MIN)
    {
        printf("Buffer does not contain the added element %d as expected.\n", containsResult);
    }
    else
    {
        printf("Buffer contains the added element %d as expected.\n", containsResult);
    }

    // Free allocated memory
    free(buffer_data);
}

void blackboxTest8()
{
    struct circularBuffer buffer;
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);

    int addedElement = addElement(&buffer, 25);
    int addedElement2 = addElement(&buffer, 36);

    int containsResult = contains(&buffer, addedElement2);

    // Print the buffer content
    ets_delay_us(2000000ul);
    printf("Buffer after adding elements:\n ");
    printBuffer(&buffer);

    if (containsResult == addedElement2)
    {
        printf("\nBuffer contains the value of the second element added: %d\n", addedElement2);
    }
    else
    {
        printf("\nBuffer does not contain the value of the second element added.\n");
    }

    // Free allocated memory
    free(buffer_data);
}

void blackboxTest9()
{
    struct circularBuffer buffer;
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);

    int addedElement = addElement(&buffer, 25);
    int addedElement2 = addElement(&buffer, 36);
    int addedElement3 = addElement(&buffer, 50);

    int containsResult = contains(&buffer, addedElement3);

    // Print the buffer content
    ets_delay_us(2000000ul);
    printf("Buffer after adding elements:\n ");
    printBuffer(&buffer);

    if (containsResult == addedElement3)
    {
        printf("\nBuffer contains the value of the second element added: %d\n", addedElement3);
    }
    else
    {
        printf("\nBuffer does not contain the value of the second element added.\n");
    }

    // Free allocated memory
    free(buffer_data);
}

void app_main()
{
    printf("\n\n\n");
    printf("\n\n\n");
    printf("Running Blackbox Test 9\n");
    // blackboxTest1();
    // blackboxTest2();
    // blackboxTest3();
    // blackboxTest4();
    // blackboxTest5();
    // blackboxTest6();
    // blackboxTest7();
    // blackboxTest8();
    // blackboxTest9();
    /*
        struct circularBuffer buffer;
        int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
        initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);
    */

    printf("\n\n\n");
}
