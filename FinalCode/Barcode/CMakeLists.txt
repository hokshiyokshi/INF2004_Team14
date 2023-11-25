if (TARGET tinyusb_device)
    add_executable(Barcode
        Barcode.c
    )

    # pull in common dependencies
    target_link_libraries(Barcode pico_stdlib hardware_adc)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(Barcode 1)
    pico_enable_stdio_uart(Barcode 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(Barcode)

    # add url via pico_set_program_url
    example_auto_set_url(Barcode)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()
