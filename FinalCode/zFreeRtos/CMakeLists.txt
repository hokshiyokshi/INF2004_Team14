# pull in common dependencies and additional pwm hardware support
# target_link_libraries(ping pico_stdlib hardware_pwm)
# # add in the adc stuff
# target_link_libraries(ping hardware_adc)

if (EXISTS ${PICO_LWIP_CONTRIB_PATH}/apps/ping/ping.c)
    add_executable(picow_freertos_ping_nosys
            picow_freertos_ping.c
            ${PICO_LWIP_CONTRIB_PATH}/apps/ping/ping.c
            )
    target_compile_definitions(picow_freertos_ping_nosys PRIVATE
            WIFI_SSID=\"${WIFI_SSID}\"
            WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
            )
    target_include_directories(picow_freertos_ping_nosys PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/../.. # for our common lwipopts
            ${PICO_LWIP_CONTRIB_PATH}/apps/ping
            )
    target_link_libraries(picow_freertos_ping_nosys
            pico_cyw43_arch_lwip_threadsafe_background
            pico_stdlib
            pico_lwip_iperf
            FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
            #add in PWM for motor
            pico_stdlib hardware_pwm
            # add in ADC
            hardware_adc
            # for line sensors
            hardware_i2c
            )
    pico_add_extra_outputs(picow_freertos_ping_nosys)
    # MUST be added to enable file to create uf2
    pico_enable_stdio_usb(picow_freertos_ping_nosys 1)
    #Disable UART output
    pico_enable_stdio_uart(picow_freertos_ping_nosys 0)


    add_executable(picow_freertos_ping_sys
            picow_freertos_ping.c
            ${PICO_LWIP_CONTRIB_PATH}/apps/ping/ping.c
            )
    target_compile_definitions(picow_freertos_ping_sys PRIVATE
            WIFI_SSID=\"${WIFI_SSID}\"
            WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
            NO_SYS=0            # don't want NO_SYS (generally this would be in your lwipopts.h)
            LWIP_SOCKET=1       # we need the socket API (generally this would be in your lwipopts.h)
            PING_USE_SOCKETS=1
            )
    target_include_directories(picow_freertos_ping_sys PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/../.. # for our common lwipopts
            ${PICO_LWIP_CONTRIB_PATH}/apps/ping
            )
    target_link_libraries(picow_freertos_ping_sys
            pico_cyw43_arch_lwip_sys_freertos
            pico_stdlib
            pico_lwip_iperf
            FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
            #add in PWM for motor
            pico_stdlib hardware_pwm
            # add in ADC
            hardware_adc
            # add in i2c for line sensor
            hardware_i2c
            )
        pico_add_extra_outputs(picow_freertos_ping_sys)
        # MUST be added to enable file to create uf2
        pico_enable_stdio_usb(picow_freertos_ping_sys 1)
        # Disable UART output
        pico_enable_stdio_uart(picow_freertos_ping_sys 0)
endif()
