/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "lwip/ip4_addr.h"

#include "FreeRTOS.h"
#include "task.h"
#include "ping.h"
#include "message_buffer.h"

#ifndef PING_ADDR
#define PING_ADDR "142.251.35.196"
#endif
#ifndef RUN_FREERTOS_ON_CORE
#define RUN_FREERTOS_ON_CORE 0
#endif

#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )

static MessageBufferHandle_t xControlMessageBuffer;

void main_task(__unused void *params) {
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return;
    }
    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        printf(WIFI_SSID);
        printf(WIFI_PASSWORD);
        exit(1);
    } else {
        printf("Connected.\n");
    }

    while(true) {
        // not much to do as LED is in another task, and we're using RAW (callback) lwIP API
        //vTaskDelay(100);
        // Your main code for WiFi communication goes here

        // Example: Sending a message
        char message_to_send[] = "Test Wifi!"; // Message to send

        xMessageBufferSend(
                xControlMessageBuffer,        // The message buffer to write to
                message_to_send,              // The source of the data to send
                sizeof(message_to_send),      // The length of the data to send
                0                            // Do not block if the buffer is full
        );
    }

    cyw43_arch_deinit();
}

/* A Task that waits for data from the PC/Laptop via message buffer. */
void test_task(__unused void *params) {
    char received_message[64]; // Buffer to store received messages

    while (true) {
        size_t received_bytes;
        received_bytes = xMessageBufferReceive(
                xControlMessageBuffer,       // The message buffer to receive from
                received_message,            // Location to store received data
                sizeof(received_message),    // Maximum number of bytes to receive
                portMAX_DELAY                // Wait indefinitely
        );

        if (received_bytes > 0) {
            received_message[received_bytes] = '\0'; // Null-terminate the received message
            printf("Received message: %s\n", received_message);

            // Process the received message here
            if (strcmp(received_message, "Test!") == 0) {
                printf("Received!\n");
                // Perform an action in response to the received message
            } else if (strcmp(received_message, "Others") == 0) {
                printf("Received others!\n");
                // Handle this specific command
            } else {
                printf("Unknown %s\n", received_message);
                // Handle unknown messages or provide an appropriate response
            }
        }
    }
}
void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(main_task, "TestMainThread", configMINIMAL_STACK_SIZE, NULL, TEST_TASK_PRIORITY, &task);
    TaskHandle_t testtask;
    xTaskCreate(test_task, "TestPCTask", configMINIMAL_STACK_SIZE, NULL, 5, &testtask);

    xControlMessageBuffer = xMessageBufferCreate(64); // Adjust the size according to your needs
    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

int main( void )
{
    stdio_init_all();

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
}
