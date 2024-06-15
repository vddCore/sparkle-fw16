# What is this?
`sparkle-fw16` is an alternative Framework Laptop 16 LED matrix module firmware.

# Why is this?
I didn't like two things about the original firmware:
1. Has too many useless features.
2. It's written in Rust and my eyes just cannot stand that shit. 
   Call it a skill issue, I don't give a fuck.

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

It's implemented on top of the USB HID stack and exposes 2 feature reports, one for small
requests like configuration and one for larger requests like pushing an entire image to a
LED matrix module. The first byte of every payload denotes the command group and its ID.

## Feature Report `0x03` - `GLITTER_SMALL_REQ`
### Group `0x0-`: system control (`Sc`)
Excluding `Sc_exit_sleep` and `Sc_enter_sleep`, commands in this group will never trigger
the wake-on-command mechanism.

```
0x00 Sc_reboot_bootloader <- [void]
  -> [void] _Noreturn
```

```
0x01 Sc_reboot_firmware <- [void]
  -> [void] _Noreturn
```

```
0x02 Sc_enter_sleep <- [void]
  -> [void]
  ```

```
0x03 Sc_exit_sleep <- [void]
  -> [void]
```

```
0x04 Sc_wake_on_command <- <1> [enable: uint8_t]
  -> [void]
  
  WHERE `enable`:
    0x00: disable wake-up on command
    0x01: enable wake-up on command
    
    undefined values are treated like 0x01
```
```
0x05 Sc_ignore_sleep <- <1> [enable: uint8_t]
  -> [void]
  
  WHERE `enable`:
    0x00: disable sleep request ignoring (i.e. honor each sleep request)
    0x01: enable sleep request ignoring
    
    undefined values are treated like 0x01
```

```
0x06 Sc_get_gpio <- [void]
  -> <3> [pin_sleep: uint8_t, pin_dip_1: uint8_t, pin_intb: uint8_t]
  
  WHERE `pin_sleep`: SLEEP# pin (1 = high, 0 = low)
  WHERE `pin_dip_1`: DIP-switch pin (1 = high, 0 = low)
  WHERE `pin_intb`: current INTB pin value (1 = high, 0 = low)
```

```
0x07 Sc_get_state <- [void]
  -> <3> [wake_on_command: uint8_t, sleep_state: uint8_t, ignoring_sleep: uint8_t]

   WHERE `wake_on_command`: whether wake-on-command mechanism is active (1 = on, 0 = off)
   WHERE `sleep_state`: current sleep state
      0x00: awake
      0x01: sleeping by command
      0x02: sleeping by SLEEP# pin
   WHERE `ignoring_sleep`: whether always-on mechanism is active (1 = on, 0 = off)
```

```
0x08-0x0F reserved <- [void]
  -> [void] _Noop
```

### Group `0x1-`: controller configuration (`Cc`)
```
0x10 Cc_get_id <- [void]
  -> <1> [id_byte: uint8_t]
  
  WHERE `id_byte`: embedded LED matrix driver ID
```

```
0x11 Cc_set_global_brightness <- <1> [new_brightness: uint8_t]  
  -> [void]
  
  WHERE `new_brightness`:
    0x00-0xFF: global brightness modulation value to be used
```

```
0x12 Cc_get_global_brightness <- [void]
  -> <1> [current_brightness: uint8_t]
  
  WHERE `current_brightness`:
    0x00-0xFF: global brightness modulation value currently in use
```

```
0x13 Cc_get_dimensions <- [void]
  -> <2> [width: uint8_t, height: uint8_t]
  
  WHERE `width`:
    0x00-0xFF: amount of usable LEDs in horizontal axis (9 for the official Framework LED Matrix)
    
  WHERE `height`:
    0x00-0xFF: amount of usable LEDs in vertical axis (34 for the official Framework LED Matrix)
```

```
0x13-0x1F reserved <- [void]
  -> [void] _Noop
```

### Group `0x2-`: display interaction (`Di`)
> **NOTE**  
> Display coordinates are configured so that the point (0,0) is
> located at the top-left corner of the matrix and the point (8,33) is
> located at the bottom-right corner of the matrix.

```
0x20 Di_set_pixel <- <3> [x: uint8_t, y: uint8_t, value: uint8_t]
  -> [void]
  
  WHERE `x`:
    0x00-0xFF: horizontal axis coordinate of the pixel to be set
  
  WHERE `y`:
    0x00-0xFF: vertical axis coordinate of the pixel to be set
    
  WHERE `value`:
    0x00-0xFF: gray-scale intensity of the pixel to be set
```

```
0x21 Di_get_pixel <- <2> [x: uint8_t, y: uint8_t]
  -> <1> [pixel_value: uint8_t]
  
  WHERE `x`:
    0x00-0xFF: horizontal axis coordinate of the pixel to retrieve
    
  WHERE `y`:
    0x00-0xFF: vertical axis coordinate of the pixel to retrieve
    
  WHERE `pixel_value`:
    0x00-0xFF: current gray-scale intensity of the pixel at the provided coordinates
```

```
0x22 Di_draw_line <- <5> [x1: uint8_t, y1: uint8_t, x2: uint8_t y2: uint8_t, value: uint8_t]
  -> [void]
  
  WHERE `x1`:
    0x00-0xFF: horizontal coordinates of the point at which line drawn starts

  WHERE `y1`:
    0x00-0xFF: vertical coordinates of the point at which line drawn starts
  
  WHERE `x2y2`:
    0x0000-0xFFFF: coordinates of the point at which the line drawn ends
    
  WHERE `value`:
    0x00-0xFF: gray-scale intensity of the line drawn
```

```
0x24-0x2F reserved <- [void]
  -> [void] _Noop
```

## Feature Report `0x04` - `GLITTER_LARGE_REQ`

### Group `0x0-`: system control (`Sc`)
There are no commands utilizing this feature request at the moment.

### Group `0x1-`: controller configuration (`Cc`)
There are no commands utilizing this feature request at the moment.

### Group `0x2-`: display interaction (`Di`)
Requests in this group are ignored when wake-on-command mechanism is inactive 
and the current sleep state is not zero (see `Sc_get_state`).
```
0x23 Di_draw_bitmap <- <306> [pixels: uint8_t*]
  -> [void]
  
  WHERE `pixels`:
    0x00-0xFF: an array of gray-scale intensity values 
               representing the entire LED matrix grid
```