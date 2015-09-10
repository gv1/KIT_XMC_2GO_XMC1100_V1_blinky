# Original files from : http://eleceng.dit.ie/frank/arm/BareMetalXMC2Go/index.html

# In order to use this makefile you must have an arm compiler on your
# system. You can get one here: https://launchpad.net/gcc-arm-embedded
# Your PATH environment variable should include the 'bin' folder in
# the arm compiler directory tree.
# One final thing.  You must edit the LIBSPEC variable below to make
# it point at your arm compiler.
# A particular version of the compiler is mentioned in the LIBSPEC 
# line below.  Find out what the equivalent is for your system and change
# it accordingly.
# Tell the linker where to find the libraries -> important: use thumb versions
# LIBSPEC=-L /usr/local/gcc-arm-none-eabi-4_8-2013q4/arm-none-eabi/lib/armv6-m
LIBSPEC=-L /eda/micro64/arm/gcc-arm-none-eabi-4_9-2015q2-20150609/arm-none-eabi/lib

# Specify the compiler to use
CC=arm-none-eabi-gcc
# Specify the assembler to use
AS=arm-none-eabi-as
# Specity the linker to use
LD=arm-none-eabi-ld

CCFLAGS=-mcpu=cortex-m0 -mthumb -g

# List the object files involved in this project
OBJS=	init.o 

# The default 'target' (output) is main.elf and it depends on the object files being there.
# These object files are linked together to create main.elf
%.elf : $(OBJS) %.o
	$(LD) $(OBJS) $(*F).o $(LIBSPEC) -T linker_script.ld -lc --cref -Map main.map -nostartfiles -o $@
	arm-none-eabi-objcopy -O binary $@ $(*F).bin
	objcopy -O ihex $@ $(*F).hex
	@echo "done"
	
# The object file main.o depends on main.c.  main.c is compiled to make main.o
# main.o: main.c
# 	$(CC) -c $(CCFLAGS) main.c -o main.o
# 

%.o: %.c
	$(CC) -c $(CCFLAGS) $< -o $@
# init.o: init.c
# 	$(CC) -c $(CCFLAGS) init.c -o init.o

all:
	@echo "	Usage: make led1, make led2, make blink, make count"

# if someone types in 'make clean' then remove all object files and executables
# associated wit this project
clean: 
	-rm -f *.o
	-rm -f *.elf
	-rm -f *.bin 
	-rm -f *.hex
	-rm -f *.map
	rm flash.gdb

led1: led1.elf 
	make f ELF=led1.elf

led2: led2.elf 
	make f ELF=led2.elf

blink: blink.elf
	make f ELF=blink.elf

count: count.elf
	make f ELF=count.elf


gs gdbserver:
	JLinkGDBServer -device XMC1100-0064 -if SWD -speed 4000	

g gdb:
	arm-none-eabi-gdb 

t1:
	make led1.elf

ELF=main.elf
f flash: $(ELF)
	JLinkGDBServer -device XMC1100-0064 -if SWD -speed 4000 &
	echo "target remote :2331" > flash.gdb
	echo "monitor reset" >> flash.gdb
	echo "monitor halt" >> flash.gdb
	echo "load $(ELF)" >> flash.gdb
	echo "monitor reset" >> flash.gdb
	echo "monitor go" >> flash.gdb
	echo "quit" >> flash.gdb
	arm-none-eabi-gdb -x flash.gdb
	kill $$(pidof JLinkGDBServer)

