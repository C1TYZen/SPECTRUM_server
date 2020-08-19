#ATmega168p makefile

GCCDEVICE=	atmega168p
ADDEVICE = m168
F_CPU = 16000000 # in Hz

OBJECTS = main.o USART.o DRIVER.o ADC.o

DEBUGCOMPILE = avr-gcc -Wall -Os -g -DF_CPU=$(F_CPU) -mmcu=$(GCCDEVICE)
COMPILE = avr-gcc -Os -DF_CPU=$(F_CPU) -mmcu=$(GCCDEVICE)

AVRDUDE = avrdude -c usbasp -P COM3 -p $(ADDEVICE) -F

help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "    make obj ....... build .o files"
	@echo "    make bld ....... build firmware"
	@echo "    make upl ....... flash firmware"
	@echo "    make sze ....... print size"
	@echo "    make dmp ....... dump files"
	@echo "    ......."
	@echo "    make cln ....... delete object and hex files"
	@echo "    make clnhex .... delete hex files"
	@echo "    make clnobj .... delete object files"
	@echo "    make clndmp .... delete dump files"
	
#.o

main.o: main.c
	@echo "main.c--------------------------"
	$(COMPILE) -c main.c

USART.o: USART.c
	@echo "USART.c-------------------------"
	$(COMPILE) -c USART.c

DRIVER.o: DRIVER.c
	@echo "DRIVER.c------------------------"
	$(COMPILE) -c DRIVER.c

ADC.o: ADC.c
	@echo "ADC.c---------------------------"
	$(COMPILE) -c ADC.c

#.dmp

main.dmp: main.o
	@echo "main dump-----------------------"
	avr-objdump -d main.o > dump/main.dmp

USART.dmp: USART.o
	@echo "USART dump----------------------"
	avr-objdump -d USART.o > dump/USART.dmp

DRIVER.dmp: DRIVER.o
	@echo "DRIVER dump---------------------"
	avr-objdump -d DRIVER.o > dump/DRIVER.dmp

ADC.dmp: ADC.o
	@echo "ADC dump------------------------"
	avr-objdump -d ADC.o > dump/ADC.dmp

#.hex .elf

main.elf: $(OBJECTS)
	@echo ".elf files----------------------"
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	@echo ".hex files----------------------"
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	@echo "--------------------------------"
	avr-size -C --mcu=$(GCCDEVICE) main.elf
	@echo "--------------------------------"

#-------------
#commands-----
#-------------

obj: $(OBJECTS)

bld: main.hex

upl: main.hex
	$(AVRDUDE) -U flash:w:main.hex:i

sze: main.elf
	avr-size -C --mcu=$(GCCDEVICE) main.elf

dmp: main.dmp USART.dmp DRIVER.dmp ADC.dmp

#-------------
#clean--------
#-------------

cln:
	rm -v *.hex *.elf *.o dump/*.dmp

clnhex:
	rm -v *.hex *.elf

clnobj:
	rm -v *.o

clndmp:
	rm -v dump/*.dmp