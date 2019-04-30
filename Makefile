#
# MAKEFILE FOR the zeusd server
#
prefix = /usr/local
exec_prefix = ${prefix}
sbindir = ${exec_prefix}/sbin
bindir = ${exec_prefix}/bin
mandir = ${prefix}/man
sysconfdir = /etc/mail/rbl/
localstatedir = ${prefix}/var

srcdir = .

#
CC 	= gcc
OBJS	=zeusd.o 
SRCS	=zeusd.c 
PROG	=zeusd

WARNS		=-ansi -W -Wall -Wno-unused -Wformat -Wshadow
CFLAGS 		= $(WARNS) -DCONFDIR=\"${sysconfdir}\" -D_GNU_SOURCE -DHAVE_CONFIG_H  -D__FREEBSD__ -D__BSD__ -g -O2
LDFLAGS		= -ldevstat 
RANLIB		= @RANLIB@
INSTALL		= /usr/bin/install -c
RM		= rm -f
LN		= ln -s


#--------------------------------------------------------------------------
# Dependencies
#--------------------------------------------------------------------------
all: $(PROG) 
#
clean:
	$(RM) *core *.o *~ $(PROG) 
#
distclean: clean
	$(RM) config.h config.log config.cache config.status Makefile Makefile.bak *~ .*~ 
	$(RM) -r ./autom4te.cache/
#
$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS);

install:
	mkdir -p ${sbindir}
	cp $(DAPD) ${sbindir}
	mkdir -p ${bindir}
	cp $(DAP) ${bindir}
	cp $(DAP) ${bindir}/rwho
	cp $(DAP) ${bindir}/ruptime
	cp $(DAP) ${bindir}/rusers
	mkdir -p ${localstatedir}
	mkdir -p ${sysconfdir}
#
$(OBJS) : Makefile config.h 
# DO NOT DELETE
