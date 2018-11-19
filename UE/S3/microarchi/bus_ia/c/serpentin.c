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

#define ADDR 10

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

// ------------------------------------------------
//#define BIT(n) (1<<(n))

int ADDR_SRC  = ADDR;
int ADDR_DEST = ADDR;
int ADDR_FREQ = ADDR;

static void wtest_read(int*adest, int*reg, int*value)
{
    char* cmd;
    char rw[100]; int arg1,arg2;
    char prompt[100];
    sprintf(prompt,"entrer r|w|? [reg] [val] > ");

    while (1) {
        cmd=rl_gets(prompt);
        if ( cmd==0 ) {
            printf("\n");
            printf("EOF \n");
            while (1) pause();
        }
        int status=sscanf(cmd,"%s%d%x",rw,&arg1,&arg2);
fprintf(stderr,"st=%d ;%s;%d;%x;\n",status,rw,arg1,arg2);
        switch ( status ) {
          case 1:
            if ( rw[0]=='h' ) {
	    	printf("  h   : help\n");
	    	printf("  f # : set frequence to #\n");
		continue;
	    } else
                fprintf(stderr,"invalid command\n");
	    break;
          case 2:
            if ( rw[0]=='f' ) {
                *adest  = ADDR_FREQ;
                *reg   = 0;
                *value = arg1;
                return;
            } else
                fprintf(stderr,"invalid command\n");
            break;
        }
    }
}

static void wtest()
{
    //reset();
int cnt=0;
    Tmessage mess;
    int adest,reg,value;
    while (1) {
        wtest_read(&adest,&reg,&value);
        mess.ctl  = 0;
        mess.asrc = ADDR;
        mess.ades = adest;
        mess.uval = value&0xFFFFFF ;
        mess_send(&mess);
    }
}

static void rtest()
{
    Tmessage mess;
    while (1) {
        mess = mess_receive();
        printf("usb: from=%3d ; value=0x%06x\n",mess.asrc,mess.uval);
    }
}

// -----------------------------------------------------------
