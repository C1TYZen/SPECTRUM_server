GCCDEVICE=	atmega168p
ADDEVICE = m168
F_CPU=16000000 # in Hze

SOURCES = main.c
OBJECTS = main.o

CFLAGS = -DDEBUG_LEVEL=0 -ggdb
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(GCCDEVICE)

AVRDUDE = avrdude -c usbasp -P COM3 -p $(ADDEVICE) -F

help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "    make obj ........to build main.o"
	@echo "    make hex ....... to build main.hex"
	@echo "    make flash ..... to flash the firmware"
	@echo "    make clean ..... to delete objects and hex file"

obj: main.o

hex: main.hex

flash: main.hex
	$(AVRDUDE) -U flash:w:main.hex:i

clean:
	rm -v main.hex main.elf main.o

#-----

main.o: main.c
	$(COMPILE) -c $(SOURCES)

main.elf: main.o
	$(COMPILE) -o main.elf main.o

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size main.hex
