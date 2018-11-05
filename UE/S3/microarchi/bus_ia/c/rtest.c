/*#########################################################################
# $Software: busiac
# $section : rs232-read & write
#       $Id: rtest.c 323 2015-05-29 09:36:23Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/rtest.c $
# $Author  : Ivan Auge (Email: auge@ensiie.fr)
# #########################################################################
# 
# This file is part of the BUSIAC software: Copyright (C) 2010 by I. Auge.
#
# This program is free software; you can redistribute it  and/or modify it
# under the  terms of the GNU  General Public License  as published by the
# Free Software Foundation;  either version 2 of the License,  or (at your
# option) any later version.
#
# BUSIAC software is distributed in the hope that it will be  useful, but
# WITHOUT  ANY  WARRANTY ;  without  even  the  implied  warranty  of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
#
# You should have received a copy  of the GNU General Public License along
# with the GNU C Library; see the  file COPYING. If not, write to the Free
# Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# #######################################################################*/

#define _BSD_SOURCE
#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#include "ttyS.h"

#define TTY_NAME "/dev/ttyS0"
//#define TTY_NAME "/dev/ttyS1"
//#define TTY_NAME "/dev/ttyUSB0"

//#define TTY_BAUD B300
//#define TTY_BAUD B300
#define TTY_BAUD B9600
//#define TTY_BAUD B19200
//#define TTY_BAUD B115200


int main(int argc, char**argv)
{
    int vt100;
    int nb=0;
    unsigned char c=0;

	printf("%s: %s\n",argv[0],TTY_NAME);

	vt100 = ttyS_open(TTY_NAME,O_RDONLY,TTY_BAUD);

	while (1) {
	    c = ttyS_read(vt100)&0xff;
	    printf("read %3dth : %08x\n",nb,c); fflush(stdout);
	    nb++;
    }
}
