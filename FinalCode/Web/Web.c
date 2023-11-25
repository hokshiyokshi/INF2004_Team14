#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"

//declare task
void vHttpServerTask(void *pvParameters) {
    // WIFI Credentials - take care if pushing to github!

    // Connect to Wi-Fi
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();

    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0) {
        printf("Attempting to connect...\n");
    }

    printf("Connected!\n");

    // Print network information
    //ip4_addr_t ipaddr, netmask, gw;
    struct netif *netif = netif_default;
    if (netif) {
        printf("IP Address: %s\n", ip4addr_ntoa(&netif->ip_addr));
        printf("Netmask: %s\n", ip4addr_ntoa(&netif->netmask));
        printf("Gateway: %s\n", ip4addr_ntoa(&netif->gw));
    } else {
        printf("Network interface not found\n");
    }

    // Initialize web server



    while (1) {
        // Add any additional HTTP server-related functionality or handling here
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1 second

    }
}

