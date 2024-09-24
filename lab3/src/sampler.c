#include <stdio.h>
#include <limits.h>
#include "sampler.h"
#include <driver/gptimer.h>
#include <esp_task_wdt.h>
#include <esp_adc/adc_oneshot.h>
#include <soc/adc_channel.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <stdbool.h>
#include "esp_err.h"
#include "esp_etm.h"
#include "esp_log.h"
#include <rom/ets_sys.h>

static gptimer_handle_t timer_handle;
static adc_oneshot_unit_handle_t adc_handle; // the handle is used to keep a reference to the ADC
static gptimer_handle_t gptimer = NULL;      // this will contain the reference to the timer instance

static int sampling_freq;
static bool sampling_started = false; // Variable to track if sampling has started
volatile unsigned int counter = 0;

/* The timer callback.
The callback will be invoked in ISR context, so user shouldn’t put any blocking API in this function.
The IRAM_ATTR tells the linker to place this code into RAM instead of the Flash ROM memory.
*/
// int average = 1387;
volatile int sample; // current sample
int crossing = 0;
static int first_sample = 0; // Variable to store the first sample value
static int prevSample = 0;   // last sample

float currentTime = 0; // timeNow
float startTime = 0;   // timeStart

static bool IRAM_ATTR timerISR(gptimer_handle_t timer, const gptimer_alarm_event_data_t edata, void *user_data)
{

    // Read the current sample from the ADC
    adc_oneshot_read(adc_handle, ADC1_GPIO32_CHANNEL, &sample);

    // Check if this is the first sample (prevSample not initialized)
    if (prevSample == -1)
    {
        // Record the start time for measurement
        startTime = esp_timer_get_time();
    }
    else
    {
        // Check for zero-crossing events
        if ((prevSample < 1200) && (sample >= 1200))
        {
            // A rising edge was detected
            crossing++;
        }
        else if ((prevSample >= 1200) && (sample < 1200))
        {
            // A falling edge was detected
            crossing++;
        }
    }

    // Update prevSample for the next iteration
    prevSample = sample;

    // Return 0 to indicate successful execution
    return 0;
}

void startSampling(int freq)
{
    ///////////////////////////// Initialize ADC //////////////////////////////

    // configuration of the ADC
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,            // select the unit
        .ulp_mode = ADC_ULP_MODE_DISABLE, // do not use the ULP
    };

    // se use the "one shot" mode
    esp_err_t result = adc_oneshot_new_unit(&init_config, &adc_handle);
    ESP_ERROR_CHECK(result);

    // configure the channel, we need to choose the resolution and the attenuation
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12, // use full 12 bits width
        .atten = ADC_ATTEN_DB_11,    // set attenuation to support full scale voltage
    };
    result = adc_oneshot_config_channel(adc_handle, ADC1_GPIO32_CHANNEL, &config);
    ESP_ERROR_CHECK(result);

    // let's setup a pull-up on that pin
    // this must be called after initialisation of the ADC or it will be ignored
    // result = gpio_set_pull_mode(GPIO_NUM_32, GPIO_PULLUP_ONLY);
    // ESP_ERROR_CHECK(result);

    //////////////////////Timer config//////////////////////////////////

    // confuguration of the timer
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT, // default clock, 80MHz
        .direction = GPTIMER_COUNT_UP,      // count up at each tick
        .resolution_hz = 10 * freq,         //
    };

    // configure the timer
    esp_err_t resultTimer = gptimer_new_timer(&timer_config, &gptimer);
    ESP_ERROR_CHECK(resultTimer);

    // configuration of the alarm
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = 20,                  // this is like the compare register.
        .flags.auto_reload_on_alarm = true, // sets auto-reload to true
        .reload_count = 0,                  // restart from 0 each time the alarm fires
    };

    esp_err_t resultAlarm = gptimer_set_alarm_action(gptimer, &alarm_config);
    ESP_ERROR_CHECK(resultAlarm);

    gptimer_event_callbacks_t cbs = {
        .on_alarm = timerISR,
    };
    resultAlarm = gptimer_register_event_callbacks(gptimer, &cbs, NULL);
    ESP_ERROR_CHECK(resultAlarm);

    // enable the timer
    resultTimer = gptimer_enable(gptimer);
    ESP_ERROR_CHECK(resultTimer);
    // start the timer
    resultTimer = gptimer_start(gptimer);
    ESP_ERROR_CHECK(resultTimer);
}

void stopSampling()
{
    esp_err_t resultADC = adc_oneshot_del_unit(adc_handle);
    ESP_ERROR_CHECK(resultADC);

    esp_err_t resultTimer = gptimer_stop(gptimer);
    ESP_ERROR_CHECK(resultTimer);

    // Disable the timer (even though gptimer_del_timer should disable it too)
    resultTimer = gptimer_disable(gptimer);
    ESP_ERROR_CHECK(resultTimer);

    // Delete the timer instance
    resultTimer = gptimer_del_timer(gptimer);
    ESP_ERROR_CHECK(resultTimer);
}

void resetSampling()
{
    crossing = 0;
    prevSample = -1;
}

float getFrequency()
{
    // Get the current time using the ESP32's timer
    float currentTime = esp_timer_get_time();

    // Calculate the duration in seconds by subtracting the start time from the current time
    // and dividing by 1,000,000 to convert from microseconds to seconds
    float duration = (float)(currentTime - startTime) / 1000000;

    // Calculate the average frequency by dividing the number of "crossings" by
    // (2 * duration) where 2 represents two cycles per crossing (rising and falling edges)
    float average = (float)(crossing) / (2.0 * duration);

    // Return the calculated average frequency
    return average;
}
