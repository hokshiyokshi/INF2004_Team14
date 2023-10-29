#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "stdio.h"

//Pass the integers to the motors as a variable instead of a value
int quarterPower = 3125;
int halfPower = 6250;
int fullPower = 12500; 

//Define pins to enable/disable power output, int corresponds to GPIO
//pins 1 and 2 are used because pin 0 is already used for PWM
int pin1 = 1;
int pin2 = 2;
//Pin 3 unused to preserve wire clarity during assembly
//Pins 4 and 5 are used since pin 6 is already used for PWM
int pin4 = 4;
int pin5 = 5;

    
    //logic 0 = no power (Low), logic 1 = power (high)
    void moveForward(){
        //Forward for LHS gearbox
        gpio_put(pin1, 0);
        gpio_put(pin2, 1);
        //Forward for RHS gearbox
        gpio_put(pin4, 0);
        gpio_put(pin5, 1);
    }//for moveForward Function

    void moveBackward(){
        //Reverse for LHS gearbox
        gpio_put(pin1, 1);
        gpio_put(pin2, 0);
        //Reverse for RHS gearbox
        gpio_put(pin4, 1);
        gpio_put(pin5, 0);
    }//for moveBackward Function

    //Logic: "lock" the left wheel by setting the logic to "0"
    //Rotate the right wheel forward by setting the logic to pin4: 0, pin5: 1
    void turnLeft(){
        //"lock" the left wheel in place
        gpio_put(pin1, 0);
        gpio_put(pin2, 0);
        //Forward for RHS gearbox
        gpio_put(pin4, 0);
        gpio_put(pin5, 1);
    }//for turnLeft Function


    void turnRight(){
        //Rotate the left wheel forward
        gpio_put(pin1, 0);
        gpio_put(pin2, 1);
        //"lock" the right wheel
        gpio_put(pin4, 0);
        gpio_put(pin5, 0);
    }//for turnRight Function

    //i.e, turn on the spot anticlockwise
    void pivotSteerLeft(){
        //Forward for RHS gearbox
        gpio_put(pin4, 0);
        gpio_put(pin5, 1);
        //Reverse for LHS gearbox
        gpio_put(pin1, 1);
        gpio_put(pin2, 0);
    }//for pivotSteerLeft function


    //i.e, turn on the spot clockwise
    void pivotSteerRight(){
        //Rotate the left wheel forward
        gpio_put(pin1, 0);
        gpio_put(pin2, 1);
        //Reverse for RHS gearbox
        gpio_put(pin4, 1);
        gpio_put(pin5, 0);
    
    }//for pivotSteerRight function


int main() {

    // Tell GPIO 0 they are allocated to the PWM
    gpio_set_function(0, GPIO_FUNC_PWM);
    // Tell GPIO and 6 they are allocated to the PWM
    gpio_set_function(6, GPIO_FUNC_PWM);

    //Set up ground pins for power output (logical 1/0, H/L)
    gpio_init(pin1);
    gpio_init(pin2);
    gpio_init(pin4);
    gpio_init(pin5);
    gpio_set_dir(pin1, GPIO_OUT);
    gpio_set_dir(pin2, GPIO_OUT);
    gpio_set_dir(pin4, GPIO_OUT);
    gpio_set_dir(pin5, GPIO_OUT);


    //NOTE: POV IS LOOKING AT L298N WITH BLACK HEATSINK FACING YOU, BLUE BITS FACING AWAY
    // 0 is the motor for the LHS
    // 3 is the motor for RHS.
    uint slice_num_motor1 = pwm_gpio_to_slice_num(0);
    uint slice_num_motor2 = pwm_gpio_to_slice_num(6);

    // Divide both clocks by 100, so 125Mhz / 100 = 1.25Mhz (1,250,000 Hz)
    pwm_set_clkdiv(slice_num_motor1, 100);
    pwm_set_clkdiv(slice_num_motor2, 100);

    //Set the value at which the controller "wraps around" or becomes 0
    //the original wrap value, 3, results in a very short, high frequency PWM.
    //After changing to 12,500, it changes to a longer, lower frequency PWM.
    //Corresponds to a period of 10ms (12,500/1,250,000 = 0.01s or 10ms)
    pwm_set_wrap(slice_num_motor1, 12500);
    pwm_set_wrap(slice_num_motor2, 12500);
    

    // // Set the duty cycle for both motors
    // // Motor 1 (LHS) 
    // pwm_set_chan_level(slice_num_motor1, PWM_CHAN_A, 6250);
    // // Motor 2 (RHS)
    // pwm_set_chan_level(slice_num_motor2, PWM_CHAN_A, 6250); 

    // Enable both PWM slices
    pwm_set_enabled(slice_num_motor1, true);
    pwm_set_enabled(slice_num_motor2, true);
 

    while (1){
    pwm_set_chan_level(slice_num_motor1, PWM_CHAN_A, quarterPower);
    // Motor 2 (RHS)
    pwm_set_chan_level(slice_num_motor2, PWM_CHAN_A, quarterPower);     
    moveForward();
    //Note: DO NOT USE SLEEP in the final version. Use VtaskDelay as sleep blocks all
    //other tasks from executing
    sleep_ms(2000);


    pwm_set_chan_level(slice_num_motor1, PWM_CHAN_A, halfPower);
    // Motor 2 (RHS)
    pwm_set_chan_level(slice_num_motor2, PWM_CHAN_A, halfPower); 
    moveBackward();
    sleep_ms(2000);
    
    turnLeft();
    sleep_ms(2000);

    turnRight();
    sleep_ms(2000);

    pivotSteerLeft();
    sleep_ms(4000);

    pivotSteerRight();
    sleep_ms(4000);

    }//while loop
//DO NOT PUT ANYTHING HERE, DOES NOT RUN
}//main function
