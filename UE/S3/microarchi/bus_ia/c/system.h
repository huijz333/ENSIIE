/*#########################################################################
# $Software: busiac
# $section : rs232-mux (engine)
#       $Id: system.h 320 2015-05-29 05:21:22Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/system.h $
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

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

typedef struct _Tmessserial {
    int        fd;    /* file descriptor for reading from or writing to */
    int        nb;    /* nb char already read or wrote mess */
    Tmessage   mess;  /* the message to read or write */
    Tmdm_param *p;    /* the BUS-IA driver corresponding to fd */ 
    int        pidrw; /* the pid of the p->rdwr task (-1 if none) */
    int        pidwo; /* the pid of the p->wronly task (-1 if none) */
} Tmessserial;

/* Accumulate a byte from ms->fd into ms->mess.
 * If ms->mess is completed, it is send to fpga
 * and ms->nb is reseted to 0. */
void messserial_readPipe(Tmessserial* ms, int fpga, unsigned long long timeBetweenTwoWrite);

/* Accumulate a byte from fpga into ms->mess.
 * If ms->mess is not completed, 0 is returned.
 * If ms->mess is completed, 1 is returned
 * and ms->nb is reseted to 0.
**/
extern int messserial_readFpga(Tmessserial* ms, int fpga);

/* Write message ms->mess to fpga. */
extern void messserial_writeFpga(Tmessserial* ms, int fpga, unsigned long long timeBetweenTwoWrite);

/**
 * Data table
**/

#define SYS_DRIVERMAX 100
extern Tmessserial sys_rfiles[2*SYS_DRIVERMAX+1]; // read by main
extern int         sys_rfilesnb;
extern Tmessserial sys_wfiles[1*SYS_DRIVERMAX]; // written by main
extern int         sys_wfilesnb;

extern void         sys_file_addentry(int readfile, int fd, Tmdm_param* p, int pidrw, int pidwo);
extern Tmessserial* sys_file_getRentry_byFd  (int fd);
extern Tmessserial* sys_file_getWentry_byAddr(int addr);

/**
 * initialisation of system
**/

// 0:success ; -1:fatal failure
extern int sys_init(const char* progname);

#endif


