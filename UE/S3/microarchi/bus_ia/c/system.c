/*#########################################################################
# $Software: busiac
# $section : rs232-mux (engine)
#       $Id: system.c 326 2015-06-03 19:17:37Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/system.c $
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
int clock_gettime(clockid_t clk_id, struct timespec *tp);
#endif

#include "driver.h"
#include "system.h"

/*******************************************************************/

extern Tmessage mess_receive ()
{
    int status;
    Tmessage mess;
    if ( (status=read(FD_FROM_FPGA,&mess,sizeof(mess)))<0 ) {
        fprintf(stderr,"mess_receive: failure : %s\n",strerror(errno));
        sleep(60);
        exit(1);
    } else if ( status!=sizeof mess ) {
        fprintf(stderr,"mess_receive: failure : status=%d\n",status);
        sleep(60);
        exit(1);
    }
    return mess;
}

extern void     mess_send    (Tmessage *mess)
{
    int status;
    if ( (status=write(FD_TO_FPGA,mess,sizeof(*mess))) < 0 ) {
        fprintf(stderr,"mess_send: failure : %s\n",strerror(errno));
        sleep(60);
        exit(1);
    } else if ( status!=sizeof(*mess) ) {
        fprintf(stderr,"mess_send: failure : status=%d\n",status);
        sleep(60);
        exit(1);
    }
}

/*******************************************************************/
extern void messserial_readPipe(Tmessserial* ms, int fpga, unsigned long long timeBetweenTwoWrite)
{
    char* p = (char*) &ms->mess;
    int status = read(ms->fd,p+ms->nb,1);
    if (status<0) {
        fprintf(stderr,"read from %s driver failed : %s\n",ms->p->name,strerror(errno));
        exit(1);;
    } else if (status==0) {
        fprintf(stderr,"read from %s driver failed : status=%d\n",ms->p->name,status);
        return;
    }
    ms->nb += 1;
    if ( ms->nb == sizeof ms->mess ) {
#if 0
fprintf(stderr,
  "read mess from %s :%2d/%2d: ctl=%1d asrc=%3d ades=%3d uval=0x%08x\n",
  ms->p->name, ms->nb,sizeof ms->mess,
  ms->mess.ctl,ms->mess.asrc,ms->mess.ades,ms->mess.uval);
#endif
        messserial_writeFpga(ms,fpga,timeBetweenTwoWrite);
        ms->nb = 0;
    }
}

extern int messserial_readFpga(Tmessserial* ms, int fpga)
{
    Tmessage* m=&ms->mess;
    unsigned char c;
    if ( ms->nb==0 ) {
        memset(m,0,sizeof ms->mess);
    }
    int status = read(fpga,&c,1);
    if (status<0) {
        fprintf(stderr,"read from fpga failed : %s\n",strerror(errno));
        exit(1);;
    } else if (status==0) {
        fprintf(stderr,"read from fpga failed : status=%d\n",status);
        return 0;
    }
//fprintf(stderr,"read from fpga %d/6 0x%02x\n",ms->nb,c);
    if ( ms->nb==0 ) {
        m->uval |= (c<< 0); 
        ms->nb += 1;
    } else if ( ms->nb==1 ) {
        m->uval |= (c<< 8); 
        ms->nb += 1;
    } else if ( ms->nb==2 ) {
        m->uval |= (c<<16); 
        ms->nb += 1;
    } else if ( ms->nb==3 ) {
        m->ades = c;
        ms->nb += 1;
    } else if ( ms->nb==4 ) {
        m->asrc = c;
        ms->nb += 1;
    } else if ( ms->nb==5 ) {
        m->ctl = c;
        ms->nb =0;
        return 1;
    }
    return 0;
}

extern void messserial_writeFpga(Tmessserial* ms, int fpga, unsigned long long timeBetweenTwoWrite)
{
    static unsigned long long tmLast=0;  // in us
    unsigned long long tmCurr,diff;
    struct timespec tm;
    
    clock_gettime(CLOCK_REALTIME,&tm);
    tmCurr = tm.tv_sec*1000000LL + tm.tv_nsec/1000LL;
    diff = tmCurr-tmLast;
    //fprintf(stderr,"writeFpga: duration is %Ld us --> wait %Ld us\n",
    //        diff,timeBetweenTwoWrite-diff);
    if ( diff < timeBetweenTwoWrite )
        usleep(timeBetweenTwoWrite-diff);
    tmLast = tmCurr;
    
    Tmessage* m=&ms->mess;
    unsigned char c[4];
    c[0] = (m->uval>> 0)&0xff;
    c[1] = (m->uval>> 8)&0xff;
    c[2] = (m->uval>>16)&0xff;
    c[3] = (m->ades)    &0xff;
    int status = write(fpga,c,4);
fprintf(stderr,"messserial_writeFpga( %02x %02x %02x %02x --> fd=%d\n", c[0],c[1],c[2],c[3],fpga);
    if (status<0) {
        fprintf(stderr,"write to fpga from %s failed : %s\n",ms->p->name,strerror(errno));
        exit(1);;
    } else if (status!=4) {
        fprintf(stderr,"write to fpga from %s failed : status=%d\n",ms->p->name,status);
        exit(1);
    }
}

/*******************************************************************/

Tmessserial sys_rfiles[2*SYS_DRIVERMAX+1];
int         sys_rfilesnb;
Tmessserial sys_wfiles[1*SYS_DRIVERMAX];
int         sys_wfilesnb;

extern void sys_file_addentry(int read, int fd, Tmdm_param* p, int pidrw, int pidwo)
{
    Tmessserial* ms;
    if ( read ) {
        ms = sys_rfiles + sys_rfilesnb;
        sys_rfilesnb += 1;
    } else {
        ms = sys_wfiles + sys_wfilesnb;
        sys_wfilesnb += 1;
    }
    ms->fd     = fd;
    ms->nb     = 0;
    ms->p      = p;
    ms->pidrw  = pidrw;
    ms->pidwo  = pidwo;
}

extern Tmessserial* sys_file_getRentry_byFd(int fd)
{
    int i;
    for (i=0 ; i<sys_rfilesnb ; i+=1) {
        if ( sys_rfiles[i].fd==fd )
            return sys_rfiles+i;
    }
    return 0;
}

extern Tmessserial* sys_file_getWentry_byAddr(int addr)
{
    int i;
    for (i=0 ; i<sys_wfilesnb ; i+=1) {
        if ( sys_wfiles[i].p && sys_wfiles[i].p->addrprocessor==addr )
            return sys_wfiles+i;
    }
    return 0;
}

/*******************************************************************/


// splits str in sub-strings (eg: str="aa bb", substrings are aa and bb)
// and appends the substring to args array.
// notes: no memory allocation is performed, but str is altered.
static void sys_init_SplitStrIntoArgs(char **args, int *argsnb, char* str)
{
#define isSpace(c) ((c)==' ' || (c)=='\t')
    while (1) {
        // get start of first substring of str
        char *start;
        while ( isSpace(*str) ) str++;
        start=str;
        if (start==0) break;
        // here, the substring exists cause: *str is not nul and not a space
        // write '\0' at the end  of first substring of str
        char lastchar; // we preserve the char just following the substring
        while ( *str!=0 && !isSpace(*str) ) str++;
        lastchar=*str;
        *str=0;
        // add substring
        args[*argsnb] = start; *argsnb +=1;
        if (lastchar==0) break;
        str+=1;
    }
}

static int sys_init_CreateProcess(int rdwr, const Tmdm_param* p, int*m2rdwr, int*rdwr2m, int*wronly2m, int fdm, char fns[])
{
    char* wingeometry = rdwr!=0 ? p->rdwr_geom  : p->wronly_geom;
    char* xargs       = rdwr!=0 ? p->rdwr_xargs : p->wronly_xargs;
    struct termios slave;
    if ( tcgetattr(0, &slave)<0 ) { 
        fprintf(stderr, "fils-xterm:Error %d on tcgetattr() : %s\n", errno,strerror(errno));
        return 1;
    }

    int fils;
    if ( (fils=fork())==0 ) { 
        // fils xterm 
        char* args[50]; int argsnb=0;
        args[argsnb++]="xterm"; args[argsnb++]="-S/0";
        char title[1000];
        sprintf(title,"%s addr=%d (%s)", p->name,p->addrprocessor, rdwr==1 ? "rw" : "wo");
        args[argsnb++] = "-T"; args[argsnb++]=title;
        args[argsnb++] = "-n"; args[argsnb++]=p->name;
        if ( wingeometry ) {
            args[argsnb++] = "-geometry";
            args[argsnb++] = wingeometry;
        }
        if ( xargs ) {
            char tmp[1000]; strcpy(tmp,xargs);
            sys_init_SplitStrIntoArgs(args,&argsnb,tmp);
        }
        args[argsnb] = 0;
        { // print xterm cmd before closing std I/O streams
            char **p=args;
            fprintf(stderr,"Starting: xterm:");
            while (*p) fprintf(stderr," %s",*p++);
            fprintf(stderr,"\n");
        }

        dup2(fdm,0);
        close(1);
        close(2);
        close(fdm);
        execvp("xterm",args);
        exit(1);
    }
    
    int m2p[2];
    int p2m[2];
    if ( rdwr==1) pipe(m2p); else m2p[0]=m2p[1]=-1;
    pipe(p2m);
//fprintf(stderr,"pipe: m2p=%d/%d p2m=%d/%d\n", m2p[0], m2p[1],p2m[0], p2m[1]);
    if ( (fils=fork())==0 ) { 
        // fils software dédié au processeur
        close(fdm);
        int fds;
        if ( (fds = open(fns, O_RDWR)) < 0 ) {
            fprintf(stderr, "fils-soft:Error %d on open(%s) : %s\n", errno,fns,strerror(errno));
            exit(1);
        }
        slave.c_lflag &= ~ECHO;
        if ( tcsetattr (fds, TCSANOW, &slave)<0 ) {
            fprintf(stderr, "fils-soft:Error %d on tsgetattr() : %s\n", errno,strerror(errno));
            exit(1);
        }
#if 0
if (rdwr==1)
fprintf(stderr,"dup2(m2p[0]=%d,FD_FROM_FPGA=%d);\n",m2p[0],FD_FROM_FPGA);
fprintf(stderr,"dup2(p2m[1]=%d,FD_TO_FPGA=%d);\n",p2m[1],FD_TO_FPGA);
#endif
        dup2(fds,0);
        dup2(fds,1);
        dup2(fds,2);
        if (rdwr==1) {
            dup2(m2p[0],FD_FROM_FPGA);
            close(m2p[0]); close(m2p[1]);
        }
        dup2(p2m[1],FD_TO_FPGA);
        close(p2m[0]); close(p2m[1]);
        {   // xterm writes the window-id into fds.
            // to mask it,
            //   1) we unset ECHO flag (above)
            //   2) read id
            //   3) set ECHO flag again.
            unsigned int winid;
            scanf("%x",&winid);
            slave.c_lflag |= ECHO;
            tcsetattr (fds, TCSANOW, &slave);
        }
        if (rdwr==1)
            p->rdwr();
        else
            p->wronly();
        sleep(5);
        exit(1);
    }
    sleep(1);
    // pere 
    if ( rdwr == 1 ) {
        close(m2p[0]);
        *m2rdwr=m2p[1];
    }
    close(p2m[1]); 
    if ( rdwr == 1 )
        *rdwr2m=p2m[0];
    else
        *wronly2m=p2m[0];
    return fils;
}

static int sys_init_CreateMaster(char fsn[])
{
    int fdm;
    if ( (fdm=posix_openpt(O_RDWR)) < 0 ) {
        fprintf(stderr, "Error %d on posix_openpt()\n", errno);
        exit(1);
    }
    if ( grantpt(fdm) != 0) {
        fprintf(stderr, "Error %d on grantpt()\n", errno);
        exit(1);
    }
    if ( unlockpt(fdm)!=0) {
        fprintf(stderr, "Error %d on unlockpt()\n", errno);
        return 1;
    }

    strcpy(fsn,ptsname(fdm));
    return fdm;
}

__asm__(
".section driver_table,\"aw\",@progbits\n"
".section driver_table_END,\"aw\",@progbits\n"
//"driver_table_end: .long driver_table - driver_table_end\n"
"driver_table_end: .long -1\n"
);

extern Tmdm_param driver_table[];
extern Tmdm_param driver_table_END[];

extern int sys_init(const char* progname)
{
    Tmdm_param* p;
    for (p=driver_table ; p<driver_table_END ; p+=1) {
        printf("p=%p\n",p);
        printf("p=%p %s\n",p,p->name);
        int pidrw=-1,  pidwo=-1;
        int m2rdwr=-1, rdwr2m=-1, wronly2m = -1;

        // création du premier processus
        char  fns[100];
        int   fdm = sys_init_CreateMaster(fns);
        
        pidwo = sys_init_CreateProcess(1, p, &m2rdwr, &rdwr2m, &wronly2m, fdm, fns);
        if ( p->wronly == 0 ) {
            close(fdm);
            sys_file_addentry(0,m2rdwr,p,pidrw,pidwo);
            sys_file_addentry(1,rdwr2m,p,pidrw,pidwo);
            continue;
        }
        if ( p->samewindow==0 ) {
            close(fdm);
            fdm = sys_init_CreateMaster(fns);
        }
        pidwo = sys_init_CreateProcess(0, p, &m2rdwr, &rdwr2m, &wronly2m, fdm, fns);
#if 0
fprintf(stderr,"%s: m2rdwr=%d rdwr2m=%d wronly2m=%d\n",
        p->name, m2rdwr, rdwr2m, wronly2m);
#endif
        sys_file_addentry(0,m2rdwr,   p, pidrw, pidwo);
        sys_file_addentry(1,rdwr2m,   p, pidrw, -1);
        sys_file_addentry(1,wronly2m, p, -1,    pidwo);
        close(fdm);
    }
    return 0;
}
/*******************************************************************/
