# What is this?
`sparkle-fw16` is an alternative Framework Laptop 16 LED matrix module firmware.

# Why is this?
I didn't like a few things about the original firmware:
1. Has too many useless features.
2. It's written in Rust and my eyes just cannot stand that shit. 
   Call it a skill issue, I don't give a fuck.
3. It handles display interaction in a weird way that breaks my brain.

# How do I build it?
1. Download CMake and Ninja build tools. I'll let you figure this one out yourself.
2. Download [RP2040 Pico SDK](https://github.com/raspberrypi/pico-sdk), install it. 
When that's done, set your `PICO_SDK_PATH` and `PICO_TOOLCHAIN_PATH` environmental 
variables accordingly.
3. After setting up your environment, just navigate to this directory and execute:
```
cmake . -B build/ -G ninja -DCMAKE_BUILD_TYPE=<build_type>
cd build/
ninja
```
Where `<build_type>` is one of the following: `Release`, `RelWithDebInfo`, `Debug`.

The artifact that's been built should be a `.uf2` located somewhere in the build 
directory.

# How do I use it?
1. Take your input module(s) out of the input deck.
2. Switch the input module(s) into BOOTSEL mode by toggling the DIP-switch pin 2 
   into ON position. 
3. Place the module(s) back into the input deck. A mass storage device will appear 
   in your system. 
4. Move the file `.uf2` file into the newly appeared mass storage device. RP2040 
   should pick it up and flash it after a few seconds.
5. Refresh your file manager and you'll find the
   file magically vanished - that's how it confirms the flashing process has completed.
6. Take the input module(s) out again and toggle the DIP-switch pin 2 into OFF position.
7. Replace the input module(s). Congratulations, you're running `sparkle-fw16`.

# How does it work?
When Sparkle starts up it initializes 3 interfaces:
1. USB CDC interface for debugging purposes. All messages are logged there.
2. USB HID interface for control purposes. API requests are sent there.
3. RP2040 Reset interface for easy flashing. The modified Sparkle Picotool makes use of this.

# Glitter Protocol v1.0
The Glitter Protocol is how Sparkle talks to your Framework Laptop 16 and vice-versa.

It's implemented on top of the USB HID stack and exposes 3 HID reports.

## Feature Report `0x01` - `GLITTER_DEVICE_INFO`
Returns the current device information defined by the structure specified below.

> **NOTE**  
> This report only accepts GET_REPORT control transfer. SET_REPORT requests with this ID 
> will be ignored.

```
typedef struct glitter_device_info
{
    uint8_t sleep_pin;         /* 1 = on, 0 = off */
    uint8_t dip1_pin;          /* 1 = on, 0 = off */
    uint8_t intb_pin;          /* 1 = on, 0 = off */
    uint8_t state_flags;       /* see firmware/kernel.h for details */
    uint8_t id_reg;            /* see IS31FL3741A datasheet for details */
    uint8_t config_reg;
    uint8_t global_brightness;
    uint8_t display_width;     /* 9 for the official LED matrix module */
    uint8_t display_height;    /* 34 for the official LED matrix module */
} glitter_device_info_t;
```

## Feature Report `0x02` - `GLITTER_BASIC_CMD`
Executes a user-accessible firmware subroutine. First byte specifies command ID, while
subsequent bytes are used as command arguments where applicable.

> **`0x00` - `GLITTER_CMD_REBOOT(target: uint8_t)`**  
> Reboots the microcontroller into the specified target state.  
> 
> `target`: `0x00` to reboot into BOOTSEL mode, `0x01` to reboot the firmware. Undefined
>           reboot targets will be treated as if they were requests to reboot into firmware.

> **`0x01` - `GLITTER_CMD_SLEEP(enter: uint8_t)`**  
> Tells the firmware to enter or exit sleep mode. 
> 
> `enter`: `0x00` to initiate a wake-up, `0x01` to enter user-induced sleep. This will set the 
>          sleep state flag to the `KERNEL_SLEEPSTATE_COMMAND (0x03)` value.

> **`0x02`- `GLITTER_CMD_WAKE_ON_COMMAND(enable: uint8_t)`**  
> Toggles the integrated wake-on-command mechanism. When wake-on-command is enabled, any draw
> commands or grid control reports received will automaticall wake the LED matrix module up.
> Otherwise, they will be ignored until `GLITTER_CMD_SLEEP(false)` request is issued.
> 
> `enable`: `0x00` to disable the wake-on-command mechanism, `0x01` to enable it.

> **`0x03` - `GLITTER_CMD_SET_SLEEP_TIMEOUT(timeout_ms: int32_t)`**  
> Sets the time, in milliseconds, since last received display draw command that needs to pass
> in order to put the LED matrix is put to sleep.
> 
> `timeout_ms`: Set to `0` to disable the timeout mechanism, set to `-1` to ignore any sleep
>               requests received (including user-induced sleep and the EC sleep pin).

> **`0x04` - `GLITTER_CMD_SET_GLOBAL_BRIGHTNESS(brightness: uint8_t)`**  
> Sets the global LED brightness multiplier.
>
> `brightness`: A new global brightness multiplier to be applied.

> **`0x05` - `GLITTER_CMD_DRAW_PIXEL(x: uint8_t, y: uint8_t, brightness: uint8_t)`**  
> Sets a single pixel.
>
> `x`: The horizontal axis coordinate identifying the LED to be set.  
> `y`: The vertical axis coordinate identifying the LED to be set.  
> `brightness`: The brightness value the selected LED is going to be set to.
>
> **NOTE**  
> Coordinate system starts (point [0, 0]) at the top-right LED, like with classic 2D
> computer graphics engines and frameworks. 

> **`0x06` - `GLITTER_CMD_DRAW_LINE(x1y1: uint16_t, x2y2: uint16_t, brightness: uint8_t)`**  
> Draws a line.
>
> `x1y1`: Coordinate pair at which the line drawn starts.  
> `x2y2`: Coordinate pair at which the line drawn ends.  
> `brightness`: The brightness value of the line drawn.  
>
> **NOTE**  
> The coordinates are encoded in the following way:
> ```
> x = x1y1 >> 8;
> y = x1y1 & 0xFF;
> ```

> **`0x07` - `GLITTER_CMD_CTRLR_RESET(void)`**  
> Resets the IS31FL3741A controller to its default boot state.


## Feature Report `0x03` - `GLITTER_GRID_PWM_CNTL`
This report takes a sequence of 306 bytes representing the entire pixel grid's 
brightness values.

> **NOTE**  
> If the wake-on-command mechanism is disabled, any requests for this report ID
> are ignored by the firmware.

## Feature Report `0x04` - `GLITTER_GRID_DC_SCALE_CNTL`
This report takes a sequence of 306 bytes representing the DC scale values for
every pixel in the grid. Please consult the included IS31FL3741A datasheet for
more details on what this means.