add_executable(sparkle_fw16
    is3741.c
    is3741.h
    is3741_lut.c
    log.h
    main.c
    sparkle.c
    sparkle.h
        serial.c
        serial.h
)

# pull in common dependencies
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_i2c
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

add_compile_definitions(-DUSBD_VID=0x32AC)
add_compile_definitions(-DUSBD_PID=0x0020)
add_compile_definitions(-DUSBD_DESC_STR_MAX=48)
add_compile_definitions(-DUSBD_MANUFACTURER="Framework Laptop Inc")
add_compile_definitions(-DUSBD_PRODUCT="LED Matrix Input Module | SPARKLE")

pico_enable_stdio_usb(sparkle_fw16 1)
pico_enable_stdio_uart(sparkle_fw16 0)