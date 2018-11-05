/*#########################################################################
# $Software: busiac
# $section : rs232-mux (driver)
#       $Id: driver.h 326 2015-06-03 19:17:37Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/driver.h $
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

#ifndef FILE_DRIVER_H
#define FILE_DRIVER_H

/*******************************************************************
 * Pour définir le driver d'un composant XXX a l'adresse ADDR sur
 * le BUS-IA, il faut:
 *
 * 1) creer le fichier XXX.c
 *   - définir une variable globale de type Tmdm_param
 *      #include <stdio.h>
 *      #include "driver.h"
 *
 *      static void XXX_rw();
 *      static void XXX_wo();       // peut etre omis
 *
 *      Tmdm_param __automatic_start XXX_param = {
 *        .name          = "XXX",
 *        .addrprocessor = ADDR,
 *        .samewindow    = 0,       // voir commentaire Tmdm_param
 *        .rdwr          = XXX_rw,
 *        .wronly        = XXX_ro   // peut être omis
 *      };
 *   - puis ecrire la fonction XXX_rw et si besoin XXX_wo
 *      static void XXX_rw() { ... }
 *      static void XXX_wo() { ... }
 *
 *     Pour l'UI printf et scanf écrivent et lisent dans la fenêtre du driver,
 *     mess_send et mess_receive permettent de communiquer avec le FPGA.
**/

#if 1
#define __automatic_start __attribute__ ((section ("driver_table"),aligned(8)))
#else
#define __automatic_start 
#endif

/*******************************************************************
 * message formater
**/

typedef struct _Tmessage {
    unsigned int ctl;       /* control [3:0] */
    unsigned int asrc;      /* adresse source */
    unsigned int ades;      /* adresse destination */
    unsigned int uval;      /* data [23:0] */
} Tmessage;

#define FD_FROM_FPGA 3
#define FD_TO_FPGA   4

#define MESS_FMT "ctl=%1d ; asrc=%3d ; ades=%3d ; uval=0x%06x"
#define MESS_ARG(m) (m).ctl,(m).asrc,(m).ades,((m).uval)&0xFFFFFF

/* obtention d'un message du terminateur du BUS-IA
 *  ctl=0, ades=current->addr */
extern Tmessage mess_receive ();

/* envoie d'un message à l'initiateur du BUS-IA
 *  ctl=0, asrc=current->addr  */
extern void     mess_send    (Tmessage *mess);

/*******************************************************************
 * les fonctions des processus de gestion d'un processeur:
 * File descripteur 0: stdin
 * File descripteur 1: stdout
 * File descripteur 2: stderr
 * File descripteur FD_FROM_FPGA: lecture du FPGA
 * File descripteur FD_TO_FPGA:   ecriture sur le FPGA
 *
 * Les processus partagent le meme FD_FROM_FPGA mais ont des
 * FD_TO_FPGA différents.
**/
typedef void (*Tmdm_func)();

/*******************************************************************
 * la definition d'un driver d'un processus.
**/
typedef struct _Tmdm_param {
    char* name;            /* nom du processeur */
    int addrprocessor;     /* adresse du processeur */
    int samewindow;        /* O:oui ; 1:non
                            * il n'a d'effet que si il y a 2 processus (wronly!=0),
                            * dans ce cas
                            *   1 indique qu'ils ont le meme stdin, stdout et stderr.
                            *   0 indique qu'ils ont des stdin, stdout et stderr
                            *     différents. */
    Tmdm_func rdwr,wronly; /* rdwr=fonction du 1er processus (obligatoire)
                            * wronly=fonction du 2nd processus ou null */

    char     *rdwr_geom,   // eg: (char*)0    window de DL lignes de DC colonnes en (DX,DY)
             *wronly_geom; /*     10          window de DL lignes de 10 colonnes en (DX,DY)
                            *     10x11       window de 11 lignes de 10 colonnes en (DX,DY)
                            *     +20         window de DL lignes de DC colonnes en (20,DY)
                            *     +20+21      window de DL lignes de DC colonnes en (20,21)
                            *     10+20       window de DL lignes de 10 colonnes en (20,DY)
                            *     10x11+20+21 window de 11 lignes de 10 colonnes en (20,21)
                            * DL et DC en caractere, DX,DY en points */
    char     *rdwr_xargs;  // arguments to transmit to rdwr xterm
    char     *wronly_xargs;// arguments to transmit to wronly xterm
} Tmdm_param;

#endif
