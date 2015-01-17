WS2812 animations
=================

In this repository you find various programs and animations to use with WS2811, WS2812 or WS2812B LED stripes connected to an AVR microcontroller.
The backend is the lightweight [light_ws2812 V2.2](https://github.com/cpldcpu/light_ws2812/tree/master/light_ws2812_AVR) library made by [cpldcpu](https://github.com/cpldcpu) which is used for the communication to the LED controllers.

I developed some of the animations using an ATmega328P and some using an ATmega1284P, but they should work also on other AVR microcontrollers.
Feel free to test out other controllers and to contribute to the project.

I will follow with a hopefully complete list of existing programs. To build and flash each of them, simply edit the first six lines of the Makefile and change it to your needs.
The specified hex-file can be built using `make all` and be flashed with avrdude using `make flash`. Make sure to specify the correct programmer, programmer port, used microcontroller, and the CPU frequency.

pixeltester.c
-------------

This is a little program for testing and calibration purpose.
While it runs, it waits for input on the UART (9600 baud). Sending a number between 0 and the number of pixels on the stripe will result in a red pixel at this particular position. The number may be terminated by `\r` which should be default using various serial terminals.
With that you can visually see which pixel is at which position.

schaukel.c
----------

This is my first animation. For testing, I hung my new led stripe around a lamp.
I thought about making a cool physical animation with it, because it was hanging around like a curve.
So if you got incidentally a led stripe bending down like a bow or arch you may like this animation if firstly the number of pixels and secondly the starting, ending, and center point of the curve are defined in the C file.

You may also like to watch my setup on [YouTube](http://youtu.be/bFX1fudH1B0).

