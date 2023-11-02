/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 

#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
*/

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include <stdio.h>

#define TRIG_PIN 15
#define ECHO_PIN 14

int main() {
    stdio_init_all();

    // Initialize GPIO pins
    gpio_init(TRIG_PIN);
    gpio_init(ECHO_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    while (1) {
        // Send a trigger pulse
        gpio_put(TRIG_PIN, 1);
        sleep_us(10);
        gpio_put(TRIG_PIN, 0);

        // Wait for the echo pulse to start
        while (gpio_get(ECHO_PIN) == 0) tight_loop_contents();

        // Measure the pulse duration
        absolute_time_t start_time = get_absolute_time();
        while (gpio_get(ECHO_PIN) == 1) tight_loop_contents();
        absolute_time_t end_time = get_absolute_time();

        // Calculate distance in centimeters
        uint32_t pulse_duration = absolute_time_diff_us(start_time, end_time);
        float distance_cm = (pulse_duration * 0.0343) / 2;

        printf("Distance: %.2f cm\n", distance_cm);

        sleep_ms(1000);  // Wait for 1 second before taking another reading
    }

    return 0;
}
