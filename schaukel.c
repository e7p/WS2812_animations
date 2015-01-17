/*
 * schaukel.c
 * Created: 2015-01-16, e7p
 * 
 * Swing animation for WS2812 led stripes hanging around in a room
 *
        _31       119_
       / |          | \
      /   \        /   \
     /     \      /     \
    /       \_  _/       \
   /          \/          \
  /           75           \
0/                          \149
 */
#include "Light_WS2812/light_ws2812.h"
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926535

#define PIXEL_COUNT 150
struct cRGB pixels[PIXEL_COUNT];

#define COLOR_COUNT 7
struct cRGB colors[COLOR_COUNT] = {
	{0xFF, 0x00, 0x00},
	{0xFF, 0xFF, 0x00},
	{0x00, 0xFF, 0x00},
	{0x00, 0xFF, 0xFF},
	{0x00, 0x00, 0xFF},
	{0xFF, 0x00, 0xFF},
	{0xFF, 0xFF, 0xFF}
};

#define MIN_POS 31
#define MID_POS 75
#define MAX_POS 119

struct character {
	uint16_t waitframes;
	uint16_t pos;
	uint16_t lastpos;
	int8_t phase;
	uint32_t starttime;
};

// CHAR_COUNT can be defined as anything. For now we just use the number of available different colors
#define CHAR_COUNT COLOR_COUNT
struct character chars[CHAR_COUNT];

void resetChar(uint8_t i) {
	if(rand() & 0x01) {
		chars[i].pos = PIXEL_COUNT - 1;
		chars[i].phase = 1;
	} else {
		chars[i].pos = 0;
		chars[i].phase = -1;
	}
	chars[i].lastpos = chars[i].pos;
	chars[i].waitframes = rand() & 0xFFFF;
	chars[i].starttime = 0;
}

int main() {
	for(uint8_t i = 0; i < CHAR_COUNT; i++) {
		resetChar(i);
	}

	uint32_t frames = 0;
	while(1) {
		frames++;
		for(uint8_t i = 0; i < PIXEL_COUNT; i++) {
			pixels[i].r = 0;
			pixels[i].g = 0;
			pixels[i].b = 0;
		}

		for(uint8_t i = 0; i < CHAR_COUNT; i++) {
			if(chars[i].waitframes > 0) {
				chars[i].waitframes--;
			} else {
				chars[i].lastpos = chars[i].pos;
				if(chars[i].pos > MAX_POS) {
					chars[i].pos--;
					chars[i].waitframes = 60;
				} else if(chars[i].pos < MIN_POS) {
					chars[i].pos++;
					chars[i].waitframes = 60;
				} else {
					if(chars[i].starttime == 0) {
						chars[i].starttime = frames;
					}
					double t = (double)(frames-chars[i].starttime)/100;
					if(t > 80) {
						resetChar(i);
						continue;
					}

					// easiest approach to simulate attenuation while in a sinusoid movement
					chars[i].pos = (uint16_t)round((exp(-0.05*t)*sin(PI*t*0.8+0.5*PI*chars[i].phase)*((MAX_POS-MIN_POS)/2))+MID_POS);

					// No waitframes here, as calculations need to be done frequently
					chars[i].waitframes = 0;
				}
			}
			pixels[chars[i].pos].r |= colors[i % COLOR_COUNT].r;
			pixels[chars[i].pos].g |= colors[i % COLOR_COUNT].g;
			pixels[chars[i].pos].b |= colors[i % COLOR_COUNT].b;
			if(chars[i].pos > chars[i].lastpos) {
				for(uint8_t j = 0; j < chars[i].pos - chars[i].lastpos; j++) {
					pixels[chars[i].pos - j].r |= colors[i % COLOR_COUNT].r >> j;
					pixels[chars[i].pos - j].g |= colors[i % COLOR_COUNT].g >> j;
					pixels[chars[i].pos - j].b |= colors[i % COLOR_COUNT].b >> j;
				}
			} else if(chars[i].pos < chars[i].lastpos) {
				for(uint8_t j = 0; j < chars[i].lastpos - chars[i].pos; j++) {
					pixels[chars[i].pos + j].r |= colors[i % COLOR_COUNT].r >> j;
					pixels[chars[i].pos + j].g |= colors[i % COLOR_COUNT].g >> j;
					pixels[chars[i].pos + j].b |= colors[i % COLOR_COUNT].b >> j;
				}
			}
		}

		ws2812_setleds(pixels, PIXEL_COUNT);
	}
}