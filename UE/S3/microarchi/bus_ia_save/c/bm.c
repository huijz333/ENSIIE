/*#########################################################################
# $Software: busiac
# $section : rs232-mux (driver)
#       $Id: bm.c 326 2015-06-03 19:17:37Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/bm.c $
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

#define  HISPTORY_FILE "h_bm"
#include "pretty_input.h"


#define dbg0_printf(...) 

#if 0
#undef  dbg0_printf
#define dbg0_printf(...) fprintf(stderr,__VA_ARGS__)
#endif

#define ADDR    176

static void rdwr();

static void bm_main();

static Tmdm_param plus12 __automatic_start = {
    .name          = "bm",
    .addrprocessor = ADDR,
    .samewindow    = 0,
    .rdwr          = bm_main,
    .rdwr_geom     = "100x15+0+160",
    .rdwr_xargs    = "-rv -fn 10x20",
};

unsigned long freq_ms2hard(unsigned int v)
{
    double h     = 50e6;              // horloge en hertz
    double vs    = v; vs = vs * 1e-3; // periode en seconde
    double tick  = h*vs;
    double shift = 1<<12;
    return tick/shift;
}    

// bm commands
#define BM_CMD_PLAY          0x01
#define BM_CMD_SCORET        0x02
#define BM_CMD_SCOREM        0x03
#define BM_CMD_PERDU         0x04
#define BM_CMD_LoadFreq0     0x11
#define BM_CMD_LoadFreq1     0x12
#define BM_CMD_LoadFreq2     0x14
#define BM_CMD_LoadFreqA     0x17
#define BM_CMD_LoadSymbol0   0x21
#define BM_CMD_LoadSymbol1   0x22
#define BM_CMD_LoadSymbol2   0x24
#define BM_CMD_LoadSymbolA   0x27
#define BM_CMD_LoadNbSymbol0 0x31
#define BM_CMD_LoadNbSymbol1 0x32
#define BM_CMD_LoadNbSymbol2 0x34
#define BM_CMD_LoadNbSymbolA 0x37

// bm answers
#define BM_STATUS_RESULT 0x0
#define BM_STATUS_OK     0x1
#define BM_STATUS_ERR    0x7

static void bm_init();
static void bm_init2();
static void bm_partie();

// reads commands from stdin and returns them in action, param
// arrays. the returned value *nbparam indicates the array size.
static void bm_read(unsigned int *action, unsigned int* param, int* nbparam)
{
    char prompt[100];
    char buf1[100];
    char buf2[100];
    sprintf(prompt,"entrer cmd (? help) > ");

    char* cmd;
    while (1) {
        cmd=rl_gets(prompt);
        if ( cmd==0 ) {
            printf("\n");
            printf("EOF \n");
            while (1) pause();
        }
        unsigned int x0,x1,x2;
        int status=sscanf(cmd,"%s%d%d%d",buf1,&x0,&x1,&x2);
        if ( status<1 ) {
            fprintf(stderr,"missing command\n");
        } else if ( *buf1 == '?' || *buf1 == 'h' || *buf1=='H') {
            printf("   h[elp]           : cette aide.\n");
            printf("   r[ound]          : joue une manche.\n");
            printf("   f[requnce] #     : fixe la frequence de tous\n"
                   "                      les digits à # ms.\n");
            printf("   f[requnce] l m r : fixe la frequence du digit\n"
                   "                      gauche à l ms, du milieu à\n"
                   "                      m ms et le droit à d ms.\n");
            printf("   n #              : fixe la taille des sequences des\n"
                   "                      digits à # élément (#<16).\n");
            printf("   n l m r          : mais la taille de la sequence du\n"
                   "                      digit gauche à l, du milieu à m\n"
                   "                      et le droit à d (l,m,d <16).\n");
            printf("   s[eq] #          : ajoute le carctères  à la séquence\n"
                   "                      des digits (#<=127).\n");
            printf("   is #             : charge la sequence de symboles numéro #\n");
            printf("   p                : lance une partie\n");
        } else if ( *buf1 == 'r' ) {
            *action = BM_CMD_PLAY;
            *param  = 0;
            *nbparam= 1;
            return;
        } else if ( *buf1 == 'f' ) {
            if ( status==2 ) {
                *action = BM_CMD_LoadFreqA;
                *param  = freq_ms2hard(x0);
                *nbparam= 1;
                return;
            } else if ( status==4 ) {
                action[0] = BM_CMD_LoadFreq2; param[0] = freq_ms2hard(x0);
                action[1] = BM_CMD_LoadFreq1; param[1] = freq_ms2hard(x1);
                action[2] = BM_CMD_LoadFreq0; param[2] = freq_ms2hard(x2);
                *nbparam= 3;
                return;
            }
            fprintf(stderr, "commande %s mal formatée.\n", buf1);
        } else if ( *buf1 == 'n' ) {
            if ( status==2 ) {
                *action = BM_CMD_LoadNbSymbolA;
                *param  = x0&0xF;
                *nbparam= 1;
                return;
            } else if ( status==4 ) {
                action[0] = BM_CMD_LoadNbSymbol2; param[0] = x0 & 0xF;
                action[1] = BM_CMD_LoadNbSymbol1; param[1] = x1 & 0xF;
                action[2] = BM_CMD_LoadNbSymbol0; param[2] = x2 & 0xF;
                *nbparam= 3;
                return;
            }
            fprintf(stderr,
                "commande %s mal formatée.\n", buf1);
        } else if ( *buf1 == 's' ) {
            if ( status==2 ) {
                *action = BM_CMD_LoadSymbolA;
                *param  = x0 & 0x7f;
                *nbparam= 1;
                return;
            }
            fprintf(stderr,
                "commande %s mal formatée.\n", buf1);
        } else if ( buf1[0] == 'i' && buf1[1] == 's' && status==2 ) {
            if      ( x0==1 ) bm_init();
            else if ( x0==2 ) bm_init2();
            else fprintf(stderr,"%s %d est incorrect (valid: 1 et 2)\n",buf1,x0);
        } else if ( *buf1 == 'p' ) {
            bm_partie();
        } else {
            fprintf(stderr,"%s est une commande inconnue\n",buf1);
        }
    }
}

Tmessage send_cmd(unsigned int cmd, unsigned int arg)
{
    Tmessage mess;

    // send the request
    mess.ctl     = 0;
    mess.asrc    = 255;
    mess.ades    = ADDR;
    mess.uval    = ((cmd&0xFF)<<16) | (arg&0XFFFF);
    dbg0_printf("-->: from=%3d ; to=%3d ; value=0x%06x\n",
            mess.asrc,mess.ades,mess.uval);
    mess_send(&mess);

    // get the answer 
    mess = mess_receive();
    dbg0_printf("<--: from=%3d ; to=%3d ; value=0x%06x\n",
            mess.asrc,mess.ades,mess.uval);
    unsigned int kind  = (mess.uval>>21) & 0X7;
    unsigned int param =  mess.uval&0x1FFFFF;
    switch ( kind ) {
        case BM_STATUS_RESULT:
            dbg0_printf("   : kind=%d (result) r2=%02x r1=%02x r0=%02x\n",
                    kind, (param>>14)&0x7F, (param>> 7)&0x7F, (param>> 0)&0x7F);
            break;
        case BM_STATUS_OK:
            dbg0_printf("   : kind=%d (oki)\n", kind);
            break;
        case BM_STATUS_ERR:
            dbg0_printf("   : kind=%d (error)\n",kind);
            break;
        default:
            dbg0_printf("   : kind=%d (? ? ?) param=%06x\n", kind, param);
            break;
    }
    return mess;
}

static void bm_main()
{
    Tmessage mess;
    unsigned int ta[10],tb[10],nbcmd;
    int i;

    while (1) {
        /* lecture des operandes a et b a additionner sur stdin */
        bm_read(ta,tb,&nbcmd);

        for ( i = 0 ; i<nbcmd ; i++ ) {
            unsigned int a = ta[i];
            unsigned int b = tb[i];
            mess = send_cmd(a,b);
            unsigned int kind  = (mess.uval>>21) & 0X7;
            unsigned int param =  mess.uval&0x1FFFFF;
            switch ( kind ) {
                case BM_STATUS_RESULT:
                    printf("Result: r2=%02x r1=%02x r0=%02x\n",
                            (param>>14)&0x7F, (param>> 7)&0x7F, (param>> 0)&0x7F);
                    break;
                case BM_STATUS_OK:
                    break;
                case BM_STATUS_ERR:
                default:
                    printf("Error\n",kind);
                    break;
            }
        }
    }
}

#define b(n) (1<<(n))

static char seq1[] = {
    b(6)|b(5) , b(5)|b(0) , b(0)|b(2) , b(2)|b(3) ,
    b(3)|b(4) , b(4)|b(0) , b(0)      , b(0)|b(1) ,
    b(1)|b(6)
};
#define seq1nb  sizeof(seq1)

static char seq2[] = {
    b(6)|b(5)|b(0) , b(5)|b(0)|b(2) , b(0)|b(2)|b(3) , b(2)|b(3)|b(4) ,
    b(3)|b(4)|b(0) , b(4)|b(0) ,      b(0)      , b(0)|b(1) ,
    b(0)|b(1)|b(6) , b(1)|b(6)|b(5)
};
#define seq2nb  sizeof(seq2)

static char seq3[] = {
    ~b(0) , ~b(1) , ~b(2) , ~b(3) , ~b(4) , ~b(5) , 
    ~b(0) , ~b(1) , ~b(2) , ~b(3) , ~b(4) , ~b(5) , 
    b(0)
};
#define seq3nb  sizeof(seq3)

static void bm_init()
{
    int i;
    
    for ( i=seq1nb-1 ; i>=0 ; i-=1)
        send_cmd(BM_CMD_LoadSymbol2,seq1[i]);
    send_cmd(BM_CMD_LoadNbSymbol2,seq1nb);
    for ( i=0 ; i<seq1nb ; i+=1)
        send_cmd(BM_CMD_LoadSymbol1,seq1[i]);
    send_cmd(BM_CMD_LoadNbSymbol1,seq1nb);
    for ( i=seq2nb-1 ; i>=0 ; i-=1)
        send_cmd(BM_CMD_LoadSymbol0,seq2[i]);
    send_cmd(BM_CMD_LoadNbSymbol2,seq2nb);

}

static void bm_init2()
{
    int i;
    
    for ( i=seq3nb-1 ; i>=0 ; i-=1)
        send_cmd(BM_CMD_LoadSymbol2,seq3[i]);
    for ( i=0 ; i<seq3nb ; i+=1)
        send_cmd(BM_CMD_LoadSymbol1,seq3[i]);
    for ( i=seq3nb-1 ; i>=0 ; i-=1)
        send_cmd(BM_CMD_LoadSymbol0,seq3[i]);

    send_cmd(BM_CMD_LoadNbSymbolA,seq3nb);
}

int plusOuMoins20pc(int freq)
{
    double pc = (rand()%40)-20;
    double f  = freq;
    f += f*pc/100.;
    freq = f;
    return freq_ms2hard(freq);
}

int bm_score(unsigned int symbol)
{

    if ( symbol == b(0) )
        return 2;
    if ( (symbol&b(0))==0 )
        return 0;
    int ret,i;
    for ( ret=0,i=1 ; i<=6 ; i++)
        if ( (symbol&b(i))!=0 )
            ret+=1;
    if (ret==1)
        return 1;
    else
        return 0;
}

static void bm_partie()
{
    Tmessage mess;
    int score_t=0,score_m,level,freq=1000;
    bm_init();
    send_cmd(BM_CMD_SCORET, 0 );
    send_cmd(BM_CMD_SCOREM, 0 );
    for (level=1 ; ; level+=1,freq -= (30*freq)/100 ) {
        send_cmd(BM_CMD_LoadFreq0, plusOuMoins20pc(freq));
        send_cmd(BM_CMD_LoadFreq1, plusOuMoins20pc(freq));
        send_cmd(BM_CMD_LoadFreq2, plusOuMoins20pc(freq));
        fprintf(stderr,"----->: manche %2d : freq %4dkH : ",level,freq);
        mess=send_cmd(BM_CMD_PLAY,0);
        unsigned int kind   = (mess.uval>>21) & 0X7;
        if ( kind!=BM_STATUS_RESULT ) {
            fprintf(stderr,"FATAL: pas de score renvoyé\n");
            return;
        }
        score_m  = bm_score( (mess.uval>>14) & 0x7F );
        score_m += bm_score( (mess.uval>> 7) & 0x7F );
        score_m += bm_score( (mess.uval>> 0) & 0x7F );
        score_t += score_m;
        send_cmd(BM_CMD_SCORET, (((score_t/10)<<4)&0xF0) | (score_t%10) );
        send_cmd(BM_CMD_SCOREM, score_m%10 );
        fprintf(stderr,"score-m=%d  score-t=%d\n", score_m,score_t);
        if (score_m <= 2) {
            send_cmd(BM_CMD_PERDU, 0 );
            fprintf(stderr,"PERDU: total=%d\n",score_t);
            return;
        }
        sleep(2);
    }
}
