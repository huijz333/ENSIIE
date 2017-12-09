# ########################################## #

######  #     #   ###     #       #     ######                                  
#       ##    #  #   #                  #                                       
#       # #   #   #      ##      ##     #                                       
#####   #  #  #    #      #       #     #####                                   
#       #   # #     #     #       #     #                                       
#       #    ##  #   #    #       #     #                                       
######  #     #   ###   #####   #####   ######      

# ########################################## #

# Makefile minimaliste pour le TP IPI sur la manipulation d’images
# Pierre Tellier, ENSiiE, décembre 2017

APPLI=tp_image
CSRC= main.c effets-photom.c   pnm.c effets-geom.c filtrages.c
CC = gcc

CFLAGS = -Wall -Wextra -ansi -I. 

COBJ=$(CSRC:.c=.o)


.c.o:
	$(CC) $(CFLAGS) -c $*.c

$(APPLI):	$(COBJ)
	$(CC) -o $(APPLI) $(COBJ) -lm

clean:
	-rm *.o *[~%] core *.bak


# DO NOT DELETE

effets-geom.o: effets-geom.h
effets-photom.o: effets-photom.h
filtrages.o: filtrages.h
main.o: pnm.h effets-photom.h effets-geom.h filtrages.h
pnm.o: pnm.h
