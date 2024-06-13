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
When Sparkle starts up it initializes 3 interfaces. Two of them are CDC serial ports, one
is a regular RP2040 reset-over-usb port. CDC serial port 0 is for debugging purposes. Log 
outputs and such. CDC serial port 1 is the control port, all API requests should be sent
there.