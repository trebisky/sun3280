/* Bare metal startup for sun3/280
 *
 * Tom Trebisky  5-19-2025
 *
 */
	.text

# Some random notes about the 3/280 rom
#
# >b?
# Boot syntax: b [!][dev(ctlr,unit,part)] name [options]
# Possible boot devices:
#   id: PANTHER (ipi) controller
#   xd: Xylogics 7053 disk
#   xy: Xylogics 440/450 disk
#   sd: SCSI disk
#   ie: Sun/Intel Ethernet
#   gn: Sun Generic Network
#   xt: Xylogics 472 tape
#   st: SCSI tape

start_here:
	movew #0x2700,%sr
	bsr start

.globl wr_diag

wr_diag:
	movec %dfc,%a0
	moveq #3,%d1
	movec %d1,%dfc

	moveb %sp@(7),%d0
	movesb %d0,0x70000000
	movec %a0,%dfc
	rts

# loop reading context register
# value will be in d0
# I see:
#  D0: ABABABF0?
#  D1: 00000003?
#  D2: DEADBEEF?
# (but when I preload d0 with 0, I see all zeros !?)

.globl con
con:
		moveq #3,%d1
		movec %d1,%dfc
		lea 0x30000000,%a0
		| moveq #0,%d0
		movel #0xabababab, %d0
		movesb %a0@,%d0
		movel #0xdeadbeef, %d2
		bras	con

.globl putch

# Send a character to serial A, spin until Tx is ready
# 0xfffe000         ; SCC-A control

# We have a timeout on the poll for status
# Then a short timeout to let the SCC recover
putch:
		moveb %sp@(7), %d0		| get the character
		moveb #'Q', %d0

poll_1:
		moveq #-1,%d2
		moveb 0xfffe000,%d1
		btst #2,%d1
		dbne %d2,poll_1

recover:
		moveq #15,%d2
		dbf %d2,recover		| recovery delay

		moveb %d0,0xfffe002
		/* bogus write to other channel */
		moveb %d0,0xfffe006
		rts

/*
loop:	bra	loop
*/

# Idiotic junk to avoid complaints from linker
.section        .note.GNU-stack,"",%progbits

/* THE END */
