/*#########################################################################
# $Software: busiac
# $section : rs232-mux (driver)
#       $Id: usb-ctl.c 330 2015-06-05 09:27:16Z ia $
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/usb-ctl.c $
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "driver.h"
#include "usb.h"

#define  HISPTORY_FILE "usb-test"
#include "pretty_input.h"

// ------------------------------------------------
//#define BIT(n) (1<<(n))

# define ADDR        10
# define ADDR_SRC  ADDR
# define ADDR_DEST ADDR
# define ADDR_FREQ ADDR
# define ADDR_PROG ADDR
# define ADDR_CHCK ADDR
# define PROMPT "> "

#define SERP_CMD_SetFreq         0

static void wtest();
static void rtest();

static Tmdm_param serp_handler __automatic_start = {
    .name          = "serp wrapper",
    .addrprocessor = ADDR,
    .samewindow    = 0,
    .rdwr          = rtest,
    .wronly        = wtest,
    .wronly_geom   = "100x10+0+0",
    .rdwr_geom     = "100x15+0+160",
};



static void wtest_read(int * adest, int * value) {
	char* cmd;
	char rw[100];
	unsigned int arg1, arg2;

	while (1) {
		cmd = rl_gets(PROMPT);
		if ( cmd==0 ) {
			printf("\n");
			printf("EOF \n");
			while (1) pause();
		}
		int status = sscanf(cmd, "%s%u%u", rw, &arg1, &arg2);
		fprintf(stderr, "st=%d ;%s;%u;%u;\n", status, rw, arg1, arg2);
		switch ( status ) {
			case 1:
				switch ( rw[0] ) {
					case 'f':
						printf("  h   : help\n");
						printf("  c   : clear the programmable serpentin\n");
						printf("  f # : set frequence to X (0 < # < 2^24)\n");
						printf("  t # : enable/disable h1000 ticking (# in {0, 1})\n");
						printf("  n # : set number of frames to be looped (# in [|1, 32|])\n");
						printf("  s # @ : set the segments # for the frame @ (# in [|0b0000000, 0b0000001, ..., 0b1111111|] and @ in [|0, 31|])\n");
						break ;
					case 'c':
						*adest = ADDR_PROG;
						*value = 0b000000000000000000000000;
						break ;
					default:
						goto cmd_error;
				}
			case 2:
				switch ( rw[0] ) {
					case 'f':
						*adest  = ADDR_FREQ;
						*value = arg1;
						break ;
					case 't':
						*adest = ADDR_CHCK;
						*value = arg1 ? 1 : 0;
						break ;
					case 'n':
						if (arg1 < 1 || arg1 > 32) {
							goto cmd_error;
						}
						*adest = ADDR_PROG;
						*value = 0b010000000000000000000000 | arg1;
						break ;
					default:
						goto cmd_error;
				}
			case 3:
				switch (rw[0]) {
					case 's':
						if (arg1 > 0b1111111 || arg2 >= 32) {
							goto cmd_error;
						}
						*adest = ADDR_PROG;
						*value  = 0b100000000000000000000000 | (arg1 & 0b111111100000) | arg2;
						break ;
					default:
						goto cmd_error;
				}
			default:
cmd_error:
				fprintf(stderr,"Invalid command\n");

		}
	}
}

static void wtest() {
	//reset();
	int cnt=0;
	Tmessage mess;
	int adest,value;
	while (1) {
		wtest_read(&adest, &value);
		mess.ctl  = 0;
		mess.asrc = ADDR;
		mess.ades = adest;
		mess.uval = value & 0xFFFFFF ;
		mess_send(&mess);
	}
}

static void rtest() {
	Tmessage mess;
	while (1) {
		mess = mess_receive();
		printf("usb: from=%3d ; value=0x%06x\n",mess.asrc,mess.uval);
	}
}

// -----------------------------------------------------------
