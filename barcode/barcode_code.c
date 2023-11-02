#include "pico/stdlib.h"
#include <stdio.h>

#define IR_SENSOR_PIN 3
#define TRANSITION_THRESHOLD 500  // This threshold determines the barcode is wide or narrow.  Have to adjust according to the speed
typedef enum {
    BLACK, 
    WHITE
} Color;

int main() {
    stdio_init_all();
    gpio_init(IR_SENSOR_PIN);
    gpio_set_dir(IR_SENSOR_PIN, GPIO_IN);  

    Color currentColor, lastColor;
    uint32_t colorStartTime, colorDuration;

    // Initial state
    lastColor = gpio_get(IR_SENSOR_PIN) ? BLACK : WHITE;
    colorStartTime = to_ms_since_boot(get_absolute_time());

    while (1) {
        currentColor = gpio_get(IR_SENSOR_PIN) ? BLACK : WHITE;

        if (currentColor != lastColor) {
            // Calculate duration for which the last color lasted
            colorDuration = to_ms_since_boot(get_absolute_time()) - colorStartTime;

            // Based on duration, determine if segment was narrow or wide
            if (colorDuration < TRANSITION_THRESHOLD) {
                printf("%s Narrow\n", (lastColor == BLACK) ? "Black" : "White");
            } else {
                printf("%s Wide\n", (lastColor == BLACK) ? "Black" : "White");
            }

            // Reset for the next color segment
            lastColor = currentColor;
            colorStartTime = to_ms_since_boot(get_absolute_time());
        }

        sleep_ms(50);  // Check for color transitions every 50ms. might have to adjust depending on speed
    }

    return 0;
}
