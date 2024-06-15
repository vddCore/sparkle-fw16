add_compile_definitions(-DCFG_TUSB_CONFIG_FILE="${CMAKE_SOURCE_DIR}/src/include/drivers/usbcomm/tusb_config.h")

add_executable(${PROJECT_NAME}
    drivers/is3741.c
    drivers/led_matrix_lut.c
    drivers/led_matrix.c
    drivers/usbcomm/usb_descriptors.c
    drivers/usbcomm/usb_stdio.c
    drivers/usbcomm/usb_reset_interface.c
    drivers/usbcomm/usb_control.c
    drivers/usbcomm/usb_task.c
    drivers/pins.c
    sparkle.c
    main.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/src/include
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_unique_id
    hardware_i2c
    tinyusb_board
    tinyusb_device
)

pico_add_extra_outputs(${PROJECT_NAME})