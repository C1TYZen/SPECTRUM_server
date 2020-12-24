#ATmega168p makefile

LIBPATH = src/lib/
LIBOBJ =  ADC.o DRIVER.o PORTB.o PORTD.o USART.o
MAINOBJ = ADC.o DRIVER.o PORTB.o USART.o
FILTEROBJ = DRIVER.o PORTB.o PORTD.o USART.o

GCCDEVICE=	atmega168p
ADDEVICE = m168
F_CPU = 16000000 # in Hz

COMPILE = avr-gcc -Os -DF_CPU=$(F_CPU) -mmcu=$(GCCDEVICE)
DEBUGCOMPILE = avr-gcc -Wall -Os -g -DF_CPU=$(F_CPU) -mmcu=$(GCCDEVICE)
AVRDUDE = avrdude -c usbasp -P COM3 -p $(ADDEVICE) -F

help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "    make obj .......... build .o files"
	@echo "    make objmain .......build main.o etc. files"
	@echo "    make objfilter .....build filter.o etc. files"
	@echo "    make main ......... build main"
	@echo "    make filter ....... build filter"
	@echo "    make uplmain ...... upload main"
	@echo "    make uplfilter .... upload filter"
	@echo " "
	@echo "    make cln .......... delete object and hex files"
	@echo "    make clnhex ....... delete hex files"
	@echo "    make clnobj ....... delete object files"
	
#.o
#programs
main.o: src/main.c
	@echo "---main.c--------------------------"
	$(COMPILE) -c src/main.c

filter.o: src/filter.c
	@echo "---filter.c------------------------"
	$(COMPILE) -c src/filter.c

#lib
ADC.o: $(LIBPATH)ADC.c
	@echo "---ADC.c---------------------------"
	$(COMPILE) -c $(LIBPATH)ADC.c

DRIVER.o: $(LIBPATH)DRIVER.c
	@echo "---DRIVER.c------------------------"
	$(COMPILE) -c $(LIBPATH)DRIVER.c

PORTB.o: $(LIBPATH)PORTB.c
	@echo "---PORTB.c-------------------------"
	$(COMPILE) -c $(LIBPATH)PORTB.c

PORTD.o: $(LIBPATH)PORTD.c
	@echo "---PORTD.c-------------------------"
	$(COMPILE) -c $(LIBPATH)PORTD.c

USART.o: $(LIBPATH)USART.c
	@echo "---USART.c-------------------------"
	$(COMPILE) -c $(LIBPATH)USART.c

#.dmp
main.dmp: main.o
	@echo "---main dump-----------------------"
	avr-objdump -d main.o > dmp/main.dmp

ADC.dmp: ADC.o
	@echo "---ADC dump------------------------"
	avr-objdump -d ADC.o > dmp/ADC.dmp

DRIVER.dmp: DRIVER.o
	@echo "---DRIVER dump---------------------"
	avr-objdump -d DRIVER.o > dmp/DRIVER.dmp

PORTB.dmp: PORTB.o
	@echo "---PORTB dump----------------------"
	avr-objdump -d PORTB.o > dmp/ADC.dmp

USART.dmp: USART.o
	@echo "---USART dump----------------------"
	avr-objdump -d USART.o > dmp/USART.dmp

#.hex .elf
main.elf: main.o $(MAINOBJ)
	@echo "---.elf files----------------------"
	$(COMPILE) -o main.elf main.o $(MAINOBJ)

main.hex: main.elf
	@echo "---.hex files----------------------"
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size -C --mcu=$(GCCDEVICE) main.elf

filter.elf: filter.o $(FILTEROBJ)
	@echo "---.elf files----------------------"
	$(COMPILE) -o filter.elf filter.o $(FILTEROBJ)

filter.hex: filter.elf
	@echo "---.hex files----------------------"
	avr-objcopy -j .text -j .data -O ihex filter.elf filter.hex
	avr-size -C --mcu=$(GCCDEVICE) filter.elf

#-------------
#commands-----
#-------------
obj: main.o filter.o $(LIBOBJ)

objmain: main.o $(MAINOBJ)

objfilter: filter.o $(FILTEROBJ)

main: main.hex main.elf
	avr-size -C --mcu=$(GCCDEVICE) main.elf

filter: filter.hex filter.elf
	avr-size -C --mcu=$(GCCDEVICE) filter.elf

uplmain: main.hex
	$(AVRDUDE) -U flash:w:main.hex:i

uplfilter: filter.hex
	$(AVRDUDE) -U flash:w:filter.hex:i

dmp: main.dmp USART.dmp DRIVER.dmp ADC.dmp

#-------------
#clean--------
#-------------
cln:
	rm -v *.hex *.elf *.o dmp/*.dmp

clnhex:
	rm -v *.hex *.elf

clnobj:
	rm -v *.o

clndmp:
	rm -v dmp/*.dmp