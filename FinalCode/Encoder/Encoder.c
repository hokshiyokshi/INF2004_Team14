#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


//ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED
//Timer interval, timer will tick every 10^-5 seconds
#define timerInterval 10    

//isRunningLeft = To check the state of the timer, false = not running, true = running
//startTimeLeft = Start time of the left encoder
//elapsedTimeLeft = Time since start for the left encoder
volatile bool isRunningLeft = false;
volatile uint64_t startTimeLeft = 0;
volatile uint64_t elapsedTimeLeft = 0;

volatile bool isRunningRight = false;
volatile uint64_t startTimeRight = 0;
volatile uint64_t elapsedTimeRight = 0;


// volatile float global_distance_cm = 0.0;


// Function to start or stop the timer when the encoder = 1 or = 20
void toggleTimeLeft(uint gpio, uint32_t events);
void toggleTimeRight(uint gpio, uint32_t events);

// Timer interrupt handler to update the elapsed time
bool updateElapsedTimeLeft(repeating_timer_t *rt);
bool updateElapsedTimeRight(repeating_timer_t *rt);
//TIMER PORTION

//Remember to declare during integration
//GPIO pin 8 and 10 = set as power pin
int pin8 = 8;
int pin11 = 11;
//GPIO pin 9 and 12 = set as Ground pin
int groundPin9 = 9;
int groundPin12 = 12;

//testing
//pin 7 is the left
uint pin7 = 7;
//pin 13 is the right
uint pin13 = 13;

float distanceLeft = 0.0;
float distanceRight = 0.0;

float speedLeft = 0.0;
float speedRight = 0.0;
//ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED


void distanceTravelled(uint gpio, uint32_t events) {
    while(1){
        printf("test");
    }//while
    if(gpio==7){

        //declared as static to retain its value 
        static int turnCountLeft = 0;
        //Radius of the wheel: ~30mm
        //Circumference of the wheel = 2* pi * r = ~ 188.49 ~ 188.5mm
        //188.5 / 20 (Wheel encoder has 20 slots) = 9.425mm per turn 
        distanceLeft += 9.425;

        turnCountLeft += 1;

        //For timer: Only start the turncount when encoder registers 1 count
        if (turnCountLeft == 1){
            if (!isRunningLeft){
                // Start the stopwatch if it's not running
                startTimeLeft = time_us_64();
                elapsedTimeLeft = 0;
                isRunningLeft = true;
                }//If stateemnt for checking state of timer
        }//If statement for Overall Timer

        //For when the turnCountLeft Reaches 20 to reset and stop the timer
        else if (turnCountLeft == 20){
            if (isRunningLeft)
        {
            // Stop the timer
            elapsedTimeLeft = time_us_64() - startTimeLeft;
            isRunningLeft = false;
        }//if statement
        else
        {
            // Reset the Timer 
            elapsedTimeLeft = 0;
        }//else statement
        }//Else if for turnCountLeft ==20

        // //DEBUGGING ONLY
        // printf("\n Left wheel time: %lld",elapsedTimeLeft);
        printf("\n Left wheel Turns: %d", turnCountLeft);
        printf("\n Left wheel Distance: %.2f", distanceLeft);
        // //DEBUGGING ONLY

        if (turnCountLeft == 20){
            //print up t 3 d.p
            printf("\nLeft wheel Distance (Millimeters): %.3f", distanceLeft);

            printf("\nElapsed time (left): %lld", (elapsedTimeLeft));
            //print the speed for the last 20 turns
            speedLeft = 188.5/(elapsedTimeLeft / 1000000.00);
            printf("\nSpeed [left] (mm/s): %.4f", speedLeft);

            //reset the count to 0
            turnCountLeft = 0;
            //reset elapsedtime to 0.0  

            //reset speed to 0
            speedLeft = 0.0;
        }//for if loop
    }//for GPIO 7

    if(gpio==13){
        //declared as static to retain its value 
        static int turnCountRight = 0;
        //Radius of the wheel: ~30mm
        //Circumference of the wheel = 2* pi * r = ~ 188.49 ~ 188.5mm
        //188.5 / 20 (Wheel encoder has 20 slots) = 9.425mm per turn 
        distanceRight += 9.425;

        turnCountRight += 1;


        //For timer: Only start the turncount when encoder registers 1 count
        if (turnCountRight == 1){
            if (!isRunningRight){
                // Start the stopwatch if it's not running
                startTimeRight = time_us_64();
                elapsedTimeRight = 0;
                isRunningRight = true;
                }//If stateemnt for checking state of timer
        }//If statement for Overall Timer

        //For when the turnCountLeft Reaches 20 to reset and stop the timer
        else if (turnCountRight == 20){
            if (isRunningRight)
        {
            // Stop the timer
            elapsedTimeRight = time_us_64() - startTimeRight;
            isRunningRight = false;
        }//if statement
        else
        {
            // Reset the Timer 
            elapsedTimeRight = 0;
        }//else statement
        }//Else if for turnCountLeft ==20

        //DEBUGGING ONLY
        // printf("\n Right wheel time: %f", );
        printf("\n Right wheel Turns: %d", turnCountRight);
        printf("\n Right wheel Distance: %.2f", distanceRight);
        //DEBUGGING ONLY

        if (turnCountRight == 20){
            //print up t 3 d.p
            printf("\nRight wheel Distance (Millimeters): %.3f", distanceRight);

            printf("\nElapsed time (Right): %lld", (elapsedTimeRight));
            //print the speed for the last 20 turns
            speedRight = 188.5/(elapsedTimeRight / 1000000.00);
            printf("\nSpeed [Right] (mm/s): %.4f", speedRight);

            //reset the count to 0
            turnCountRight = 0;
            //reset elapsedtime to 0.0   

            //reset speed to 0
            speedRight = 0.0;
        }//for if loop
    }//for GPIO 13
}//for distanceTravelled
//ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED

// Timer interrupt handler to update the elapsed time
bool updateElapsedTimeLeft(repeating_timer_t *rt)
{
    if (isRunningLeft)
    {
        // Calculate the elapsed time if the stopwatch is running
        elapsedTimeLeft = time_us_64() - startTimeLeft;
    }
    return true;
}//timer

bool updateElapsedTimeRight(repeating_timer_t *rt)
{
    if (isRunningRight)
    {
        // Calculate the elapsed time if the stopwatch is running
        elapsedTimeRight = time_us_64() - startTimeRight;
    }
    return true;
}//timer

