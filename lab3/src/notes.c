#include <math.h>
#include <stdio.h>
#include <string.h>
#include "notes.h"
#include "pins.h"

// Define the reference frequency A4 (440 Hz)
#define REFERENCE_FREQ_A4 440.0f

// Function to convert a frequency to a musical note
void freq2note(float freq, char *stringbuff)
{
    // Define an array to map semitone values to note names and accidentals
    char noteNames[12] = {'A', 'A', 'B', 'C', 'C', 'D', 'D', 'E', 'F', 'F', 'G', 'G'};
    char accidentals[12] = {' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#'};

    // Calculate the number of semitones relative to A4
    int floatInt = (int)round(12 * log2(freq / REFERENCE_FREQ_A4));

    // Handle negative and out-of-range values
    if (floatInt < 0)
    {
        floatInt = 12 + floatInt; // Wrap around for negative values
    }
    else if (floatInt > 11)
    {
        floatInt = floatInt - 12; // Wrap around for values greater than 11
    }

    // Set the note and accidental (if any) characters in the output string buffer
    stringbuff[0] = noteNames[floatInt];   // Set the note character
    stringbuff[1] = accidentals[floatInt]; // Set the accidental character

    // Null-terminate the string to make sure it's a valid C string
    stringbuff[2] = '\0';
}
