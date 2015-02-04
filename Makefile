MMCU=atmega1284p
F_CPU=8000000
PROG=usbasp
PROG_PORT=usb

TARGET=rgbfader.hex

OBJ=Light_WS2812/light_ws2812.o

FORMAT=ihex
OBJCOPY=avr-objcopy
OBJSIZE=avr-size
CC=avr-gcc
CFLAGS=-mmcu=$(MMCU) -I. -gdwarf-2 -DF_CPU=$(F_CPU)UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wundef -std=gnu99 -Wundef -lm

all: $(TARGET)

clean:
	rm -f $(TARGET)

flash: $(TARGET)
	sudo avrdude -U flash:w:$(TARGET):i -C /etc/avrdude.conf -v -p $(MMCU) -b 57600 -c $(PROG) -P $(PROG_PORT)

%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.elf: %.o $(OBJ)
	$(CC) $(CFLAGS) $^ --output $@
	$(OBJSIZE) --format=avr --mcu=$(MMCU) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@