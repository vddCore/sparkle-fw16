add_compile_definitions(-DCFG_TUSB_CONFIG_FILE="${CMAKE_SOURCE_DIR}/src/usbcomm/tusb_config.h")

add_executable(${PROJECT_NAME}
    is3741.c
    is3741.h
    led_matrix_lut.c
    led_matrix.c
    led_matrix.h
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

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_unique_id
    hardware_i2c
    tinyusb_board
    tinyusb_device
)

pico_add_extra_outputs(${PROJECT_NAME})