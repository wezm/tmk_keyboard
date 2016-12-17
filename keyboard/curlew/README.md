Curlew Keyboard
===============

A split grid layout 40% keyboard powered by STM32 and featuring 128x32 OLED display.

Building
--------

Curlew makes use of the [µGFX] graphics library for managing the display. In
addition to the steps described on the
[TMK running on top of ChibiOS][tmk-chibios]
page you also need to grab a copy of µGFX. From the top level directory of TMK:

    git clone https://git.ugfx.io/ugfx/ugfx.git tmk_core/tools/ugfx

[µGFX]: http://ugfx.io/
[tmk-chibios]: https://github.com/tmk/tmk_keyboard/blob/master/tmk_core/protocol/chibios/README.md

With the dependencies in place build with `make`:

    cd keyboard/curlew
    make

Flashing
--------

To load the firmware onto the device the built-in DFU bootloader will be used. Bridge the
BOOT0 and VDD pins and plug in the keyboard, then run [dfu-util]:

    cd keyboard/curlew
    dfu-util -a 0 -s 0x08000000:leave -D build/ch.bin --serial FFFFFFFEFFFF

[dfu-util]: http://dfu-util.sourceforge.net

After flashing, unplug the keyboard, remove the BOOT0 jumper and plug it back in.

**Note:** This process is a work in progress and will be improved.

Debugging
---------

Connect the keyboard via SWD/ST-LINK. Run `openocd`:

    openocd -f interface/stlink-v2.cfg -f target/stm32f0x.cfg

In another window, run gdb:

    cd keyboard/curlew
    arm-none-eabi-gdb -ex 'target remote :3333' build/ch.elf

_To be continued..._
