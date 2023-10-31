#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "stdio.h"

//Pass the integers to the motors as a variable instead of a value
//Removes guesswork for powering up of motors.
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

    //Function to control the power of the motors.
    //Use together with variables in order to provide appropriate power.
    //logic 0 = no power (Low), logic 1 = power (high)
    void moveForward(int power, int duration){
        //GPIO pins initialized directly, not as variables.
        //motor 1 (LHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
        // Motor 2 (RHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
        //Forward for LHS gearbox
        gpio_put(pin1, 0);
        gpio_put(pin2, 1);
        //Forward for RHS gearbox
        gpio_put(pin4, 0);
        gpio_put(pin5, 1);

        //Note: DO NOT USE SLEEP in the final version. Use VtaskDelay as sleep blocks all
        //other tasks from executing

        //sleep for designated time
        sleep_ms(duration);
    }//for moveForward Function

    void moveBackward(int power, int duration){
        //motor 1 (LHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
        // Motor 2 (RHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
        //Reverse for LHS gearbox
        gpio_put(pin1, 1);
        gpio_put(pin2, 0);
        //Reverse for RHS gearbox
        gpio_put(pin4, 1);
        gpio_put(pin5, 0);
        //Note: DO NOT USE SLEEP in the final version. Use VtaskDelay as sleep blocks all
        //other tasks from executing

        //sleep for designated time
        sleep_ms(duration);
    }//for moveBackward Function

    //Logic: "lock" the left wheel by setting the logic to "0"
    //Rotate the right wheel forward by setting the logic to pin4: 0, pin5: 1
    void turnLeft(int power, int duration){
        //motor 1 (LHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
        // Motor 2 (RHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
        //"lock" the left wheel in place
        gpio_put(pin1, 0);
        gpio_put(pin2, 0);
        //Forward for RHS gearbox
        gpio_put(pin4, 0);
        gpio_put(pin5, 1);

        //sleep for designated time
        sleep_ms(duration);
    }//for turnLeft Function

    void turnRight(int power, int duration){
        //motor 1 (LHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
        // Motor 2 (RHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
        //Rotate the left wheel forward
        gpio_put(pin1, 0);
        gpio_put(pin2, 1);
        //"lock" the right wheel
        gpio_put(pin4, 0);
        gpio_put(pin5, 0);

        //sleep for designated time
        sleep_ms(duration);
    }//for turnRight Function

    //i.e, turn on the spot anticlockwise
    void pivotSteerLeft(int power, int duration){
        //motor 1 (LHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
        // Motor 2 (RHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
        //Forward for RHS gearbox
        gpio_put(pin4, 0);
        gpio_put(pin5, 1);
        //Reverse for LHS gearbox
        gpio_put(pin1, 1);
        gpio_put(pin2, 0);

        //sleep for designated time
        sleep_ms(duration);
    }//for pivotSteerLeft function

    //i.e, turn on the spot clockwise
    void pivotSteerRight(int power, int duration){
        //motor 1 (LHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
        // Motor 2 (RHS)
        pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
        //Rotate the left wheel forward
        gpio_put(pin1, 0);
        gpio_put(pin2, 1);
        //Reverse for RHS gearbox
        gpio_put(pin4, 1);
        gpio_put(pin5, 0);
    
        //sleep for designated time
        sleep_ms(duration);
    }//for pivotSteerRight function

int main() {

    //initialization for USB cable (To print output)
    stdio_init_all();
    
    
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
    // 6 is the motor for RHS.
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

    // Enable both PWM slices 
    pwm_set_enabled(slice_num_motor1, true);
    pwm_set_enabled(slice_num_motor2, true);
 

    while (1){
    moveForward(fullPower, 2000);


    moveBackward(halfPower, 4000);
    
    turnLeft(fullPower, 2000);

    turnRight(fullPower, 2000);

    pivotSteerLeft(halfPower, 2000);

    pivotSteerRight(halfPower, 2000);

    }//while loop
//DO NOT PUT ANYTHING HERE, DOES NOT RUN
}//main function
