Sun 3/280 projects - a serial demo

May 20, 2025

We can sort of blink the lights, now let's see if
 we can write to a serial port

A tip to sun3 hackers.  I have been using picocom to give
me a break to get back to the rom.  Then I was using k2
to do a full reset and test new code.  Just typing "b"
is faster and perfect in most caes.

Typing b! (then c) is super fast (it does not do a reset).

This demo has a very basic linker file.
Many normal C language features do not work properly.
A lack of zeroing of BSS variables is one example.
That will be a job for the next demo
