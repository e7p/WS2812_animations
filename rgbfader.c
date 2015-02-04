/*
 * rgbfader.c
 * Created: 2015-01-16, e7p
 * 
 * RGB-Fader
 */

#include "Light_WS2812/light_ws2812.h"
#include <util/delay.h>

#define PIXEL_COUNT 30
struct cRGB pixels[PIXEL_COUNT];
struct cRGB pixelfade[PIXEL_COUNT];

#define COLOR_COUNT 7
struct cRGB colors[COLOR_COUNT] = {
	{0x00, 0xFF, 0x00},
	{0xFF, 0xFF, 0x00},
	{0xFF, 0x00, 0x00},
	{0xFF, 0x00, 0xFF},
	{0x00, 0xFF, 0xFF},
	{0x00, 0x00, 0xFF},
	{0xFF, 0xFF, 0xFF}
};

int main() {
	uint8_t color = 0, j = 0;

	while(1) {
		j++;
		for(uint8_t i = 0; i < PIXEL_COUNT; i++) {
			pixels[i] = colors[color];
			if((i - j) % 6 == 0) {
				color++;
				if(color == COLOR_COUNT)
					color = 0;
			}
		}
		uint8_t fade_finished = 0;
		while(fade_finished < PIXEL_COUNT) {
			fade_finished = 0;
			for(uint8_t i = 0; i < PIXEL_COUNT; i++) {
				if(pixelfade[i].r == pixels[i].r && pixelfade[i].g == pixels[i].g && pixelfade[i].b == pixels[i].b) {
					fade_finished++;
				} else {
					if(pixelfade[i].r < pixels[i].r) {
						pixelfade[i].r++;
					} else if(pixelfade[i].r > pixels[i].r) {
						pixelfade[i].r--;
					}
					if(pixelfade[i].g < pixels[i].g) {
						pixelfade[i].g++;
					} else if(pixelfade[i].g > pixels[i].g) {
						pixelfade[i].g--;
					}
					if(pixelfade[i].b < pixels[i].b) {
						pixelfade[i].b++;
					} else if(pixelfade[i].b > pixels[i].b) {
						pixelfade[i].b--;
					}
				}
			}
			ws2812_setleds(pixelfade, PIXEL_COUNT);
		}
	}
}
