IDENT='-DID="Rev 2.1"' -DSUN3 -DM25
#
# Makefile for Sun ROM Monitor
#
# @(#)Makefile 1.1 86/09/27 SMI
#
# This Makefile is constructed from a configuration description.
# Most changes should be made there:
#	/usr/src/sun/mon/conf/<PromName>
# after which you should do
#	sunconfig <config>
# Generic Makefile changes should be made in
#	/usr/src/sun/mon/conf/Makefile.<cputype>
# after which sunconfig should be rerun for all config files.
#
# -N on loader gives old-style load module (not pageable or pagealigned)
LD=/bin/ld -N
# -j on assembler gives short external PC-relative jumps, JSR's.
AS=/bin/as -20 -j
ASNOJ=/bin/as -20
C2=/lib/c2 -20

COPTS=
CFLAGS=-I../h -I. ${COPTS} ${IDENT} -DMC68000
AFLAGS=-c
LDFLAGS=-lpixrect -lc

RELOC=0FEF0000

OBJS=romvec.o trap.o mapmem.o reset.o cpu.map.o diag.o banner.o commands.o \
	idprom.o usecmd.o getline.o printf.o busyio.o keypress.o\
	getkey.o ktab.s2.o boot.o monalloc.o common.o blts.o \
	expand.o finit.o fwritestr.o gallmash.o mem_grab.o if_le.o \
	sc.o sd.o xxboot.o xxprobe.o scutils.o space.o st.o \
	inet.o tftp.o diagmenus.o machdep.o cmp.o patt.o db.o si.o

CFILES=../sun3/romvec.s ../sun3/trap.s ../sys/mapmem.c \
	../sys/reset.c ../sun3/cpu.map.s ../diag/diag.s \
	../sys/banner.c ../sys/commands.c ../sys/idprom.c \
	../sys/usecmd.c ../sys/getline.c ../sys/printf.c \
	../sys/busyio.c ../sys/keypress.c \
	../sys/getkey.c ../sys/ktab.s2.c \
	../sys/boot.c ../sys/monalloc.c ../sys/common.c \
	../sun3/blts.s ../sys/expand.c ../sys/finit.c \
	../sys/fwritestr.c ../sys/gallmash.c ../sys/mem_grab.c \
	../dev/if_le.c ../dev/sc.c \
	../dev/sd.c ../sys/xxboot.c ../sys/xxprobe.c \
	../dev/scutils.c ../sun3/space.s ../dev/st.c \
	../sys/inet.c ../sys/tftp.c \
	../diag/diagmenus.c ../sun3/machdep.c ../sun3/cmp.s \
	../sun3/patt.s ../sun3/db.s ../dev/si.c

all: sunmon
# what ../[a-z]*/* >WhatModules
# The above remembers what the version # of each module was at the time
# this sunmon was built.  This is needed since the SCCS id's don't compile
# into the PROM, they are just comments in the source.

sunmon: Makefile ${OBJS} 
	@echo loading sunmon
	@rm -f sunmon
	$(LD) -o sunmon -T $(RELOC) ${OBJS} ${LDFLAGS}
	size sunmon

# assym.h contains assembler defns of structure members.  It's made by
# genassym.c, which just printf's their values.  make depend keeps it all OK.
# structconst.h contains hex values of constant structs like page map entries.
# This hack is used to get around C's refusal to initialize local structs
# and inability to define structure constants.
genassym.o: ../sun3/genassym.c Makefile
	${CC} -c ${CFLAGS} ../sun3/genassym.c
assym.h: genassym.o
	${CC} ${CFLAGS} genassym.o; ./a.out > ../sun3/assym.h; rm -f a.out
structconst.h: ../sun3/assym.h
	grep PME_ ../sun3/assym.h >../sun3/structconst.h

install: sunmon
	;

clean:
	rm -f *.o *.s *.h *.lst errs linterrs lint.foo foo1 foo2 grindlist
	rm -f sunmon WhatModules

lint:
	ls ${CFILES} >lint.foo
	lint -hbxnz -I. ${COPTS} ${IDENT} `fgrep -v .s <lint.foo`
	rm lint.foo

depend:
	@echo 'making dependencies ...'
	@egrep '^#include' ../sun3/genassym.c ${CFILES} | \
	      egrep -v '<' | \
	      sed 's/:[^"]*"\([^"]*\)".*/: \1/' | \
	      sed 's/\.[cs]/.o/' | \
	      sed 's,^\([.0-9a-z/]*/\)\([^:]*\): *,\2: \1,' | \
	      sed 's,: [.0-9a-z/]*Makefile,: Makefile,' | \
	      sed 's,: [.0-9a-z/]*assym\.h,: assym.h,' | \
	      sed 's,: [.0-9a-z/]*structconst\.h,: structconst.h,' | \
	      sed 's,/[^ /.]*/\.\./,/,' >depend.tmp
# The above is:
#	 egrep:	get all #include lines from all files.
#	 egrep: exclude system includes, just use "....." includes
#	 sed:   : #include "foobar.h"  ==>  : foobar.h
#		get rid of extraneous gunk
#	 sed:   file.c: foobar.h       ==>  file.o: foobar.h
#		the dependency is on the .o, not the .c or .s
#	 sed:   ../dir/name.o: ../h/bar.h   ==>  name.c: ../dir/../h/bar.h
#		the include file is relative to the source file's dir
#	 sed:   name.o: ../dir/Makefile	    ==>  name.o: Makefile
#		We know that 'Makefile' is in current dir, not source dir.
#	 sed:   name.o: ../dir/assym.h	    ==>  name.o: assym.h
#		We know that 'assym.h' is in current dir, not source dir.
#	 sed:   name.o: ../dir/structconst.h	    ==>  name.o: structconst.h
#		We know that 'structconst.h' is in current dir, not source dir.
#	 sed:   /name/../     ==>   /
#		clean up the cases like ../kernel/../h  to ../h
#        See, that wasn't too bad, was it?
	@awk ' { if ($$1 != prev) { print rec; rec = $$0; prev = $$1; } \
		else { if (length(rec $$2) > 78) { print rec; rec = $$0; } \
		       else rec = rec " " $$2 } } \
	      END { print rec } ' <depend.tmp  > makedep
# The above just merges lines of dependencies for the same module until
# they would exceed 78 characters.  Just for prettiness.
	@echo '/^# DO NOT DELETE THIS LINE/+1,$$d' >eddep
	@echo '$$r makedep' >>eddep
	@echo 'w' >>eddep
	@ed - Makefile < eddep
	@rm eddep makedep depend.tmp
	@echo '... done'

tags:
	/usr/ucb/ctags ${CFILES}

print:
	../sun3/pr.sh `basename \`pwd\`` Makefile $(CFILES) ../h/*.h | lpr

grindlist: ../prom_etc/files
	ls `grep -v '^#' ../sun3/files | colrm 30 | sed 's,^,../,' ` | sort -t/ +2 >grindlist

grindc: grindlist
	igrind ${GRINDOPTS} `grep '\.c$$' grindlist`

grinds: grindlist
	igrind ${GRINDOPTS} -n `pwd`/Makefile `grep -v '\.c$$' grindlist`

grindh: assym.h
	igrind ${GRINDOPTS} `pwd`/assym.h `pwd`/structconst.h ../h/*.h \
		../sun*/*.h

grind: grindh grindc grinds
	@echo

romvec.o: ../sun3/romvec.s
	cp ../sun3/romvec.s romvec.c
	${CC} -E -DASM ${CFLAGS} romvec.c >romvec.i
	${AS} ${LFLAG} romvec.i -o romvec.o
	rm -f romvec.c romvec.i
trap.o: ../sun3/trap.s
	cp ../sun3/trap.s trap.c
	${CC} -E -DASM ${CFLAGS} trap.c >trap.i
	${AS} ${LFLAG} trap.i -o trap.o
	rm -f trap.c trap.i
mapmem.o: ../sys/mapmem.c
	${CC} -S ${CFLAGS} ../sys/mapmem.c
	mv mapmem.s mapmem.p
	${C2} mapmem.p mapmem.s
	${AS} ${LFLAG} mapmem.s -o mapmem.o
	rm -f mapmem.p
reset.o: ../sys/reset.c
	${CC} -S ${CFLAGS} ../sys/reset.c
	mv reset.s reset.p
	${C2} reset.p reset.s
	${AS} ${LFLAG} reset.s -o reset.o
	rm -f reset.p
cpu.map.o: ../sun3/cpu.map.s
	cp ../sun3/cpu.map.s cpu.map.c
	${CC} -E -DASM ${CFLAGS} cpu.map.c >cpu.map.i
	${AS} ${LFLAG} cpu.map.i -o cpu.map.o
	rm -f cpu.map.c cpu.map.i
diag.o: ../diag/diag.s
	cp ../diag/diag.s diag.c
	${CC} -E -DASM ${CFLAGS} diag.c >diag.i
	${AS} ${LFLAG} diag.i -o diag.o
	rm -f diag.c diag.i
banner.o: ../sys/banner.c
	${CC} -S ${CFLAGS} ../sys/banner.c
	mv banner.s banner.p
	${C2} banner.p banner.s
	${AS} ${LFLAG} banner.s -o banner.o
	rm -f banner.p
commands.o: ../sys/commands.c
	${CC} -S ${CFLAGS} ../sys/commands.c
	mv commands.s commands.p
	${C2} commands.p commands.s
	${ASNOJ} ${LFLAG} commands.s -o commands.o
	rm -f commands.p
idprom.o: ../sys/idprom.c
	${CC} -S ${CFLAGS} ../sys/idprom.c
	mv idprom.s idprom.p
	${C2} idprom.p idprom.s
	${AS} ${LFLAG} idprom.s -o idprom.o
	rm -f idprom.p
usecmd.o: ../sys/usecmd.c
	${CC} -S ${CFLAGS} ../sys/usecmd.c
	mv usecmd.s usecmd.p
	${C2} usecmd.p usecmd.s
	${AS} ${LFLAG} usecmd.s -o usecmd.o
	rm -f usecmd.p
getline.o: ../sys/getline.c
	${CC} -S ${CFLAGS} ../sys/getline.c
	mv getline.s getline.p
	${C2} getline.p getline.s
	${AS} ${LFLAG} getline.s -o getline.o
	rm -f getline.p
printf.o: ../sys/printf.c
	${CC} -S ${CFLAGS} ../sys/printf.c
	mv printf.s printf.p
	${C2} printf.p printf.s
	${AS} ${LFLAG} printf.s -o printf.o
	rm -f printf.p
busyio.o: ../sys/busyio.c
	${CC} -S ${CFLAGS} ../sys/busyio.c
	mv busyio.s busyio.p
	${C2} busyio.p busyio.s
	${AS} ${LFLAG} busyio.s -o busyio.o
	rm -f busyio.p
keypress.o: ../sys/keypress.c
	${CC} -S ${CFLAGS} ../sys/keypress.c
	mv keypress.s keypress.p
	${C2} keypress.p keypress.s
	${AS} ${LFLAG} keypress.s -o keypress.o
	rm -f keypress.p
getkey.o: ../sys/getkey.c
	${CC} -S ${CFLAGS} ../sys/getkey.c
	mv getkey.s getkey.p
	${C2} getkey.p getkey.s
	${AS} ${LFLAG} getkey.s -o getkey.o
	rm -f getkey.p
ktab.s2.o: ../sys/ktab.s2.c
	${CC} -S ${CFLAGS} ../sys/ktab.s2.c
	mv ktab.s2.s ktab.s2.p
	${C2} ktab.s2.p ktab.s2.s
	${AS} ${LFLAG} ktab.s2.s -o ktab.s2.o
	rm -f ktab.s2.p
boot.o: ../sys/boot.c
	${CC} -S ${CFLAGS} ../sys/boot.c
	mv boot.s boot.p
	${C2} boot.p boot.s
	${AS} ${LFLAG} boot.s -o boot.o
	rm -f boot.p
monalloc.o: ../sys/monalloc.c
	${CC} -S ${CFLAGS} ../sys/monalloc.c
	mv monalloc.s monalloc.p
	${C2} monalloc.p monalloc.s
	${AS} ${LFLAG} monalloc.s -o monalloc.o
	rm -f monalloc.p
common.o: ../sys/common.c
	${CC} -S ${CFLAGS} ../sys/common.c
	mv common.s common.p
	${C2} common.p common.s
	${AS} ${LFLAG} common.s -o common.o
	rm -f common.p
blts.o: ../sun3/blts.s
	cp ../sun3/blts.s blts.c
	${CC} -E -DASM ${CFLAGS} blts.c >blts.i
	${AS} ${LFLAG} blts.i -o blts.o
	rm -f blts.c blts.i
expand.o: ../sys/expand.c
	${CC} -S ${CFLAGS} ../sys/expand.c
	mv expand.s expand.p
	${C2} expand.p expand.s
	${AS} ${LFLAG} expand.s -o expand.o
	rm -f expand.p
finit.o: ../sys/finit.c
	${CC} -S ${CFLAGS} ../sys/finit.c
	mv finit.s finit.p
	${C2} finit.p finit.s
	${AS} ${LFLAG} finit.s -o finit.o
	rm -f finit.p
fwritestr.o: ../sys/fwritestr.c
	${CC} -S ${CFLAGS} ../sys/fwritestr.c
	mv fwritestr.s fwritestr.p
	${C2} fwritestr.p fwritestr.s
	${AS} ${LFLAG} fwritestr.s -o fwritestr.o
	rm -f fwritestr.p
gallmash.o: ../sys/gallmash.c
	${CC} -S ${CFLAGS} ../sys/gallmash.c
	mv gallmash.s gallmash.p
	${C2} gallmash.p gallmash.s
	${AS} ${LFLAG} gallmash.s -o gallmash.o
	rm -f gallmash.p
mem_grab.o: ../sys/mem_grab.c
	${CC} -S ${CFLAGS} ../sys/mem_grab.c
	mv mem_grab.s mem_grab.p
	${C2} mem_grab.p mem_grab.s
	${AS} ${LFLAG} mem_grab.s -o mem_grab.o
	rm -f mem_grab.p
if_le.o: ../dev/if_le.c
	${CC} -S ${CFLAGS} ../dev/if_le.c
	mv if_le.s if_le.p
	${C2} if_le.p if_le.s
	${AS} ${LFLAG} if_le.s -o if_le.o
	rm -f if_le.p
sc.o: ../dev/sc.c
	${CC} -S ${CFLAGS} ../dev/sc.c
	mv sc.s sc.p
	${C2} sc.p sc.s
	${AS} ${LFLAG} sc.s -o sc.o
	rm -f sc.p
sd.o: ../dev/sd.c
	${CC} -S ${CFLAGS} ../dev/sd.c
	mv sd.s sd.p
	${C2} sd.p sd.s
	${AS} ${LFLAG} sd.s -o sd.o
	rm -f sd.p
xxboot.o: ../sys/xxboot.c
	${CC} -S ${CFLAGS} ../sys/xxboot.c
	mv xxboot.s xxboot.p
	${C2} xxboot.p xxboot.s
	${AS} ${LFLAG} xxboot.s -o xxboot.o
	rm -f xxboot.p
xxprobe.o: ../sys/xxprobe.c
	${CC} -S ${CFLAGS} ../sys/xxprobe.c
	mv xxprobe.s xxprobe.p
	${C2} xxprobe.p xxprobe.s
	${AS} ${LFLAG} xxprobe.s -o xxprobe.o
	rm -f xxprobe.p
scutils.o: ../dev/scutils.c
	${CC} -S ${CFLAGS} ../dev/scutils.c
	mv scutils.s scutils.p
	${C2} scutils.p scutils.s
	${AS} ${LFLAG} scutils.s -o scutils.o
	rm -f scutils.p
space.o: ../sun3/space.s
	cp ../sun3/space.s space.c
	${CC} -E -DASM ${CFLAGS} space.c >space.i
	${AS} ${LFLAG} space.i -o space.o
	rm -f space.c space.i
st.o: ../dev/st.c
	${CC} -S ${CFLAGS} ../dev/st.c
	mv st.s st.p
	${C2} st.p st.s
	${AS} ${LFLAG} st.s -o st.o
	rm -f st.p
inet.o: ../sys/inet.c
	${CC} -S ${CFLAGS} ../sys/inet.c
	mv inet.s inet.p
	${C2} inet.p inet.s
	${AS} ${LFLAG} inet.s -o inet.o
	rm -f inet.p
tftp.o: ../sys/tftp.c
	${CC} -S ${CFLAGS} ../sys/tftp.c
	mv tftp.s tftp.p
	${C2} tftp.p tftp.s
	${AS} ${LFLAG} tftp.s -o tftp.o
	rm -f tftp.p

diagmenus.o: ../diag/diagmenus.c
	${CC} -S ${CFLAGS} ../diag/diagmenus.c
	mv diagmenus.s diagmenus.p
	${C2} diagmenus.p diagmenus.s
	${AS} ${LFLAG} diagmenus.s -o diagmenus.o
	rm -f diagmenus.p

machdep.o: ../sun3/machdep.c
	${CC} -S ${CFLAGS} ../sun3/machdep.c
	mv machdep.s machdep.p
	${C2} machdep.p machdep.s
	${AS} ${LFLAG} machdep.s -o machdep.o
	rm -f machdep.p

cmp.o: ../sun3/cmp.s
	cp ../sun3/cmp.s cmp.c
	${CC} -E -DASM ${CFLAGS} cmp.c >cmp.i
	${AS} ${LFLAG} cmp.i -o cmp.o
	rm -f cmp.c cmp.i

patt.o: ../sun3/patt.s
	cp ../sun3/patt.s patt.c
	${CC} -E -DASM ${CFLAGS} patt.c >patt.i
	${AS} ${LFLAG} patt.i -o patt.o
	rm -f patt.c patt.i

db.o: ../sun3/db.s
	cp ../sun3/db.s db.c
	${CC} -E -DASM ${CFLAGS} db.c >db.i
	${AS} ${LFLAG} db.i -o db.o
	rm -f db.c db.i
si.o: ../dev/si.c
	${CC} -S ${CFLAGS} ../dev/si.c
	mv si.s si.p
	${C2} si.p si.s
	${AS} ${LFLAG} si.s -o si.o
	rm -f si.p


# DO NOT DELETE THIS LINE -- make depend uses it

banner.o: ../sun3/sunmon.h ../h/globram.h 
banner.o: ../h/idprom.h ../sun3/cpu.addrs.h ../h/video.h
banner.o: ../h/fbio.h ../h/clock.h ../h/eeprom.h
boot.o: ../h/globram.h ../h/bootparam.h ../h/sunromvec.h
boot.o: ../dev/saio.h ../h/eeprom.h ../sun3/cpu.addrs.h
busyio.o: ../sun3/cpu.addrs.h ../dev/saio.h ../dev/zsreg.h 
busyio.o: ../sun3/sunmon.h ../h/globram.h
commands.o: ../sun3/sunmon.h ../h/globram.h ../sun3/m68vectors.h
commands.o: ../h/statreg.h ../sun3/cpu.addrs.h ../sun3/cpu.map.h
commands.o: ../h/buserr.h ../sun3/cpu.misc.h ../h/pginit.h
commands.o: ../h/montrap.h ../h/led.h ../h/keyboard.h
commands.o: ../sun3/memerr.h ../h/interreg.h ../h/clock.h
commands.o: ../h/enable.h ../h/eeprom.h
common.o: ../sun3/cpu.addrs.h ../dev/saio.h ../h/globram.h
common.o: ../h/types.h ../dev/dklabel.h
cpu.map.o: ../sun3/assym.h 
diag.o: ../sun3/assym.h ../h/led.h 
finit.o: ../sun3/cpu.addrs.h ../h/globram.h ../h/dpy.h
finit.o: ../h/pginit.h ../h/eeprom.h
fwritestr.o: ../h/globram.h ../h/dpy.h ../h/keyboard.h
fwritestr.o: ../h/video.h ../sun3/cpu.addrs.h ../h/enable.h
genassym.o: ../sun3/cpu.addrs.h ../sun3/cpu.map.h ../sun3/sunmon.h
genassym.o: ../h/globram.h ../sun3/m68vectors.h ../dev/zsreg.h
genassym.o: ../h/sunromvec.h ../h/dpy.h
genassym.o: ../dev/am8068.h ../h/montrap.h ../h/statreg.h
genassym.o: ../sun3/memreg.h ../dev/cg2reg.h ../dev/am9513.h
genassym.o: ../h/suntimer.h ../h/enable.h ../h/clock.h
genassym.o: ../sun3/memerr.h ../h/interreg.h 
getkey.o: ../h/keyboard.h ../h/asyncbuf.h ../h/globram.h ../sun3/sunmon.h
idprom.o: ../h/idprom.h
if_le.o: ../dev/if_le.c ../dev/saio.h ../h/socket.h ../dev/if.h
if_le.o: ../h/in.h ../dev/if_ether.h ../dev/if_lereg.h
if_le.o: ../h/idprom.h ../sun3/cpu.map.h
inet.o: ../dev/saio.h ../h/socket.h ../dev/if.h ../h/in.h ../dev/if_ether.h
inet.o: ../h/in_systm.h ../h/ip.h ../h/sainet.h ../h/sunromvec.h ../h/idprom.h
keypress.o: ../sun3/sunmon.h ../h/globram.h ../h/keyboard.h  ../h/asyncbuf.h
ktab.s2.o: ../h/keyboard.h
mapmem.o: ../sun3/sunmon.h ../sun3/cpu.map.h ../sun3/cpu.addrs.h
mapmem.o: ../h/sunromvec.h ../h/pginit.h ../h/globram.h
mapmem.o: ../h/enable.h ../sun3/structconst.h 
mem_grab.o: ../h/types.h ../h/pixrect.h ../sun3/memreg.h 
mem_grab.o: ../h/memvar.h ../h/pr_util.h
monalloc.o: ../sun3/cpu.map.h ../sun3/cpu.addrs.h ../dev/saio.h ../h/globram.h
reset.o: ../sun3/sunmon.h ../sun3/cpu.map.h ../sun3/cpu.addrs.h
reset.o: ../sun3/cpu.misc.h ../h/globram.h ../dev/zsreg.h ../h/keyboard.h
reset.o: ../h/sunromvec.h ../sun3/m68vectors.h ../h/fbio.h ../h/pginit.h
reset.o: ../h/montrap.h ../h/dpy.h ../h/video.h ../h/led.h ../h/setbus.h
reset.o: ../dev/saio.h ../h/clock.h ../h/interreg.h
reset.o: ../h/enable.h ../sun3/memerr.h ../sun3/structconst.h ../h/eeprom.h
romvec.o: ../sun3/assym.h 
sc.o: ../h/types.h ../h/param.h ../dev/screg.h ../dev/saio.h ../h/sunromvec.h
scutils.o: ../sun3/structconst.h ../sun3/cpu.addrs.h ../sun3/cpu.map.h
scutils.o: ../h/pginit.h ../sun3/memreg.h ../dev/cg2reg.h ../h/eeprom.h
sd.o: ../dev/saio.h ../dev/dklabel.h ../dev/dkio.h ../dev/screg.h
sd.o: ../h/sunromvec.h
space.o: ../sun3/assym.h
st.o: ../dev/saio.h ../dev/dklabel.h ../dev/dkio.h 
st.o: ../dev/screg.h ../dev/streg.h
tftp.o: ../dev/saio.h ../h/socket.h ../dev/if.h ../h/in.h ../dev/if_ether.h
tftp.o: ../h/in_systm.h ../h/ip.h ../h/udp.h ../h/sainet.h
tftp.o: ../h/sunromvec.h ../sun3/cpu.addrs.h ../dev/sasun.h ../h/tftp.h
trap.o: ../sun3/assym.h ../h/led.h
usecmd.o: ../sun3/sunmon.h ../h/globram.h ../sun3/cpu.misc.h
usecmd.o: ../dev/zsreg.h ../sun3/m68vectors.h ../dev/saio.h
xxboot.o: ../dev/saio.h ../dev/xycreg.h ../dev/xtreg.h
xxprobe.o: ../h/sunromvec.h ../h/setbus.h ../dev/saio.h
diagmenus.o: ../sun3/sunmon.h ../h/globram.h ../sun3/cpu.misc.h ../dev/zsreg.h
diagmenus.o: ../sun3/m68vectors.h ../h/sunromvec.h ../dev/saio.h
diagmenus.o: ../dev/promether.h ../sun3/machdep.h ../diag/diagmenus.h
diagmenus.o: ../h/systypes.h ../dev/amd_ether.h ../sun3/cpu.addrs.h
machdep.o: ../h/systypes.h ../sun3/machdep.h
si.o: ../h/types.h ../h/buf.h ../dev/screg.h ../dev/sireg.h ../dev/scsi.h
si.o: ../dev/saio.h ../h/sunromvec.h ../h/idprom.h

