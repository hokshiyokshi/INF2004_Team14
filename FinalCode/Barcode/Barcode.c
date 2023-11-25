#include "FreeRTOS.h" 
#include "pico/stdlib.h" 
#include <stdio.h> 
#include <time.h> 

// Pin where our barcode sensor is connected
#define SENSOR_PIN 3
// How we tell narrow stuff from wide stuff in the barcode
#define BARCODE_THRESHOLD 500
// Each barcode character has 9 bits (like 010011001)
#define MAX_BARCODE_UNITS 9

// This is to keep track of what we're reading: a narrow bar, a wide bar, etc.
typedef enum {
    NARROW_BAR,
    WIDE_BAR,
    NARROW_SPACE,
    WIDE_SPACE
} BarcodeUnit;

// This links each character (like 'A') to its barcode pattern ("100001001")
typedef struct {
    char character;
    char pattern[10]; // 9 for the pattern + 1 for the string end
} Code39Char;

// The patterns for all letters and the asterisk in Code 39
Code39Char code39Alphabet[] = {
    {'*', "010010100"}, {'A', "100001001"}, {'B', "001001001"},
    {'C', "101001000"}, {'D', "000011001"}, {'E', "100011000"},
    {'F', "001011000"}, {'G', "000001101"}, {'H', "100001100"},
    {'I', "001001100"}, {'J', "000011100"}, {'K', "100000011"},
    {'L', "001000011"}, {'M', "101000010"}, {'N', "000010011"},
    {'O', "100010010"}, {'P', "001010010"}, {'Q', "000000111"},
    {'R', "100000110"}, {'S', "001000110"}, {'T', "000010110"},
    {'U', "110000001"}, {'V', "011000001"}, {'W', "111000000"},
    {'X', "010010001"}, {'Y', "110010000"}, {'Z', "011010000"}
};

// This function figures out which letter a pattern matches
char decodeBarcodeCharacter(char *pattern) {
    for (int i = 0; i < sizeof(code39Alphabet) / sizeof(Code39Char); ++i) {
        if (strcmp(pattern, code39Alphabet[i].pattern) == 0) {
            return code39Alphabet[i].character; // Found the matching letter
        }
    }
    return '?'; // We return '?' if we don't recognize the pattern
}

// The main task for scanning barcodes
void vBarcodeTask(void *pvParameters) {
    gpio_init(SENSOR_PIN); // Start the pin
    gpio_set_dir(SENSOR_PIN, GPIO_IN); // Set the pin to read data (input)

    // Some variables to keep track of what we're reading
    BarcodeUnit currentUnit, lastUnit;
    uint32_t unitStartTime, unitDuration;
    int unitCount = 0;
    char pattern[10]; // Here we'll build the pattern we're reading
    bool scanningBarcode = false; // True when we're in the middle of scanning a barcode

    printf("Barcode Task Started\n"); // Just saying we started

    // We start by reading the sensor: is it a bar or a space?
    lastUnit = gpio_get(SENSOR_PIN) ? NARROW_BAR : NARROW_SPACE;
    unitStartTime = to_ms_since_boot(get_absolute_time());

    while (1) { // Keep doing this forever
        currentUnit = gpio_get(SENSOR_PIN) ? NARROW_BAR : NARROW_SPACE;

        // Check if we've moved from a bar to a space or vice versa
        if (currentUnit != lastUnit) {
            unitDuration = to_ms_since_boot(get_absolute_time()) - unitStartTime;
            pattern[unitCount] = unitDuration < BARCODE_THRESHOLD ? '0' : '1';
            lastUnit = currentUnit;
            unitStartTime = to_ms_since_boot(get_absolute_time());

            // If we've got a full character (9 units), let's decode it
            if (++unitCount == MAX_BARCODE_UNITS) {
                pattern[9] = '\0'; // Close off our string
                char decodedChar = decodeBarcodeCharacter(pattern);

                // If it's an asterisk, we're starting or ending a barcode
                if (decodedChar == '*') {
                    if (!scanningBarcode) {
                        scanningBarcode = true; // Start scanning
                    } else {
                        scanningBarcode = false; // Stop scanning
                        printf("\nBarcode End\n"); // We're done with this barcode
                    }
                } else if (scanningBarcode) {
                    printf("Decoded: %c ", decodedChar); // Show the letter we found
                }

                unitCount = 0; // Ready to read the next character
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // Wait a bit before checking the sensor again
    }
}
