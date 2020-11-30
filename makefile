#ATmega168p makefile

GCCDEVICE=	atmega168p
ADDEVICE = m168
F_CPU = 16000000 # in Hz

OBJECTS = main.o USART.o DRIVER.o ADC.o PORTB.o

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

main.o: src/main.c
	@echo "---main.c--------------------------"
	$(COMPILE) -c src/main.c

USART.o: src/USART.c
	@echo "---USART.c-------------------------"
	$(COMPILE) -c src/USART.c

DRIVER.o: src/DRIVER.c
	@echo "---DRIVER.c------------------------"
	$(COMPILE) -c src/DRIVER.c

ADC.o: src/ADC.c
	@echo "---ADC.c---------------------------"
	$(COMPILE) -c src/ADC.c

PORTB.o: src/PORTB.c
	@echo "---PORTB.c---------------------------"
	$(COMPILE) -c src/PORTB.c

#.dmp

main.dmp: main.o
	@echo "---main dump-----------------------"
	avr-objdump -d main.o > dump/main.dmp

USART.dmp: USART.o
	@echo "---USART dump----------------------"
	avr-objdump -d USART.o > dump/USART.dmp

DRIVER.dmp: DRIVER.o
	@echo "---DRIVER dump---------------------"
	avr-objdump -d DRIVER.o > dump/DRIVER.dmp

ADC.dmp: ADC.o
	@echo "---ADC dump------------------------"
	avr-objdump -d ADC.o > dump/ADC.dmp

PORTB.dmp: PORTB.o
	@echo "---PORTB dump----------------------"
	avr-objdump -d PORTB.o > dump/ADC.dmp

#.hex .elf

main.elf: $(OBJECTS)
	@echo "---.elf files----------------------"
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	@echo "---.hex files----------------------"
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size -C --mcu=$(GCCDEVICE) main.elf

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