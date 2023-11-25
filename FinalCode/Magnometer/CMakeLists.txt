
add_executable(Magnometer
Magnometer.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(Magnometer pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(Magnometer)

# add url via pico_set_program_url
example_auto_set_url(Magnometer)
pico_enable_stdio_usb(Magnometer 1)
