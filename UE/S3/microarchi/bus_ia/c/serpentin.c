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

# define ADDR_N     10
# define ADDR_SS    11
# define ADDR_CHCK  12
# define PROMPT "Enter a command ('h' for help) > "

static void wtest();
static void rtest();

static Tmdm_param n_handler __automatic_start = {
    .name          = "N",
    .addrprocessor = ADDR_N,
    .samewindow    = 0,
    .rdwr          = rtest,
    .wronly        = wtest,
    .wronly_geom   = "80x20+0+0",
    .rdwr_geom     = "80x20+0+400",
};

static Tmdm_param ss_handler __automatic_start = {
    .name          = "SS",
    .addrprocessor = ADDR_SS,
    .samewindow    = 0,
    .rdwr          = rtest,
    .wronly        = wtest,
    .wronly_geom   = "80x20+600+0",
    .rdwr_geom     = "80x20+600+400",
};

static Tmdm_param chck_handler __automatic_start = {
    .name          = "CHCK",
    .addrprocessor = ADDR_CHCK,
    .samewindow    = 0,
    .rdwr          = rtest,
    .wronly        = wtest,
    .wronly_geom   = "80x20+1200+0",
    .rdwr_geom     = "80x20+1200+400",
};


static void wtest_read(int * adest, int * value) {
	char* cmd;
	char rw[100];
	unsigned int arg1 = -1;
	unsigned int arg2 = -1;

	while (1) {
		cmd = rl_gets(PROMPT);
		if ( cmd==0 ) {
			printf("\n");
			printf("EOF \n");
			while (1) pause();
		}
		int status = sscanf(cmd, "%s%u%u", rw, &arg1, &arg2);
		fprintf(stderr, "st=%d ; cmd=%s ; arg1=%u ; arg2=%u\n", status, rw, arg1, arg2);
		switch ( status ) {
			case 1:
				switch ( rw[0] ) {
					case 'h':
						printf("  h   : help\n");
						printf("  c   : clear the programmable serpentin\n");
						printf("  f # : set frequence to X (0 < # < 2^24)\n");
						printf("  t # : enable/disable h1000 ticking (# in {0, 1})\n");
						printf("  n # : set number of frames to be looped (# in [|1, 32|])\n");
						printf("  s # @ : set the segments @ for the frame for the frame # (# in [|1, 32|] and @ in [|0b0000000, 0b0000001, ..., 0b1111111|])\n");
						continue ;
					case 'c':
						*adest = ADDR_SS;
						*value = 0b000000000000000000000000;
						return ;
					default:
						goto cmd_error;
				}
			case 2:
				switch ( rw[0] ) {
					case 'f':
						*adest  = ADDR_N;
						*value = 0b000000000000000000000000 | (arg1 & 0b001111111111111111111111);
						return ;
					case 't':
						*adest = ADDR_CHCK;
						*value = arg1 ? 1 : 0;
						return ;
					case 'n':
						if (arg1 < 1 || arg1 > 32) {
							goto cmd_error;
						}
						*adest = ADDR_SS;
						*value = 0b010000000000000000000000 | arg1;
						return ;
					default:
						goto cmd_error;
				}
			case 3:
				switch (rw[0]) {
					case 's':
						if (arg1 < 1 || arg1 > 32 || arg2 > 0b1111111) {
							goto cmd_error;
						}
						*adest = ADDR_SS;
						*value  = 0b100000000000000000000000 | ((arg2 & 0b1111111) << 6) | arg1;
						return ;
					default:
						goto cmd_error;
				}
			default:
cmd_error:
				fprintf(stderr,"Invalid command\n");
				continue ;

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
		mess.asrc = 255;
		mess.ades = adest;
		mess.uval = value & 0xFFFFFF ;
		printf("sending msg ctl=%d ; asrc=%d ; ades=%d ; uval=%u\n", mess.ctl, mess.asrc, mess.ades, mess.uval);
		mess_send(&mess);
		printf("sent msg ctl=%d ; asrc=%d ; ades=%d ; uval=%u\n", mess.ctl, mess.asrc, mess.ades, mess.uval);
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
