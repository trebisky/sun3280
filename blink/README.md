Sun 3/280 projects - blink the diagnostic LEDs

May 19, 2025

This is always the starting place for bare metal coding.

The bootrom will use tftp and look for the file C0A8001E.
Whatever this file is, it gets loaded to 0x4000 and then the
rom branches to it.  It also sets SP to 0x4000 so the stack
will grow down from the code start location.

I copied the Makefile and general setup for this from my
Callan project.  The Callan is/was a 68000 machine, but it
made a good starting point nonetheless.
