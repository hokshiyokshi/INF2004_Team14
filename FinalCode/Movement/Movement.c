#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "stdio.h"

//Pass the integers to the motors as a variable instead of a value
//Removes guesswork for powering up of motors.
//based on the lab exercise, power displayed proportional to 12,500 (i.e, n/12500) * 100%
//25, 40, 45, 50, and 100% power respectively. Based on testing, only 45% and above
//will reliably start up.
int twentyFivePercentPower = 3125;
int fourtyPercent = 5300;
int fourtyFivePercentPower = 5625;
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
//MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED
//DECLARATIONS DECLARATIONS DECLARATIONS DECLARATIONS DECLARATIONS DECLARATIONS

//LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR
//Left side (Will use board's ground pin)
//Power pin
int pin14 = 14;
//Feedback pin
int pin15 = 15;

//Right side (Will use board's ground pin)
//power pin
int pin16 = 16;
//feedback pin
int pin17 = 17; 

void stoppu(int power, int duration){
    //GPIO pins initialized directly, not as variables.
    //motor 1 (LHS)
    printf("STOPPED");
    pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
    // Motor 2 (RHS)
    pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
    //Stop
    gpio_put(pin1, 0);
    gpio_put(pin2, 0);
    //Stop
    gpio_put(pin4, 0);
    gpio_put(pin5, 0);

    //Note: DO NOT USE SLEEP in the final version. Use VtaskDelay as sleep blocks all
    //other tasks from executing

    //sleep for designated time
    vTaskDelay(duration);
}//for stop Function

void moveForward(int power, int duration){
    //GPIO pins initialized directly, not as variables.
    //motor 1 (LHS)
    printf("FORWARD");
    pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
    // Motor 2 (RHS)
    pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
    gpio_put(pin1, 0);
    gpio_put(pin2, 1);
    //Forward for RHS gearbox
    gpio_put(pin4, 0);
    gpio_put(pin5, 1);

    //Used VtaskDelay as sleep blocks all other tasks from executing
    //"sleep" for designated time
    vTaskDelay(duration);
}//for moveForward Function

void moveBackward(int power, int duration){
    //motor 1 (LHS)
    printf("\nBACKWARD\n");
    pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
    // Motor 2 (RHS)
    pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
    //Reverse for LHS gearbox
    gpio_put(pin1, 1);
    gpio_put(pin2, 0);
    //Reverse for RHS gearbox
    gpio_put(pin4, 1);
    gpio_put(pin5, 0);

    //"sleep" for designated time
    vTaskDelay(duration);
}//for moveBackward Function

//Logic: "lock" the left wheel by setting the logic to "0"
//Rotate the right wheel forward by setting the logic to pin4: 0, pin5: 1
void turnLeft(int power, int duration){
    //motor 1 (LHS)
    printf("LEFT TURN");
    pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
    // Motor 2 (RHS)
    pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
    //"lock" the left wheel in place
    gpio_put(pin1, 0);
    gpio_put(pin2, 1);
    //Forward for RHS gearbox
    gpio_put(pin4, 0);
    gpio_put(pin5, 0);

    //"sleep" for designated time
    vTaskDelay(duration);
}//for turnLeft Function

void turnRight(int power, int duration){
    printf("LEFT RIGHT");
    //motor 1 (LHS)
    pwm_set_chan_level(pwm_gpio_to_slice_num(0), PWM_CHAN_A, power);
    // Motor 2 (RHS)
    pwm_set_chan_level(pwm_gpio_to_slice_num(6), PWM_CHAN_A, power); 
    //Rotate the left wheel forward
    gpio_put(pin1, 0);
    gpio_put(pin2, 0);
    //"lock" the right wheel
    gpio_put(pin4, 0);
    gpio_put(pin5, 1);

    //"sleep" for designated time
    vTaskDelay(duration);
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

    //"sleep" for designated time
    vTaskDelay(duration);
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
    
    //"sleep" for designated time
    vTaskDelay(duration);
}//for pivotSteerRight function


void lineDetector(void *pvParameters) {

//CORRECT: State 0(LOW) = white
//STATE 1(HIGH) = BLACK
    // printf("connect status: no ip \n connect status: link up\n Connected!\n IP Address: 172.20.10.13 \n Netmask: 255.255.255.240 \n Gateway: 172.20.10.1\n");  
    while (1){
        if(global_distance_cm<30){
            printf("GLOBAL DIST <30\n");
            printf("%f\n", global_distance_cm);
            stoppu(0,1500);
            moveBackward(fourtyPercent, 250);
            continue;
        }
        else if(global_distance_cm>30){
            printf("GLOBAL DIST >30\n");
            printf("%f\n", global_distance_cm);
        }

        if(gpio_get(15) == 0 && gpio_get(17) == 0){
            printf("state 0 W");
            moveForward(fourtyPercent,250);
            }//if
        else if (gpio_get(pin15)==1){
            printf("state 1 B");
        turnRight(fourtyPercent, 250);
        }//else if
        else if (gpio_get(pin17) ==1){
            printf("state 1 B(17)");
            turnLeft(fourtyPercent, 250);
        }//else if
                else if (gpio_get(15) == 1 && gpio_get(17) == 1){
            moveBackward(fourtyPercent,1000);
            pivotSteerLeft(fourtyPercent,750);
        }
    }//while
}
