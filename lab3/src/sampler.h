#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <driver/adc.h>
#include <driver/timer.h>
#include <esp_task_wdt.h>

/**
* Starts sampling on a given pin with a given fequency.
* Parameter freq: the sampling frequency
*/
void startSampling(int freq);

/**
* Stops the sampling process.
*/
void stopSampling();

/**
* Resets frequency detection without stopping the sampling process.
*/
void resetSampling();

/**
* Computes the average frequency of the signal that has been (or is being) sampled.
*/
float getFrequency();

#endif