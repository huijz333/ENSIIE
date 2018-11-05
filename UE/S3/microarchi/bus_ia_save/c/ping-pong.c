/*#########################################################################
# $Software: busiac
# $section : rs232-mux (driver)
#       $Id: ping-pong.c 323 2015-05-29 09:36:23Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/ping-pong.c $
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
#include <stdio.h>
#include <unistd.h>

#include "driver.h"

//#define PLEIN_POT

static void ping();
static void pong();

static Tmdm_param pingpong __automatic_start = {
    .name          = "pingpong",
    .addrprocessor = 255,
    .samewindow    = 0,
    .rdwr          = pong,
    .wronly        = ping,
    .wronly_geom   = "100x10+0+0",
    .rdwr_geom     = "100x15+0+160",
};

#define MESSVAL(cnt) (((((cnt)+2)%256)<<16 | (((cnt)+1)%256)<<8 | (cnt)%256))

static void ping()
{
    unsigned int cnt=0;
    int nbmess=0;
    Tmessage mess;
    while (1) {
        //if ( nbmess == 3 ) {sleep(5); continue;}
        nbmess+=1;
        mess.ctl     = 0;
        mess.asrc    = 255;
        mess.ades    = 255;
        mess.uval = MESSVAL(cnt);
        //if ( (rand()%100)==0 ) mess.uval += 0x800000;
        mess_send(&mess);
        cnt = (cnt+1)%256;
#ifdef PLEIN_POT
        if (nbmess%1000!=0)
#endif
            fprintf(stderr,"ping:%5d: to=%3d ; value=0x%06x\n",
                nbmess,mess.ades,mess.uval);
#ifndef PLEIN_POT
        usleep(2000000);
#endif
    }
}

#include <time.h>

static void pong()
{
    unsigned int cnt=0,nberror=0;
    int nbmess=1;
    Tmessage mess;
#ifdef PLEIN_POT
    time_t tp0 = time((void*)0);
#endif

    while (1) {
        nbmess+=1;
        mess = mess_receive();
        if ( MESSVAL(cnt) != mess.uval ) {
            nberror += 1;
            if ( MESSVAL(mess.uval&0xFF) == mess.uval )
                cnt = mess.uval&0xFF;
        }
        cnt = (cnt+1)%256;
#ifndef PLEIN_POT
        fprintf(stderr,"pong:%5d:%5d: from=%3d ; value=0x%06x\n",
            nbmess,nberror,mess.asrc,mess.uval);
#else
        if (nbmess%1000==0) {
            time_t tp = time((void*)0);
            double duree,debit;
            if ( tp == tp0 )
                duree=debit=0;
            else  {
                duree=tp; duree -= tp0;
                debit=1000; debit = debit/duree;
                tp0 = tp;
                fprintf(stderr,"pong:%5d:%5d: from=%3d ; value=0x%06x duree=%e s  debit=%e mess/s\n",
                    nbmess,nberror,mess.asrc,mess.uval,duree,debit);
            }
        }
#endif
    }
}

