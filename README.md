
# notes
* worked as fast as possible, therefore
    * no headers/objects
    * code reuse as copy-paste
    * hardcoded pins
    * no thought for interop with other UNIX systems
    * used arduino for esp32
* tested on raspberry pi model 3 (2015) and esp32c3 devkitc-02

# ioctl
* `blink_led` and `notify_edge` infinitely loop
* gpio pin 17 blinks led
    * expects resistor between pin and led anode (1k ohm worked for me)
* gpio pin 27 is edge notification
* `gpioinfo` is inspired by `libgpiod` program of the same name
    * does not implement any commandline arguments

# i2c
* esp32c3 slave device blinks its onboard led if it receives anything
    to its address (0x42)
* `i2cdetect` is inspired by `i2c-tools` program of the same name
    * does not implement any commandline arguments
* `toggle_led` simply writes a byte to the i2c address 0x42
    * i fully acknowledge how trivial this program is
    * at your request, i could come up with a more complex i2c schema and
        implement that
    * however, i suspect part of the point is that the kernel takes care of
        so much for me

# the setup
![](setup.jpg)

