//include statements for magnometer
#include "FreeRTOS.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <math.h>

//Magnometer memory
#define MAGNETOMETER_I2C_ADDR 0x1E
//I2C port to be used, can be i2c0 or i2c1 (Dependent on the specific pin)
#define I2C_PORT i2c0
//Magnometer pins
#define SDA_PIN 8
#define SCL_PIN 9

double initial_angle = 0.0; // Initialize initial angle to 0 degrees
int16_t prev_x = 0;
int16_t prev_y = 0;
bool update_initial_angle = true; // Flag to update initial angle

//Magnometer task
void vMagnetometerTask(void *pvParameters) {
    // I2C magnetometer initialization
    uint8_t init_data[] = {0x00, 0x70};

    //Essentially block port until measurement is complete
    //I2C port calls the variable, after thatm the address, 
    //initializes the data, 2 = bytes to write, false = don't end with stop i.e,
    //keep taking readings
    i2c_write_blocking(I2C_PORT, MAGNETOMETER_I2C_ADDR, init_data, 2, false);

    // Set magnetometer to Continuous-conversion mode
    uint8_t continuous_mode_data[] = {0x02, 0x00}; // Register 0x02, mode 0x00
    i2c_write_blocking(I2C_PORT, MAGNETOMETER_I2C_ADDR, continuous_mode_data, 2, false);

     // Initialize state variables
    // enum {
    //     MOVE_TO_CORNER,
    //     TURN,
    //     CONTINUE_STRAIGHT
    // } robotState;
    
    // robotState = MOVE_TO_CORNER;

    while (1) {
        // Code to read from the magnetometer
        uint8_t read_cmd = 0x03; // Command to read data
        uint8_t buffer[6]; // Buffer to store the data
        i2c_write_blocking(I2C_PORT, MAGNETOMETER_I2C_ADDR, &read_cmd, 1, true);
        i2c_read_blocking(I2C_PORT, MAGNETOMETER_I2C_ADDR, buffer, 6, false);

        // Process the magnetometer data
        // Convert buffer to meaningful data (depends on the magnetometer)
        int16_t x = (buffer[0] << 8) | buffer[1];
        int16_t y = (buffer[2] << 8) | buffer[3];
        int16_t z = (buffer[4] << 8) | buffer[5];

        // Print the magnetometer data
        printf("Magnetometer X: %d, Y: %d, Z: %d\n", x, y, z);

        // Calculate the angle change (simplified calculation)
        double current_angle = atan2(y, z) * (180.0 / M_PI);

        double angle_change = current_angle - initial_angle;

        // // Implement state-based logic
        // switch (robotState) {
        //     case MOVE_TO_CORNER:
        //         // Move forward toward the corner
        //         // Move forward toward the corner
        //         printf("State: MOVE_TO_CORNER\n");
        //         moveForward(halfPower, 50);
        //         break;

        //     case TURN:
        //         // Rotate the robot 90 degrees (adjust duration as needed)
        //         printf("State: TURN\n");
        //         pivotSteerLeft(halfPower, 80);
        //         break;

        //     case CONTINUE_STRAIGHT:
        //         // Continue moving straight
        //         printf("State: CONTINUE_STRAIGHT\n");
        //         moveForward(halfPower, 80);
        //         break;

        //     default:
        //         break;
        // }


        printf("Angle: %.2f degrees\n", angle_change);

        // Calculate speed, aka change in x and y axes
        int16_t x_change = x - prev_x;
        int16_t y_change = y - prev_y;
        printf("X Change: %d, Y Change: %d\n\n", x_change, y_change);

        prev_x = x;
        prev_y = y;

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }//while loop
}//function
