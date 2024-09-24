#ifndef PINS_H_
#define PINS_H_
#include <stdint.h>

/* initialises the LED pin */
void initLEDPin();

/* initialises the button pin */
void initButtonPin(uint8_t pinN);

/* switches LED A on if level!=0 or off if level==0*/
void setLEDpinA(uint8_t pinN, uint8_t level);

/* switches LED  B on if level!=0 or off if level==0*/
void setLEDpinB(uint8_t pinN, uint8_t level);

/* tells if button is currently being pressed */
uint8_t isButtonPressed();

void flashLEDS();



#endif