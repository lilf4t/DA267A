#include "random.h"
#include "esp_system.h"
#include "esp_random.h"
#include <stdint.h>
#include <stdlib.h>

// Define the maximum value for the random number generator
#define UINT32_MAX_F ((float)UINT32_MAX)

int getRandommsecs(int min, int max)
{
    // Generate a random 32-bit unsigned integer
    uint32_t randomValue = esp_random();

    // Convert the random value to a float between 0 and 1
    float r = (float)randomValue / UINT32_MAX_F;

    // Calculate the random value between min and max
    int randomMillis = min + (int)(r * (max - min + 1));

    return randomMillis;
}
