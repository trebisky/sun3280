# Build a bare metal serial demo for the Sun3/280
#
# Tom Trebisky 5/20/2025
#
# This is from a Fedora package
#CC = m68k-linux-gnu-gcc -m68020 -O2 -fno-builtin
CC = m68k-linux-gnu-gcc -m68020 -fno-builtin

AS = m68k-linux-gnu-as -m68020
LD = m68k-linux-gnu-ld --no-warn-rwx-segments
DUMP = m68k-linux-gnu-objdump -d

OBJS = start.o serial.o

all: serial.bin serial.dump

install: serial.bin
	cp serial.bin /var/lib/tftpboot/sun3_test.bin

# Here is the crucial part, linking.
#  the -s switch says to strip the symbol table.
#  note the handy binary image output option!
serial.bin:	$(OBJS)
	$(LD) -T sun3.lds --oformat binary -s -o serial.bin $(OBJS)

serial.elf:	$(OBJS)
	$(LD) -T sun3.lds -s -o serial.elf $(OBJS)

serial.dump:	serial.elf
	$(DUMP) serial.elf >serial.dump

start.o:      start.S
	$(AS) -o start.o start.S

serial.o:	serial.c
	$(CC) -c serial.c

serial.s:	serial.c
	$(CC) -S serial.c

clean:
	rm -f $(OBJS) serial.bin 
	rm -f *.o *.dump
	rm -f serial.elf serial.s

# THE END
