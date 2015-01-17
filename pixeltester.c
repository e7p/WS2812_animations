/*
 * pixeltester.c
 * Created: 2015-01-16, e7p
 * 
 * Testing single pixels of WS2812 Stripes by submitting numbers over the serial console
 */

#include <avr/io.h>
#include "Light_WS2812/light_ws2812.h"

#define PIXEL_COUNT 150
struct cRGB pixels[PIXEL_COUNT];

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

unsigned char readByte() {
	while(!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void sendByte(unsigned char c) {
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void sendString(char* str) {
	while(*str != '\0') {
		sendByte(*str++);
	}
}

uint16_t readNumber() {
	uint16_t n = 0;
	unsigned char c;
	while(1) {
		c = readByte();
		if(c >= '0' && c <= '9') {
			n = 10 * n + (c - '0');
			sendByte(c);
		} else if(c == '\r') {
			sendString("\r\n");
			break;
		}
	}
	return n;
}

int main() {
	uint16_t led = 0;

	UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;

	while(1) {
		pixels[led].r = 255;
		pixels[led].g = 0;
		pixels[led].b = 0;
		ws2812_setleds(pixels, PIXEL_COUNT);
		pixels[led].r = 0;
		pixels[led].g = 0;
		pixels[led].b = 0;
		led = readNumber();
	}
}