//include other files like barcode, encoder etc etc
#include "freertos/Barcode/Barcode.c"
#include "freertos/Encoder/Encoder.c"
#include "freertos/Magnometer/Magnometer.c"
#include "freertos/Movement/Movement.c"
#include "freertos/Ultrasonic/Ultrasonic.c"

//standard include statements
#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "lwip/ip4_addr.h"

#include "FreeRTOS.h"
#include "task.h"
#include "ping.h"
#include "message_buffer.h"

#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"

//magnometer 
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "hardware/i2c.h"
#include <math.h>

//Wifi
#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"

#define mbaTASK_MESSAGE_BUFFER_SIZE       ( 60 )

#ifndef PING_ADDR
#define PING_ADDR "142.251.35.196"
#endif
#ifndef RUN_FREERTOS_ON_CORE
#define RUN_FREERTOS_ON_CORE 0


#endif

//Macro to define task priority
#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )

//declare task
void vHttpServerTask(void *pvParameters) {
    // // WIFI Credentials - take care if pushing to github!

    // // Connect to Wi-Fi
    // cyw43_arch_init();
    // cyw43_arch_enable_sta_mode();

    // while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0) {
    //     printf("Attempting to connect...\n");
    // }

    // printf("Connected!\n");

    // // Print network information
    // //ip4_addr_t ipaddr, netmask, gw;
    // struct netif *netif = netif_default;
    // if (netif) {
    //     printf("IP Address: %s\n", ip4addr_ntoa(&netif->ip_addr));
    //     printf("Netmask: %s\n", ip4addr_ntoa(&netif->netmask));
    //     printf("Gateway: %s\n", ip4addr_ntoa(&netif->gw));
    // } else {
    //     printf("Network interface not found\n");
    // }

    // // Initialize web server



    // while (1) {
    //     // Add any additional HTTP server-related functionality or handling here
    //     vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1 second

    // }
}
//FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS 


//TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS
//ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED
void encoderInterrupt(void *pvParameters) {
    // Unused (void) to avoid compiler warnings about an unused parameter
    (void)pvParameters;
    
    // Your code here, possibly using parameters if required
    gpio_set_irq_enabled_with_callback(7, GPIO_IRQ_EDGE_RISE, true, &distanceTravelled);
    gpio_set_irq_enabled(13, GPIO_IRQ_EDGE_RISE, true);
    while(1){
        //run the check for interrupt forever
        //yield to other tasks
        vTaskDelay(10);
    }//while
}//encoderInterrupt
//ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED

// //LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR
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
    // Suppress unused parameter compiler warning 

// //LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR


//TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS TASKS



//CREATE TASKS CREATE TASKS CREATE TASKS CREATE TASKS CREATE TASKS CREATE TASKS CREATE TASKS CREATE 
void vLaunch( void) {
//note to self: ALL TASKS MUST BE INDEXED SEQUENTIALLY! 1,2,3 NOT 3,2,1 OR ERRORS

TaskHandle_t lineDetector_;
xTaskCreate(lineDetector, "Detect", configMINIMAL_STACK_SIZE, NULL, 1, &lineDetector_);
TaskHandle_t Ultrasonic_;
xTaskCreate(vUltrasonicSensorTask, "Ultrasonic Sensor Task", configMINIMAL_STACK_SIZE, 
NULL, 2, &Ultrasonic_);
TaskHandle_t Magnometer_;
xTaskCreate(vMagnetometerTask, "Magnetometer Task", configMINIMAL_STACK_SIZE, NULL, 3, &Magnometer_);
TaskHandle_t encoder_;    
xTaskCreate(encoderInterrupt, "Distance", configMINIMAL_STACK_SIZE, NULL, 4, &encoder_);
TaskHandle_t Barcode__;
xTaskCreate(vBarcodeTask, "Detect", configMINIMAL_STACK_SIZE, NULL, 5, &Barcode__);
    //     TaskHandle_t httpServerTask;
    // xTaskCreate(vHttpServerTask, "HTTP Server Task", configMINIMAL_STACK_SIZE, NULL, 6, &httpServerTask);



//CREATE TASKS CREATE TASKS CREATE TASKS CREATE TASKS CREATE TASKS CREATE TASKS CREATE TASKS CREATE 

#if NO_SYS && configUSE_CORE_AFFINITY && configNUM_CORES > 1
    // we must bind the main task to one core (well at least while the init is called)
    // (note we only do this in NO_SYS mode, because cyw43_arch_freertos
    // takes care of it otherwise)
    vTaskCoreAffinitySet(task, 1);
#endif

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}//vTask

int main( void ){
//enable USB output
stdio_init_all();


//ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED
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

//magnometer
i2c_init(I2C_PORT, 100000); // Initialize I2C at 100 kHz
gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
gpio_pull_up(SDA_PIN);
gpio_pull_up(SCL_PIN);

// Initialize GPIO pins for ultrasonic sensor
gpio_init(TRIG_PIN);
gpio_init(ECHO_PIN);
gpio_set_dir(TRIG_PIN, GPIO_OUT);
gpio_set_dir(ECHO_PIN, GPIO_IN);

//power to pin 8 and 10
gpio_put(pin8, 1);
gpio_put(pin11, 1);
//cut power to pin 9 and 12
gpio_put(groundPin9, 0);
gpio_put(groundPin12, 0);

repeating_timer_t timerLeft;
repeating_timer_t timerRight;
add_repeating_timer_ms(timerInterval, updateElapsedTimeLeft, NULL, &timerLeft);
add_repeating_timer_ms(timerInterval, updateElapsedTimeRight, NULL, &timerRight);
//ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED ENCODER RELATED



//MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED
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
//MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED MOTOR RELATED



//LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR
//14 and 16 = power, 15 and 17 = input
gpio_init(pin14);
gpio_init(pin15);
gpio_init(pin16);
gpio_init(pin17);
//power to pins 14 and 16 (15 and 17 are for feedback)
gpio_set_dir(pin14, GPIO_OUT);
gpio_set_dir(pin16, GPIO_OUT);
gpio_put(pin14, 1);
gpio_put(pin16, 1);
//set 15 and 17 to accept inputs
gpio_set_dir(pin15, GPIO_IN);
gpio_set_dir(pin17, GPIO_IN);
//LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR LINE DETECTOR




/* Configure the hardware ready to run the demo. */
    const char *rtos_name;
#if ( portSUPPORT_SMP == 1 )
    rtos_name = "FreeRTOS SMP";
#else
    rtos_name = "FreeRTOS";
#endif

#if ( portSUPPORT_SMP == 1 ) && ( configNUM_CORES == 2 )
    printf("Starting %s on both cores:\n", rtos_name);
    vLaunch();
#elif ( RUN_FREERTOS_ON_CORE == 1 )
    printf("Starting %s on core 1:\n", rtos_name);
    multicore_launch_core1(vLaunch);
    while (true);
#else
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();
#endif
    return 0;
}//main
