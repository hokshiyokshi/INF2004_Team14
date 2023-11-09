#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <math.h>

// Define constants for I2C configuration
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define MAGNETOMETER_ADDR 0x1E

int main() {
    stdio_init_all();

    // Initialize I2C communication at 100kHz
    i2c_init(I2C_PORT, 100000);

    // Set I2C mode and slave address for magnetometer
    i2c_set_slave_mode(I2C_PORT, false, MAGNETOMETER_ADDR);

    sleep_ms(1000); // Wait for 1 second

    // Enable pull-up resistors for SDA and SCL lines
    gpio_set_pulls(I2C_SDA, true, false);
    gpio_set_pulls(I2C_SCL, true, false);

    // Set I2C baudrate
    i2c_set_baudrate(I2C_PORT, 100000);

    // Set GPIO pins to I2C function
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    // Set MR_REG_M (0x02) to Continuous-conversion mode (0x00) using two-byte sequence over the I2C bus
    uint8_t data[2]; // { register, data }
    data[0] = 0x02; // Address of MR_REG_M
    data[1] = 0x00; // Value to set MD1 and MD0 to 0 for continuous-conversion mode
    i2c_write_blocking(I2C_PORT, MAGNETOMETER_ADDR, data, 2, true);

    uint8_t data_read[6]; // Array to store magnetometer data
    uint8_t reg = 0x03;   // Register to read data from

    double initial_angle = 0.0; // Initialize initial angle to 0 degrees
    int16_t prev_x = 0;
    int16_t prev_y = 0;

    while (1) {
        // Read data from the magnetometer
        i2c_write_blocking(I2C_PORT, MAGNETOMETER_ADDR, &reg, 1, false);

        int result = i2c_read_blocking(I2C_PORT, MAGNETOMETER_ADDR, data_read, 6, false);
        if (result == PICO_ERROR_GENERIC) {
            printf("I2C read error\n");
            continue;
        }

        int16_t x = (data_read[0] << 8) | data_read[1];
        int16_t y = (data_read[4] << 8) | data_read[5]; 
        int16_t z = (data_read[2] << 8) | data_read[3];
        printf("X: %d, Y: %d, Z: %d\n", x, y, z);

        // Calculate the angle change. Tangent of (y / x) in radians. 
        double current_angle = atan2(y, x) * (180.0 / M_PI); // Converts the angle from radians to degrees. M_PI is a constant represents the value of π from math library.
        double angle_change = current_angle - initial_angle; // Calculate change in angle from initial

        printf("Angle change: %.2f degrees\n", angle_change);

        // Calculate change in x and y axes
        int16_t x_change = x - prev_x;
        int16_t y_change = y - prev_y;
        printf("X Change: %d, Y Change: %d\n\n", x_change, y_change);

        prev_x = x;
        prev_y = y;

        // Adjust the refresh rate as needed
        sleep_ms(1000);  // Refresh every 1 second
    }

    return 0;
}
