add_executable(sparkle_fw16
        is3741.c
        is3741.h
        is3741_lut.c
        log.h
        main.c
        sparkle.c
        sparkle.h
)

pico_enable_stdio_usb(sparkle_fw16 1)
pico_enable_stdio_uart(sparkle_fw16 0)

# pull in common dependencies
target_link_libraries(
    sparkle_fw16 
    pico_stdlib
    hardware_i2c
)

# create map/bin/hex file etc.
pico_add_extra_outputs(sparkle_fw16)