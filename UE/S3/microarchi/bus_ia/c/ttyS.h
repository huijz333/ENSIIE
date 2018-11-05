/*#########################################################################
# $Software: busiac
# $section : rs232-mux (engine)
#       $Id: ttyS.h 320 2015-05-29 05:21:22Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/ttyS.h $
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

#ifndef FILE_TTYS_H
#define FILE_TTYS_H

extern int  ttyS_open(const char* base, int flag, int baud);
extern int  ttyS_read(int vt100);
extern void ttyS_write(int vt100, unsigned char x);

#endif /* FILE_TTYS_H */
