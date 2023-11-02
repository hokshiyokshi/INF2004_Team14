add_executable(magnometer3
magnometer3.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(magnometer3 pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(magnometer3)

# add url via pico_set_program_url
example_auto_set_url(magnometer3)
pico_enable_stdio_usb(magnometer3 1)