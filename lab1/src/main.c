#include <esp_task_wdt.h>
#include "pins.h"
#include "random.h"

/*
Waits for "millis" milliseconds without upsetting the watchdog timer
*/
void waitMs(unsigned int millis)
{
TickType_t delay = millis / portTICK_PERIOD_MS;
vTaskDelay(delay);
}
void app_main()
{
initPins();
while (1)
{
// signal that the game is about to start by making both of them flash
setLEDA(1);
setLEDB(1);
waitMs(500); // Wait for 0.5 seconds

// switch both LEDs off
setLEDA(0);
setLEDB(0);

// get a random duration between 3 and 5 seconds
int randomDuration = getRandommsecs(3000, 5000);

// wait that random duration
waitMs(randomDuration);

// switch both LEDs ON
setLEDA(1);
setLEDB(1);

uint8_t winner = 0;
while (!winner)
{
// check if either button A or B are pressed
// if any is pressed, set winner to 1 for A or 2 for B
if(isButtonAPressed()) {
    winner = 1;
} else if(isButtonBPressed()){
    winner = 2;
}

}
// if A wins, flash LED A and switch off B
if(winner == 1) {
   setLEDB(0); // Turn off LED A
    waitMs(5000); // Wait for 0.5 seconds 
    setLEDB(1); // Turn on LED A
}

// if B wins, flash LED B and switch off A
if(winner == 2) {
   

    setLEDA(0); // Turn off LED A
    waitMs(5000); // Wait for 0.5 seconds
    setLEDA(1); // Turn on LED A
}

// switch off both A and B and wait for some time to restart the game
setLEDA(0);
setLEDB(0);
waitMs(5000);
}
}