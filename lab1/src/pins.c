#include "pins.h"
#include "driver/gpio.h"
#include <esp32/rom/ets_sys.h>

/* initialises the 4 pins */
void initPins()
{
    printf("Initializing pins...\n");
    // init the 2 LEDs pins as output and the 2 buttons' pins as input
    // you will need to use gpio_config()

    // Initialize the 2 LEDs pins as output
    gpio_config_t ledConfig;
    ledConfig.pin_bit_mask = ((uint64_t)1 << 25) | ((uint64_t)1 << 33); // Set bits 25 and 33
    ledConfig.mode = GPIO_MODE_OUTPUT;
    ledConfig.pull_down_en = 0;
    ledConfig.pull_up_en = 0;

    // Configure the LED pins
    gpio_config(&ledConfig);

    // Initialize the 2 buttons' pins as input
    gpio_config_t buttonConfig;
    buttonConfig.pin_bit_mask = ((uint64_t)1 << 26) | ((uint64_t)1 << 27); // Set bits 26 and 27
    buttonConfig.mode = GPIO_MODE_INPUT;
    buttonConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    buttonConfig.pull_up_en = GPIO_PULLUP_ENABLE;
    //DETTA ÄR EN PULL UP

    // Configure the button pins
    gpio_config(&buttonConfig);

    printf("Pins initialized.\n");
}
/* switches LED A on if level!=0 or off if level==0*/
void setLEDA(uint8_t level)
{
    printf("Setting LED A to %d\n", level);
    if (level)
    {
        // set the pin of LED A to ON
        // you probably need to use gpio_set_level()

        // set pin 25 to high
        gpio_set_level(25, 1);
       // ets_delay_us(1000000ul);
    }
    else
    {
        // set the pin of LED A to OFF
        // set pin 25 to low
        gpio_set_level(25, 0);
       // ets_delay_us(1000000ul);
    }
}
/* switches LED B on if level!=0 or off if level==0*/
void setLEDB(uint8_t level)
{
    printf("Setting LED B to %d\n", level);

    // same as setLEDA()
    if (level)
    {
        // set the pin of LED A to ON
        // you probably need to use gpio_set_level()

        // set pin 33 to high
        gpio_set_level(33, 1);
       // ets_delay_us(1000000ul);
    }
    else
    {
        // set the pin of LED A to OFF
        // set pin 33 to low
        gpio_set_level(33, 0);
      //  ets_delay_us(1000000ul);
    }
}

/* tells if button A is currently being pressed */
uint8_t isButtonAPressed()
{
    // read the value of button A
    // if using a pull-up, the button is pressed when the pin is LOW
    printf("Button A is pressed.");

    return gpio_get_level(26) == 0; // return 1 if A is pressed, 0 otherwise
}

/* tells if button A is currently being pressed */
uint8_t isButtonBPressed()
{
    // same as with button A, but another pin
    printf("Button B is pressed");
    
    return gpio_get_level(27) == 0;
}