// remember to include these dependencies
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


//GPIO pin 8 and 10 = set as power pin
int pin8 = 8;
int pin11 = 11;
//GPIO pin 9 and 12 = set as Ground pin
int groundPin9 = 9;
int groundPin12 = 12;


float elapsedTimeLeft= 0.0;
float elapsedTimeRight = 0.0;

float distanceLeft = 0.0;
float distanceRight = 0.0;

float speed = 0.0;

void distanceTravelledLeft(){
    //declared as static to retain its value 
    static int turnCountLeft = 0;
    //Radius of the wheel: ~30mm
    //Circumference of the wheel = 2* pi * r = ~ 188.49 ~ 188.5mm
    //188.5 / 20 (Wheel encoder has 20 slots) = 9.425mm per turn 
    distanceLeft += 9.425;

    turnCountLeft += 1;

    //DEBUGGING ONLY
    printf("\n Left wheel Turns: %d", turnCountLeft);
    printf("\n Left wheel Distance: %.2f", distanceLeft);
    //DEBUGGING ONLY


    if (turnCountLeft == 20){
    //print up t 3 d.p
    printf("\nLeft wheel Distance (Millimeters): %.3f", distanceLeft);
    //print the speed for the last 20 turns
    printf("\nSpeed: %.3f", 188.5/elapsedTimeLeft);

    //reset the count to 0
    turnCountLeft = 0;
    //reset elapsedtime to 0.0   
    elapsedTimeLeft = 0.0;
    //reset speed to 0
    speed = 0.0;
    }//for if loop

} //for distanceTravelledLeft Func

//To calculate distance travelled on the right side
void distanceTravelledRight(){
    //declared as static to retain its value 
    static int turnCountRight = 0;
    //Radius of the wheel: ~30mm
    //Circumference of the wheel = 2* pi * r = ~ 188.49 ~ 188.5mm
    //188.5 / 20 (Wheel encoder has 20 slots) = 9.425mm per turn 
    distanceRight += 9.425;

    turnCountRight += 1;

    //DEBUGGING ONLY
    printf("\n Right wheel Turns: %d", turnCountRight);
    printf("\n Right wheel Distance: %.2f", distanceRight);
    //DEBUGGING ONLY


    if (turnCountRight == 20){
    //print up t 3 d.p
    printf("\nRight wheel Distance (Millimeters): %.3f", distanceRight);
    //print the speed for the last 20 turns
    printf("\nRight Speed: %.3f", 188.5/elapsedTimeRight);

    //reset the count to 0
    turnCountRight = 0;
    //reset elapsedtime to 0.0   
    elapsedTimeRight = 0.0;
    //reset speed to 0
    speed = 0.0;
    }//for if loop

} //for distanceTravelledRight Func


//Remember to declare during integration

int main(){

    //initialization for USB cable (To print output)
    stdio_init_all();


    //initialize pins 8, 10 and 9, 11
    gpio_init(pin8);
    gpio_init(groundPin9);
    gpio_init(pin11);
    gpio_init(groundPin12);
    //Enable the pins as "OUT" pins
    gpio_set_dir(pin8, GPIO_OUT);
    gpio_set_dir(groundPin9, GPIO_OUT);
    gpio_set_dir(pin11, GPIO_OUT);
    gpio_set_dir(groundPin12, GPIO_OUT);

    //power to pin 8 and 10
    gpio_put(pin8, 1);
    gpio_put(pin11, 1);
    //cut power to pin 9 and 12
    gpio_put(groundPin9, 0);
    gpio_put(groundPin12, 0);

//Basically,  GPIO 7 is connected to the "OUT" portion of the wheel encoder. 
// Edge rise is set, instead of the original GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL.
//Edge rise = only calls the function when the channel between the wheel encoders are clear.
//E.g, put a card in between the channels, the function is only called when the card is lifted.
//true is to determine if interrupt is enabled or not. 
//&distanceTravelled calls the function when edge rises.
//Same concept with GPIO 13, and each is used for a respective wheel (L and R).
    gpio_set_irq_enabled_with_callback(7, GPIO_IRQ_EDGE_RISE, true, 
    &distanceTravelledLeft);    
    gpio_set_irq_enabled_with_callback(13, GPIO_IRQ_EDGE_RISE, true, 
    &distanceTravelledRight);  

// | GPIO_IRQ_EDGE_FALL
//while true, call function distanceTravelled
//Note: Don't use while(true), it doesn't work
        while (1){ //wait foreverr
            elapsedTimeLeft += 0.1;
            elapsedTimeRight += 0.1;
            sleep_ms(100);
        }//for while loop

//return 0 for main 
    return 0;
}//for main

