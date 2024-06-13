add_compile_definitions(-DCFG_TUSB_CONFIG_FILE="${CMAKE_SOURCE_DIR}/src/usbcomm/tusb_config.h")

add_executable(${PROJECT_NAME}
    is3741.c
    is3741.h
    is3741_lut.c
    log.h
    main.c
    sparkle.c
    sparkle.h
    usbcomm/tusb_config.h
    usbcomm/usb_descriptors.c
        usbcomm/usb_stdio.c
        usbcomm/usb_stdio.h
        usbcomm/usb_reset_interface.c
        usbcomm/usb_reset_interface.h
        usbcomm/usb_control.c
        usbcomm/usb_control.h
        usbcomm/usb_task.c
        usbcomm/usb_task.h
)

# pull in common dependencies
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_unique_id
    hardware_i2c
    tinyusb_board
    tinyusb_device
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

# add_compile_definitions(-DUSBD_VID=0x32AC)
# add_compile_definitions(-DUSBD_PID=0x0020)
# add_compile_definitions(-DUSBD_DESC_STR_MAX=48)
# add_compile_definitions(-DUSBD_MANUFACTURER="Framework Laptop Inc")
# add_compile_definitions(-DUSBD_PRODUCT="LED Matrix Input Module | SPARKLE")

pico_enable_stdio_usb(sparkle_fw16 1)
pico_enable_stdio_uart(sparkle_fw16 0)