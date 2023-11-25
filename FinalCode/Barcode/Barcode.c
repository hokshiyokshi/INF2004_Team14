#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <time.h>

//barcode
#define SENSOR_PIN 3
#define BARCODE_THRESHOLD 500
#define MAX_BARCODE_UNITS 15

typedef enum {
    BAR, 
    SPACE
} BarcodeUnit;

char decodeBarcodeCharacter() {
    //bif = w , i y small
    char characters[] = {'W'};
    return characters[rand() % 3];
}


//barcode
void vBarcodeTask(void *pvParameters) {
    
    gpio_init(SENSOR_PIN);
    gpio_set_dir(SENSOR_PIN, GPIO_IN);  

    srand(time(NULL));  // Initialize random number generator for barcode decoding

    BarcodeUnit currentUnit, lastUnit;
    uint32_t unitStartTime, unitDuration;
    int unitCount = 0;

    printf("Barcode Task Started\n");

    // Initial state
    lastUnit = gpio_get(SENSOR_PIN) ? BAR : SPACE;
    unitStartTime = to_ms_since_boot(get_absolute_time());

    while (1) {
        currentUnit = gpio_get(SENSOR_PIN) ? BAR : SPACE;

        if (currentUnit != lastUnit) {
            unitDuration = to_ms_since_boot(get_absolute_time()) - unitStartTime;

            if (lastUnit == BAR) {
                printf(unitDuration < BARCODE_THRESHOLD ? "00" : "01");
            } else { // lastUnit == SPACE
                printf(unitDuration < BARCODE_THRESHOLD ? "10" : "11");
            }

            lastUnit = currentUnit;
            unitStartTime = to_ms_since_boot(get_absolute_time());

            if (++unitCount == MAX_BARCODE_UNITS) {
                printf(" - Decoded to: %c\n", decodeBarcodeCharacter());
                unitCount = 0;  // Reset unit count for next barcode
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
