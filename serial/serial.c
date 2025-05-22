/* serial.c
 *
 * Try to send characters to Serial on the
 * Sun 3/280 without any operating system.
 * This gets started straight from the boot rom.
 *  5-20-2025 -- Tom Trebisky
 */

typedef volatile unsigned short vu_short;
typedef volatile unsigned char vu_char;

#define BIT(nr)		(1<<(nr))

// #define DIAG 0x70000000

void delay_x ( void );
void wr_diag ( int );
void con ( void );
void putch ( int );
void recovery ( void );

/* Write here to send data to serial A
0x0fe02006
 * port A really comes second the way Sun wired it all up.
 * My eeprom location 0x1f is set to 0x10 for "use port A"
 * and I am getting data on port A.
*/

struct scc {
		vu_char		csr_b;		/* 00 */
		char		_pad1;
		vu_char		data_b;		/* 02 */
		char		_pad2;
		vu_char		csr_a;		/* 04 */
		char		_pad3;
		vu_char		data_a;		/* 06 */
};

#define SCC_BASE	0x0fe02000

#define ST_TXE	BIT(2)
#define ST_RXRDY	BIT(0)

void
putch ( int cc )
{
		struct scc *sp = (struct scc *) SCC_BASE;

		if ( cc == '\n' )
			putch ( '\r' );

		while ( ! (sp->csr_a & ST_TXE ) )
			// recovery ();
			;
		sp->data_a = cc;
		// recovery ();
}

void
puts ( char *msg )
{
		while ( *msg )
			putch ( *msg++ );
}

void
flood ( void )
{
		for ( ;; ) {
			putch ( 'X' );
			putch ( 'a' );
		}
}

char big_msg[100];

void
mk_test ( void )
{
		int i;
		char *p;

		p = big_msg;

		for ( i=0; i<26; i++ )
			*p++ = 'A' + i;
		for ( i=0; i<26; i++ )
			*p++ = 'a' + i;
		for ( i=0; i<10; i++ )
			*p++ = '0' + i;
		*p = '\0';
}

void
big_test ( void )
{
		int i;

		for ( i=0; i<26; i++ )
			putch ( 'A' + i );
		for ( i=0; i<26; i++ )
			putch ( 'a' + i );
		for ( i=0; i<10; i++ )
			putch ( '0' + i );
		putch ( '\n' );
}

void
greet ( void )
{
		for ( ;; ) {
			puts ( "Hello World\n" );
			// puts ( big_msg );
			big_test ();
			delay_x ();
		}
}

/* Without any assembly language startup file, this
 *  MUST be first.
 */
void
start ( void )
{
		// All on
		// wr_diag ( 0 );

		// con ();
		// flood ();
		greet ();
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
