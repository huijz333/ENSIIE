/*#########################################################################
# $Software: busiac
# $section : rs232-mux (driver)
#       $Id: pretty_input.h 323 2015-05-29 09:36:23Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/pretty_input.h $
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

#if 0
#include <readline/readline.h>
#include <readline/history.h>
#else
extern int   read_history(const char*);
extern void  add_history(const char*);
extern int   append_history(int,const char*);
extern int   history_truncate_file(const char*,int);
extern char* readline(const char*);
#endif

#if 1
// copie/pasted from GNU manual
/* A static variable for holding the line. */
static char *line_read = (char *)NULL;

/* Read a string, and return a pointer to it.  Returns NULL on EOF. */
static char * rl_gets (char* prompt)
{
    static int first=0;
    if ( first==0 ) {
        first=1;
        system("touch " HISPTORY_FILE);
        read_history(HISPTORY_FILE);
    }

  /* If the buffer has already been allocated,
     return the memory to the free pool. */
  if (line_read)
    {
      free (line_read);
      line_read = (char *)NULL;
    }

  /* Get a line from the user. */
  line_read = readline (prompt);

  /* If the line has any text in it, save it on the history. */
  if (line_read && *line_read) {
    add_history (line_read);
    append_history(1,HISPTORY_FILE);
    history_truncate_file(HISPTORY_FILE,100);
  }

  return line_read;
}
#endif

