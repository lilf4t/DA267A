#include <stdio.h>
#include <esp_task_wdt.h>
#include <esp_timer.h>
#include "notes.h"
#include "sampler.h"
#include "pins.h"

#define LED_PINA 12
#define LED_PINB 13
#define BUTTON_PIN 18
#define MICROPHONE_PIN 32
#define ESP_INTR_FLAG_DEFAULT 0
#define debounceTime 250000 // 250ms

// Array of target frequencies for each guitar string
float targetFrequencies[6] = {329.63f, 246.94f, 196.00f, 146.83f, 110.00f, 82.41f};
int currentString = 0; // Index of the currently tuned string

static uint64_t prevDebounceTime = -debounceTime;
static volatile float fr;

// disable interrupts for that pin
// if enough time has passed since the button was last pressed
// go to the next note
// reactivate interrupts for that pin
static void buttonPress_handler(void *arg)
{
   uint64_t CurrentTime = esp_timer_get_time();
   if ((CurrentTime - prevDebounceTime) > debounceTime)
   {
      prevDebounceTime = CurrentTime;
      if (currentString < 5)
      {
         currentString++;
      }
      else if (currentString == 5)
      {
         currentString = 0;
      }
   }
}

void setLeds(float freq, float min, float max)
{
   if (freq >= min && freq <= max)
   {
      setLEDpinA(LED_PINA, 1);
      setLEDpinB(LED_PINB, 1);
   }
   else if (freq < min)
   {
      setLEDpinA(LED_PINA, 1);
   }
   else if (freq > max)
   {
      setLEDpinB(LED_PINB, 1);
   }
}

void app_main()
{

   initLEDPin();
   initButtonPin(BUTTON_PIN);

   gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
   gpio_isr_handler_add(BUTTON_PIN, buttonPress_handler, NULL);
   vTaskDelay(pdMS_TO_TICKS(4000));

   flashLEDS();

   while (1)
   {
      setLEDpinA(LED_PINA, 0);
      setLEDpinB(LED_PINB, 0);

      vTaskDelay(pdMS_TO_TICKS(2000));
      printf("\n...Time to start...\n");

      startSampling(6000);
      vTaskDelay(pdMS_TO_TICKS(1000));

      stopSampling();
      fr = getFrequency();

      if (currentString == 0)
      {
         // String: 1 (E), Freq: 329.63 Hz, Note: E4
         printf("yellow led = note is low , blue led = note is high\n");
         printf("Note E4 \n");
         setLeds(fr, 326.63, 332.63);

         printf("frequency is: %.2f\n\n", fr);
         printf("--------------\n");
         vTaskDelay(pdMS_TO_TICKS(2000));
         resetSampling();
      }
      else if (currentString == 1)
      {
         // String: 2 (B), Freq: 246.94 Hz, Note: B3
         printf("yellow led = note is low , blue led = note is high\n");
         printf("Note B3 \n");
         setLeds(fr, 243.94, 249.94);

         printf("frequency is: %.2f\n\n", fr);
         printf("--------------\n");
         vTaskDelay(pdMS_TO_TICKS(2000));
         resetSampling();
      }
      else if (currentString == 2)
      {
         // String: 3 (G), Freq: 196.00 Hz, Note: G3
         printf("yellow led = note is low , blue led = note is high\n");
         printf("Note G3 \n");
         setLeds(fr, 193.00, 199.00);

         printf("frequency is: %.2f\n\n", fr);
         printf("--------------\n");
         vTaskDelay(pdMS_TO_TICKS(2000));
         resetSampling();
      }
      else if (currentString == 3)
      {
         // String: 4 (D), Freq: 146.83 Hz, Note: D3
         printf("yellow led = note is low , blue led = note is high\n");
         printf("Note D3 \n");
         setLeds(fr, 143.83, 149.83);

         printf("frequency is: %.2f\n\n", fr);
         printf("--------------\n");
         vTaskDelay(pdMS_TO_TICKS(2000));
         resetSampling();
      }
      else if (currentString == 4)
      {
         // String: 5 (A), Freq: 110.00 Hz, Note: A2
         printf("yellow led = note is low , blue led = note is high\n");
         printf("Note A2 \n");
         setLeds(fr, 107.00, 113.00);

         printf("frequency is: %.2f\n\n", fr);
         printf("--------------\n");
         vTaskDelay(pdMS_TO_TICKS(2000));
         resetSampling();
      }
      else if (currentString == 5)
      {
         // String: 6 (E), Freq: 82.41 Hz, Note: E2
         printf("yellow led = note is low , blue led = note is high\n");
         printf("Note E2 \n");
         setLeds(fr, 79.41, 85.41);

         printf("frequency is: %.2f\n\n", fr);
         printf("--------------\n");
         vTaskDelay(pdMS_TO_TICKS(2000));
         resetSampling();
      }
   }
}
