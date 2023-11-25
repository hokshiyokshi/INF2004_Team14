#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include <stdio.h>

//ultrasonic
#define TRIG_PIN 20
#define ECHO_PIN 21

volatile float global_distance_cm = 0.0;

// Ultrasonic Sensor Task
void vUltrasonicSensorTask(void *pvParameters) {        //edited return variable from void to float
    while (1) {
        // Send a trigger pulse
        gpio_put(TRIG_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay for 10 microseconds
        gpio_put(TRIG_PIN, 0);

        // Wait for the echo pulse to start
        while (gpio_get(ECHO_PIN) == 0) {
            // Tight loop contents can be empty or contain minimal processing
        }

        // Measure the pulse duration
        absolute_time_t start_time = get_absolute_time();
        while (gpio_get(ECHO_PIN) == 1) {
            // Tight loop contents can be empty or contain minimal processing
        }
        absolute_time_t end_time = get_absolute_time();

        // Calculate distance in centimeters
        uint32_t pulse_duration = absolute_time_diff_us(start_time, end_time);
        float distance_cm = (pulse_duration * 0.0343) / 2;

        printf("Distance: %.2f cm\n", distance_cm);

        global_distance_cm = distance_cm;

        // while(distance_cm<30){
        //     stoppu(halfPower,250);
        //     printf("Stoppu");
        // }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}
