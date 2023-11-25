#include "FreeRTOS.h"  
#include <stdio.h>     
#include "pico/stdlib.h"  
#include "hardware/i2c.h" 
#include <math.h>         

// Magnetometer configuration
#define MAGNETOMETER_I2C_ADDR 0x1E 
//I2C port to be used, can be i2c0 or i2c1 (Dependent on the specific pin)
#define I2C_PORT i2c1
//Magnometer pins 26 and 27 SDA and SCL respectively
#define SDA_PIN 26                 
#define SCL_PIN 27                 

double initial_angle = 0.0;  // Initialize the initial angle to 0 degrees
int16_t prev_x = 0;          // Initialize previous x-coordinate to 0
int16_t prev_y = 0;          // Initialize previous y-coordinate to 0
bool update_initial_angle = true; // Flag for updating the initial angle

// Function for magnetometer task
void vMagnetometerTask(void *pvParameters) {
    // I2C magnetometer initialization data
    uint8_t init_data[] = {0x00, 0x70};
    // Send initialization data to magnetometer
    i2c_write_blocking(I2C_PORT, MAGNETOMETER_I2C_ADDR, init_data, 2, false);

    // Set magnetometer to Continuous-conversion mode
    uint8_t continuous_mode_data[] = {0x02, 0x00}; // Data for continuous mode
    i2c_write_blocking(I2C_PORT, MAGNETOMETER_I2C_ADDR, continuous_mode_data, 2, false);

    // Robot state enum and initialization
    enum {
        MOVE_TO_CORNER,
        TURN,
        CONTINUE_STRAIGHT
    } robotState;
    robotState = MOVE_TO_CORNER; // Start with MOVE_TO_CORNER state

    while (1) {
        // Read data from magnetometer
        uint8_t read_cmd = 0x03; // Command to read data
        uint8_t buffer[6];       // Buffer to store data
        i2c_write_blocking(I2C_PORT, MAGNETOMETER_I2C_ADDR, &read_cmd, 1, true);
        i2c_read_blocking(I2C_PORT, MAGNETOMETER_I2C_ADDR, buffer, 6, false);

        // Process magnetometer data
        int16_t x = (buffer[0] << 8) | buffer[1]; 
        int16_t y = (buffer[2] << 8) | buffer[3]; 
        int16_t z = (buffer[4] << 8) | buffer[5]; 

        // Print magnetometer x y z
        printf("Magnetometer X: %d, Y: %d, Z: %d\n", x, y, z);

        // Calculate angle change
        double current_angle = atan2(y, z) * (180.0 / M_PI); // Calculate current angle
        double angle_change = current_angle - initial_angle;  // Calculate angle change

        // Implement state-based logic
        switch (robotState) {
            case MOVE_TO_CORNER:
                // Move forward toward the corner
                printf("State: MOVE_TO_CORNER\n");
                // moveForward(halfPower, 50); // Function call to move forward from motor
                break;

            case TURN:
                // Rotate the robot 90 degrees
                printf("State: TURN\n");
                // pivotSteerLeft(halfPower, 80); // Function call to pivot steer left from motor
                break;

            case CONTINUE_STRAIGHT:
                // Continue moving straight
                printf("State: CONTINUE_STRAIGHT\n");
                // moveForward(halfPower, 80); // Function call to move forward from motor
                break;

            default:
                break;
        }
        
        printf("Angle: %.2f degrees\n", angle_change); 

        // Calculate speed (change in x and y coordinates)
        int16_t x_change = x - prev_x; // Calculate change in x
        int16_t y_change = y - prev_y; // Calculate change in y
        printf("X Change: %d, Y Change: %d\n\n", x_change, y_change); 

        prev_x = x; // Update previous x-coordinate
        prev_y = y; // Update previous y-coordinate

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}
