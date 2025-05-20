# Build a bare metal blink demo for the Sun3/280
#
# Tom Trebisky 5/19/2025
#
# This is from a Fedora package for F34 as of 2-2022
#CC = m68k-linux-gnu-gcc -m68020 -O2 -fno-builtin
CC = m68k-linux-gnu-gcc -m68020 -fno-builtin

AS = m68k-linux-gnu-as -m68020
LD = m68k-linux-gnu-ld
DUMP = m68k-linux-gnu-objdump -d

OBJS = start.o blink.o

all: blink.bin blink.dump

install: blink.bin
	cp blink.bin /var/lib/tftpboot/sun3_blink.bin

# Here is the crucial part, linking.
#  the -s switch says to strip the symbol table.
#  note the handy binary image output option!
blink.bin:	$(OBJS)
	$(LD) -T sun3.lds --oformat binary -s -o blink.bin $(OBJS)

blink.elf:	$(OBJS)
	$(LD) -T sun3.lds -s -o blink.elf $(OBJS)

blink.dump:	blink.elf
	$(DUMP) blink.elf >blink.dump

start.o:      start.S
	$(AS) -o start.o start.S

blink.o:	blink.c
	$(CC) -c blink.c

blink.s:	blink.c
	$(CC) -S blink.c

clean:
	rm -f $(OBJS) blink.bin 
	rm -f *.o *.dump
	rm -f blink.elf blink.s

# THE END
