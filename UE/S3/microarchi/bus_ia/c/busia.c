/*#########################################################################
# $Software: busiac
# $section : rs232-mux (engine)
#       $Id: busia.c 326 2015-06-03 19:17:37Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/busia.c $
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

#define _XOPEN_SOURCE 600
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "driver.h"
#include "system.h"
#include "ttyS.h"

//#define TTY_BAUD B300
#define TTY_BAUD B9600
//#define TTY_BAUD B19200
//#define TTY_BAUD B115200

static void ToFpga(char*prgname, int vt100);

int main(int argc,char*argv[])
{
    dup2(2,FD_FROM_FPGA);   // force pipe fd and vt100 to be != FD_FROM_FPGA
    dup2(2,FD_TO_FPGA);     // force pipe fd and vt100 to be != FD_TO_FPGA
    
    // check commande line 
    if (argc!=2) {
        fprintf(stderr,"%s: mauvais nombre d'argument.\n",argv[0]);
        fprintf(stderr,"usage: %s tty_path\n",argv[0]);
        exit(1);
    }

    // get stream to FPGA
    int vt100;
    vt100 = ttyS_open(argv[1],O_RDWR,TTY_BAUD);
    sys_file_addentry(1,vt100,0,-1,-1);

    // run drivers and build data structure (sys_files) 
    if ( sys_init(argv[0])<0 ) {
        fprintf(stderr,
            "%s:FATAL: build of data structure failed, aborted.\n",
            argv[0]);
        exit(1);
    }

    if  ( fork()!=0 ) { ToFpga(argv[0],vt100); exit(0); }
    struct pollfd fds[100];
    int    nfds=1;
    fds[0].fd      = vt100;
    fds[0].events  = POLLIN;
    fds[0].revents = 0;

    Tmessserial* ms= sys_file_getRentry_byFd(vt100);
//fprintf(stderr,"AAAAA:0: ms->nb=%d ms->fd=%d\n",ms->nb,ms->fd);
    while (1) {
        int status;
        Tmessserial *ms2;

#if 1
        status = poll(fds,nfds,-1);
        if (status==-1) {
            fprintf(stderr,"%s: poll failed : %s\n",argv[0],strerror(errno));
            exit(1);
        } else if (status==0)
            continue;
        int revents  = fds[0].revents;
        if ( revents == POLLERR ) {
            fprintf(stderr,"%s: poll error on stream to FPGA (POLLERR).\n",argv[0]);
            exit(1);
        }
        if ( revents == POLLHUP ) {
            fprintf(stderr,"%s: poll error on stream to FPGA (POLLHUP).\n",argv[0]);
            exit(1);
        }
        if ( revents == POLLNVAL ) {
            fprintf(stderr,"%s: poll error on stream to FPGA (POLLNVAL).\n",argv[0]);
            exit(1);
        }
#endif
        if ( messserial_readFpga(ms,vt100) == 1 ) {
            ms2= sys_file_getWentry_byAddr(ms->mess.asrc);
            if ( ms2==0 ) {
                fprintf(stderr,
                    "No driver found for message: " MESS_FMT "\n",
                    MESS_ARG(ms->mess));
            } else {
#if 0
                fprintf(stderr,
                    "Receive message:             " MESS_FMT "\n",
                    MESS_ARG(ms->mess));
#endif
                write(ms2->fd,&ms->mess,sizeof ms->mess);
            }
        }
    }
#if 0
    // main loop 
    //   if something from FPGA send a message to corresponding driver.
    //   if message from a driver send it to FPGA

    // do the poll structure
    struct pollfd fds[100];
    int nfds= poll_build(fds,vt100);

    while (1) {
        int i,status;
        Tmessserial *ms,*ms2;
        for (i=0 ; i<nfds ; i +=1) fds[i].revents=0;
        status = poll(fds,nfds,-1);

        if (status==-1) {
            fprintf(stderr,"%s: poll failed : %s\n",argv[0],strerror(errno));
            exit(1);
        } else if (status==0)
            continue;
        for ( i=0 ; i<nfds ; i +=1) {
            if ( fds[i].revents==0 ) continue;
            int fdpolled = fds[i].fd;
            int revents  = fds[i].revents;
//fprintf(stderr,"poll1: status=%d fdpolled=%d\n",status,fdpolled);       
            ms= sys_file_getRentry_byFd(fdpolled);
            if ( ms==0 ) {
                fprintf(stderr,"%s: poll failed to find fd: %d\n",argv[0],fdpolled);
                continue;
            }
            if ( fdpolled==vt100) {
//fprintf(stderr,"poll0: status=%d fdpolled=%d\n",status,fdpolled);       
                if ( revents == POLLERR ) {
                    fprintf(stderr,"%s: poll error on stream to FPGA (POLLERR).\n",argv[0]);
                    exit(1);
                }
                if ( revents == POLLHUP ) {
                    fprintf(stderr,"%s: poll error on stream to FPGA (POLLHUP).\n",argv[0]);
                    exit(1);
                }
                if ( revents == POLLNVAL ) {
                    fprintf(stderr,"%s: poll error on stream to FPGA (POLLNVAL).\n",argv[0]);
                    exit(1);
                }
                if ( messserial_readFpga(ms,vt100) == 1 ) {
                    ms2= sys_file_getWentry_byAddr(ms->mess.asrc);
                    if ( ms2==0 ) {
                        fprintf(stderr,
                            "No driver found for message: " MESS_FMT "\n",
                            MESS_ARG(ms->mess));
                    } else {
                        fprintf(stderr,
                            "Receive message:             " MESS_FMT "\n",
                            MESS_ARG(ms->mess));
                        write(ms2->fd,&ms->mess,sizeof ms->mess);
                    }
                }
            } else {
//fprintf(stderr,"poll3: status=%d fdpolled=%d\r",status,fdpolled);       
                messserial_readPipe(ms, vt100);
            }
        }
    }
#endif

    int status;
    wait(&status);
    return 0;
}

/**
 * process that transferts the pipes to the FPGA stream
**/
nfds_t ToFpga_PollBuild(struct pollfd* fds, int vt100)
{
    nfds_t i,nfds;
    for (i=0,nfds=0 ; i<sys_rfilesnb ; i+=1 ) { 
        if ( sys_rfiles[i].fd==vt100 ) continue;
        fds[nfds].fd = sys_rfiles[i].fd;
        fds[nfds].events = POLLIN;
        nfds+=1;
    }
    return nfds;
}

static void ToFpga(char*prgname, int vt100)
{
    int baud = TTY_BAUD== B300  ?   300 : TTY_BAUD==B1200   ?   1200 :
               TTY_BAUD==B4800  ?  4800 : TTY_BAUD==B9600   ?   9600 :
               TTY_BAUD==B19200 ? 19200 : TTY_BAUD==B115200 ? 115200 :
               100 ;
    unsigned long long timeout_base= (1000000*12*6)/baud; // 12 bits per byte * 6 bytes per message
    timeout_base *= 3;
    fprintf(stderr,"%s: use %Ld us for writing a message\n",prgname,timeout_base);

    struct pollfd fds[100];
    int nfds= ToFpga_PollBuild(fds,vt100);

    while (1) {
        int i,status;
        Tmessserial *ms;
        // for (i=0 ; i<nfds ; i +=1) fds[i].revents=0;
//fprintf(stderr,"%s: polling\n",prgname);
        status = poll(fds,nfds,-1);
        if (status==-1) {
            fprintf(stderr,"%s: poll failed : %s\n",prgname,strerror(errno));
            exit(1);
        } else if (status==0)
            continue;
        for ( i=0 ; i<nfds ; i +=1) {
            if ( fds[i].revents==0 ) continue;
            int fdpolled = fds[i].fd;
//fprintf(stderr,"poll1: status=%d fdpolled=%d\n",status,fdpolled);       
            ms= sys_file_getRentry_byFd(fdpolled);
            if ( ms==0 ) {
                fprintf(stderr,"%s: poll failed to find fd: %d\n",prgname,fdpolled);
                continue;
            }
//fprintf(stderr,"poll3: status=%d fdpolled=%d\r",status,fdpolled);       
            messserial_readPipe(ms, vt100, timeout_base);
        }
    }
}

#include "system.c"
#include "ttyS.c"
