#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define MAGNETOMETER_ADDR 0x1E

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 100000);
    i2c_set_slave_mode(I2C_PORT, false, MAGNETOMETER_ADDR);

    sleep_ms(1000);

    gpio_set_pulls(I2C_SDA, true, false);
    gpio_set_pulls(I2C_SCL, true, false);

    i2c_set_baudrate(I2C_PORT, 100000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    uint8_t mode_reg = 0x02;
    uint8_t continuous_mode = 0x00;
    uint8_t single_mode = 0x01;

    uint8_t data[6];
    uint8_t reg = 0x03;

    while (1) {
        // Set continuous measurement mode
        i2c_write_blocking(I2C_PORT, MAGNETOMETER_ADDR, &mode_reg, 1, false);
        i2c_write_blocking(I2C_PORT, MAGNETOMETER_ADDR, &continuous_mode, 1, false);

        sleep_ms(100);  // Wait for measurement to complete

        // Read data from the magnetometer
        i2c_write_blocking(I2C_PORT, MAGNETOMETER_ADDR, &reg, 1, false);

        int result = i2c_read_blocking(I2C_PORT, MAGNETOMETER_ADDR, data, 6, false);
        if (result == PICO_ERROR_GENERIC) {
            printf("I2C read error\n");
            continue;
        }

        int16_t x = (data[0] << 8) | data[1];
        int16_t y = (data[4] << 8) | data[5];
        int16_t z = (data[2] << 8) | data[3];

        printf("X: %d, Y: %d, Z: %d\n", x, y, z);

        // Set single measurement mode for the next reading
        i2c_write_blocking(I2C_PORT, MAGNETOMETER_ADDR, &mode_reg, 1, false);
        i2c_write_blocking(I2C_PORT, MAGNETOMETER_ADDR, &single_mode, 1, false);

        // Adjust the refresh rate as needed
        sleep_ms(1000);  // Refresh every 1 second
    }

    return 0;
}