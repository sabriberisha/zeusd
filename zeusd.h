#ifndef __RBLCHECKD_H
#define __RBLCHECKD_H


#include "config.h"
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#endif
#include <sys/uio.h>
#include <sys/stat.h>
#include <netdb.h>
#include <arpa/nameser.h>
#include <sys/socket.h>
#include <netinet/in.h>
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif


/* Work around systems (Linux?) which don't have strlcpy */
#ifndef HAVE_STRLCPY
#warning You should upgrade your system to have strlcpy in its C library
#define	strlcpy strncpy
#endif

#define BUFSIZE 2048		/* used to read the client response from stdin */
#define MAXADDRESSSIZE 65	/* 65 bytes should be enough ...
				   hold either d.c.b.a IPv4 in dot notation (15)
				   or IPv6 address nybblewise (63 chars) */

#endif
