if (EXISTS ${PICO_LWIP_CONTRIB_PATH}/apps/ping/ping.c)

    add_executable(Web
            Web.c
            ${PICO_LWIP_CONTRIB_PATH}/apps/ping/ping.c
    )
    target_compile_definitions(Web PRIVATE
            WIFI_SSID=\"${WIFI_SSID}\"
            WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
            NO_SYS=0            # don't want NO_SYS (generally this would be in your lwipopts.h)
            LWIP_SOCKET=1       # we need the socket API (generally this would be in your lwipopts.h)
            PING_USE_SOCKETS=1
    )
    target_include_directories(Web PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/../.. # for our common lwipopts
            ${PICO_LWIP_CONTRIB_PATH}/apps/ping
    )
    target_link_libraries(Web
            pico_cyw43_arch_lwip_sys_freertos
            pico_stdlib
            pico_lwip_iperf
            FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
    )
    pico_add_extra_outputs(Web)
    pico_enable_stdio_usb(Web 1)
endif()
