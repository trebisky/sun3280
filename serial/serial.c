/* serial.c
 *
 * Try to send characters to Serial on the
 * Sun 3/280 without any operating system.
 * This gets started straight from the boot rom.
 *  5-20-2025 -- Tom Trebisky
 */

typedef volatile unsigned short vu_short;
typedef volatile unsigned char vu_char;

// #define DIAG 0x70000000

void delay_x ( void );
void wr_diag ( int );
void putch ( int );

/* Without any assembly language startup file, this
 *  MUST be first.
 */
void
start ( void )
{
		// All on
		wr_diag ( 0 );

		for ( ;; ) {
			putch ( 'X' );
		}
}

void
delay_x ( void )
{
	volatile int count;
	
	// count = 50000;
	count = 100000;
	while ( count-- )
	    ;
}

/* THE END */
