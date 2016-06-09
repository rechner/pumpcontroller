# apt-get install avr-libc avr-util gcc-avr
CC=/usr/bin/avr-gcc
LD=/usr/bin/avr-objcopy

all: compile upload

compile:
	$(CC) -mmcu=atmega48 -Wall -I. -Os -DF_CPU=8000000UL -o main.elf main.c lcd_lib.c
	$(LD) -j .text -j .data -O ihex main.elf main.hex

upload:
	sudo /usr/bin/avrdude -F -p m48 -c usbtiny -e -U flash:w:main.hex

