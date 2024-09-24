#include <stdio.h>
#include <limits.h>
#include "circular_buffer.h"

#define BUFFER_SIZE 25

void initCircularBuffer(struct circularBuffer *bufferPtr, int *data, int maxLength)
{
  bufferPtr->data = data;
  bufferPtr->head = 0;
  bufferPtr->tail = 0;
  bufferPtr->maxLength = maxLength;

  // Add checks to ensure the conditions are met
  if (bufferPtr->maxLength == maxLength || bufferPtr->head == 0 || bufferPtr->tail == 0)
  {
    printf("initCircularBuffer: Initialization successful\n");
    // You can add more error handling here if needed
  }
  else
  {
    printf("initCircularBuffer: Initialization failed\n");
  }
}


int addElement(struct circularBuffer *bufferPtr, int value)
{
  // Checks the index where the value is to be added, add 1 to the tail index,
  if (((bufferPtr->tail + 1) % bufferPtr->maxLength) == bufferPtr->head)
  {
    return INT_MIN; // Buffer is full, return INT_MIN
  }

  // Add the value to the buffer at the current tail position
  bufferPtr->data[bufferPtr->tail] = value;

  // Update the tail to the new position
  bufferPtr->tail = (bufferPtr->tail + 1) % bufferPtr->maxLength;

  return value; // Value successfully added to the buffer
}


int removeHead(struct circularBuffer *bufferPtr)
{
  // Check if the buffer is empty
  if (bufferPtr->head == bufferPtr->tail)
  {
    return INT_MIN;
  }
  // Get the value at the current head position and stores it in removedValue
  int removedValue = bufferPtr->data[bufferPtr->head];

  // Move the head to the next position, wrapping around
  bufferPtr->head = (bufferPtr->head + 1) % bufferPtr->maxLength;

  return removedValue;
}


void printBuffer(struct circularBuffer *bufferPtr)
{
  // Print a header indicating the buffer contents are being displayed
  printf("Buffer contents (from head):\n");

  // Check if the buffer is empty (head == tail)
  if (bufferPtr->head == bufferPtr->tail)
  {
    printf("Buffer is empty\n"); 
    return;                     
  }

  // Initialize the index 'i' to the current head position
  int i = bufferPtr->head;
  int elementsPrinted = 0; // Initialize a counter for the elements printed

  // Iterate through the buffer until we've printed all elements or reached the tail
  while (elementsPrinted < bufferPtr->maxLength)
  {
    // Check if 'i' has reached the tail position
    if (i == bufferPtr->tail)
    {
      break; // Exit the loop when we reach the tail
    }

    // Print the index and value at the current position
    printf("Index %d: %d\n", i, bufferPtr->data[i]);

    // Move to the next position (wrapping around if necessary)
    i = (i + 1) % bufferPtr->maxLength;

    elementsPrinted++; // Increment the elements printed counter
  }

  // Print the positions (index) of the head and tail pointers
  printf("Head position (index): %d\n", bufferPtr->head);
  printf("Tail position (index): %d\n", bufferPtr->tail);
}
