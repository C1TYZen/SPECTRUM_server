#ATmega168p makefile

GCCDEVICE=	atmega168p
ADDEVICE = m168
F_CPU=16000000 # in Hze

OBJECTS = main.o USART.o DRIVER.o ADC.o TIMER.o

COMPILE = avr-gcc -I. -Wall -Os -ggdb -DDEBUG_LEVEL=0 -DF_CPU=$(F_CPU) -mmcu=$(GCCDEVICE)

AVRDUDE = avrdude -c usbasp -P COM3 -p $(ADDEVICE) -F

help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "    make obj ....... to build .o files"
	@echo "    make bld ....... to build a firmware"
	@echo "    make upl ....... to flash a firmware"
	@echo "    ......."
	@echo "    make cln ....... to delete object and hex files"
	@echo "    make clnhex .... to delete hex files"
	@echo "    make clnobj .... to delete object files"

main.o: main.c
	$(COMPILE) -c main.c

USART.o: USART.c
	$(COMPILE) -c USART.c

DRIVER.o: DRIVER.c
	$(COMPILE) -c DRIVER.c

ADC.o: ADC.c
	$(COMPILE) -c ADC.c

TIMER.o: TIMER.c
	$(COMPILE) -c TIMER.c

main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size main.hex

#compile-----

obj: $(OBJECTS)

bld: main.hex

upl: main.hex
	$(AVRDUDE) -U flash:w:main.hex:i

#clean-----

cln:
	rm -v *.hex *.elf *.o

clnhex:
	rm -v *.hex *.elf

clnobj:
	rm -v *.o