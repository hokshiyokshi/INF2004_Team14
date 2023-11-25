if (TARGET tinyusb_device)
    add_executable(Ultrasonic
                Ultrasonic.c
            )

    # pull in common dependencies
    target_link_libraries(Ultrasonic pico_stdlib hardware_i2c)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(Ultrasonic 1)
    pico_enable_stdio_uart(Ultrasonic 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(Ultrasonic)

    # add url via pico_set_program_url
    example_auto_set_url(Ultrasonic)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()

