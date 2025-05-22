/* Bare metal startup for sun3/280
 *
 * Tom Trebisky  5-19-2025
 *
 * I thought that we could do without an assembly language
 * file.  We could just to start code running, but this
 * is better, as we know this is first and then we
 * call "start()" in C code -- which can be in any order.
 *
 * What forced my hand on this is that the DIAG
 * register is in fc=3 and the ROM gets us running
 * with fc=5.  And there is not way to switch fc
 * values from C code.  Trying to write to the DIAG
 * register without setting the right fc just gets
 * us a Bus error.
 */
	.text

start_here:
	movew #0x2700,%sr
	bsr start

.globl nuke_ints

# Nicer to turn off just level 5 and 7 than
#  to turn off all interrupts.
nuke_ints:
#	moveq #0, %d0
	moveq #0x1f, %d0
	moveb %d0, 0x0fe0a000
	rts

.globl wr_diag

# How do we do online comments in this flavor of the gnu assembler?
# ; acts to separate statements
# This pound character only works at the start of the line
# using // does not cut it.
# neither @ nor ! does it
#  nor : or "
#
# ************************
# I finally give up and look at NetBSD sources.
# *** the pipe character "|" is the thing to use.
# Who would have guessed.
# Apparently little (i.e. none) concern is given to making the
# gnu assembler consistent or useful to humans.

wr_diag:
	movec %dfc,%a0
	moveq #3,%d1
	movec %d1,%dfc
	moveb %sp@(7),%d0
	movesb %d0,0x70000000
	movec %a0,%dfc
	rts

.globl spin

spin:	bra	spin

# Idiotic junk to avoid complaints from linker
.section        .note.GNU-stack,"",%progbits

/* THE END */
