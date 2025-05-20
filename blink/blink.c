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
		wr_diag ( 0xff );	// all off
		delay_x ();
		wr_diag ( 0x3a );	// top 2 on
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

/* Without any assembly language startup file, this
 *  MUST be first.
 */
void
start ( void )
{
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
