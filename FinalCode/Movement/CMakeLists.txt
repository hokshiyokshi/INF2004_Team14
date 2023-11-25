#cmakelists for motor
add_executable(Movement
        Movement.c
        )

# pull in common dependencies and additional pwm hardware support
target_link_libraries(Movement pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(Movement)

# for USB use and stuff
pico_enable_stdio_usb(Movement 1)

# add url via pico_set_program_url
example_auto_set_url(Movement)

#cmakelists for line sensor
pico_enable_stdio_uart(Movement 0)
