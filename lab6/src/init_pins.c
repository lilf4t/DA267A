#include "init_pins.h"
#include "driver/gpio.h"
#include <esp32/rom/ets_sys.h>

#define LED_PIN_LEVEL_UP 12
#define LED_PIN_LEVEL_MIDDLE 14
#define LED_PIN_LEVEL_DOWN 27
#define BUTTON_PIN 26

void initLEDPins()
{

    gpio_config_t config;

    // Configure pin LED_PIN_LEVEL_UP as output
    // Configure pin LED_PIN_LEVEL_MIDDLE as output
    // Configure pin LED_PIN_LEVEL_DOWN as output
    config.pin_bit_mask = (u_int64_t)1 << LED_PIN_LEVEL_UP;
    config.mode = GPIO_MODE_OUTPUT;
    config.intr_type = GPIO_INTR_DISABLE;
    config.pull_up_en = 0;
    config.pull_down_en = 0;
    gpio_config(&config);

    config.pin_bit_mask = (u_int64_t)1 << LED_PIN_LEVEL_MIDDLE;
    gpio_config(&config);
    config.pin_bit_mask = (u_int64_t)1 << LED_PIN_LEVEL_DOWN;
    gpio_config(&config);

    /*
        // Initialize the 3 LEDs pins as output
        gpio_config_t ledConfig;
        ledConfig.pin_bit_mask = ((uint64_t)1 << LED_PIN_LEVEL_UP) | ((uint64_t)1 << LED_PIN_LEVEL_MIDDLE) | ((uint64_t)1 << LED_PIN_LEVEL_DOWN);
        ledConfig.mode = GPIO_MODE_OUTPUT;
        ledConfig.pull_down_en = 0;
        ledConfig.pull_up_en = 0;
        gpio_config(&ledConfig);
        */

    /*
         // Disable interrupts for the LED pins
        gpio_set_intr_type(LED_PIN_LEVEL_UP, GPIO_INTR_DISABLE);
        gpio_set_intr_type(LED_PIN_LEVEL_MIDDLE, GPIO_INTR_DISABLE);
        gpio_set_intr_type(LED_PIN_LEVEL_DOWN, GPIO_INTR_DISABLE);

    */
}

void initButtonPins()
{
    gpio_config_t config;
    config.pin_bit_mask = (u_int64_t)1 << BUTTON_PIN;
    config.mode = GPIO_MODE_INPUT;
    config.pull_up_en = 1;
    config.pull_down_en = 0;
    config.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&config);

    // Configure pin BUTTON_PIN as input, pull up and with interrupts on the negative edge
    /*
     gpio_config_t buttonConfig;
     buttonConfig.pin_bit_mask = ((uint64_t)1 << BUTTON_PIN);
     buttonConfig.mode = GPIO_MODE_INPUT;
     buttonConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
     buttonConfig.pull_up_en = GPIO_PULLUP_ENABLE;
     buttonConfig.intr_type = GPIO_INTR_NEGEDGE;
     gpio_config(&buttonConfig);
     */
    // DETTA ÄR EN PULL UP
}