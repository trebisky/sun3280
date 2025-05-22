/* blink.c
 *
 * This is exciting.  Writing some code to run on my
 * Sun 3/280 without any operating system.
 * This gets started straight from the boot rom.
 *  5-19-2025 -- Tom Trebisky
 */

typedef volatile unsigned short vu_short;
typedef volatile unsigned char vu_char;

// #define DIAG 0x70000000

void delay_x ( void );
void wr_diag ( int );

/* This ends up a mess because the ROM keeps writing to
 * the DIAG register to blink an LED, even with SR = 0x27000
 * to disable all interrupts.
 * We need to do something more drastic, like set up our
 * own vector table and VBR perhaps.
 */
void test1 ( void )
{
	for ( ;; ) {
		// wr_diag ( 0xff );	// all off
		wr_diag ( 0xaa );
		delay_x ();
		wr_diag ( 0x55 );
		delay_x ();
	}
}

/* I do this and it turns all the LED off, BUT!
 * The second LED from the right blinks!
 * The ROM must be using some interrupt with
 * a timer to do this.  Maybe.
 */
void test2 ( void )
{
		wr_diag ( 0xff );
		for ( ;; )
			;
}

/* The Rom is still getting interrupts from the TOD clock,
 * and uses them to blink an LED, which interferes with
 * what we want to do.
 * We can shut this off via the interrupt register,
 * or we can do this and disable interrupts in the 7170
 * chip itself.
 */

#define	TOD_BASE	0xfe06000

struct timer {
		char _regs[16];
		vu_char	isr;		/* 0x10 */
		vu_char control;	/* 0x11 */
};

#define CTL_RUN		0x08
#define CTL_IE		0x10

void
kill_timer ( void )
{
		struct timer *tp;

		tp = (struct timer *) TOD_BASE;

		tp->control = 0;
}

void
start ( void )
{
		// nuke_ints ();
		kill_timer ();
		test1 ();
		// test2 ();
}

void
delay_x ( void )
{
	volatile int count;
	
	count = 50000;
	count = 100000;
	while ( count-- )
	    ;
}

/* THE END */
