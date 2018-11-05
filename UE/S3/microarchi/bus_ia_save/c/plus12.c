/*#########################################################################
# $Software: busiac
# $section : rs232-mux (driver)
#       $Id: plus12.c 323 2015-05-29 09:36:23Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/plus12.c $
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
#include <unistd.h>
#include <string.h>

#include "driver.h"

#define  HISPTORY_FILE "h_plus12"
#include "pretty_input.h"

#define ADDR    192

static void rdwr();

static Tmdm_param plus12 __automatic_start = {
    .name          = "plus12",
    .addrprocessor = ADDR,
    .samewindow    = 0,
    .rdwr          = rdwr,
    .wronly        = 0
};

static void plus12_read(unsigned int *a, unsigned int* b)
{
    int  min = -(0x800);
    int  max = 0x7FF;
    char prompt[1000];
    sprintf(prompt,"entrer a et b (%d<=x<=%d) > ",min,max);

    char* cmd;
    while (1) {
        cmd=rl_gets(prompt);
        if ( cmd==0 ) {
            printf("\n");
            printf("EOF \n");
            while (1) pause();
        }
        int x,y;
        if ( sscanf(cmd,"%d%d",&x,&y)!=2 ) {
            fprintf(stderr,"missing args or bad values\n");
        } else if ( x<min || max<x ) {
            fprintf(stderr,"%d out of range\n",x);
        } else if ( y<min || max<y ) {
            fprintf(stderr,"%d out of range\n",y);
        } else {
            *a = x; *b=y;
            return;
        }
    }
}

static void rdwr()
{
    Tmessage mess;
    unsigned int a,b;
    while (1) {
        /* lecture des operandes a et b a additionner sur stdin */
        plus12_read(&a,&b);

        /* construction du message a envoyer */
        mess.ctl     = 0;
        mess.asrc    = 255;
        mess.ades    = plus12.addrprocessor;
        mess.uval    = (a&0xFFF) | ( (b&0XFFF)<<12 );
        printf("send: from=%3d ; to=%3d ; value=0x%06x\n",
                mess.asrc,mess.ades,mess.uval);
        mess_send(&mess);

        /* lecture du resultat et affichage */
        int r,ov,c,n,z;
        mess = mess_receive();
        printf("rec:  from=%3d ; to=%3d ; value=0x%06x\n",
                mess.asrc,mess.ades,mess.uval);
        r  = mess.uval&0xFFF; if ( (r&0x800)!=0) r |= 0xFFFFF000;
        ov = (mess.uval&0x800000) != 0 ;
        c  = (mess.uval&0x400000) != 0 ;
        n  = (mess.uval&0x200000) != 0 ;
        z  = (mess.uval&0x100000) != 0 ;
        printf("plus12: %5d+%5d=%4d ; ov=%d c=%d n=%d z=%d\n",
                a,b,r,ov,c,n,z);
    }
}

