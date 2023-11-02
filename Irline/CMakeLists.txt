if (TARGET tinyusb_device)
    add_executable(irline_sensor
            irline_sensor.c
            )

    # pull in common dependencies
    target_link_libraries(irline_sensor pico_stdlib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(irline_sensor 1)
    pico_enable_stdio_uart(irline_sensor 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(irline_sensor)

    # add url via pico_set_program_url
    example_auto_set_url(irline_sensor)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building irline_sensor because TinyUSB submodule is not initialized in the SDK")
endif()
