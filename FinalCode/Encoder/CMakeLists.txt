add_executable(Encoder
Encoder.c
        )

# pull in common dependencies
target_link_libraries(Encoder pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(Encoder)

pico_enable_stdio_usb(Encoder 1)

# add url via pico_set_program_url
example_auto_set_url(Encoder)
