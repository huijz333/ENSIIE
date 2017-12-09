/*
 
 ######  #     #   ###     #       #     ######
 #       ##    #  #   #                  #
 #       # #   #   #      ##      ##     #
 #####   #  #  #    #      #       #     #####
 #       #   # #     #     #       #     #
 #       #    ##  #   #    #       #     #
 ######  #     #   ###   #####   #####   ######    2017-2018
 
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "pnm.h"
#include "effets-photom.h"
#include "effets-geom.h"
#include "filtrages.h"

/*!
 \file main.c
 \author
 \date 
 \brief programme principal : test des fonctionnalités de manipulation d'image
 */


int main(int argc, char **argv)
{
	int xsize, /*! largeur de l'image origine */
	    ysize,     /*! hauteur de l'image origine */
	    xsize2,    /*! largeur de l'image résultat */
	    ysize2;    /*! hauteur de l'image résultat */

	PnmFile fich;  /*! "fichier" image : nom, type, taille, bitmap */
	PnmFile fich2; /*! "fichier" image : nom, type, taille, bitmap */
	unsigned char * res=NULL;

	if (argc<3) {
		fprintf(stderr,"usage : %s fichier.p[b|g|p]m fichier.p[b|g|p]m\n",argv[0]);
		exit(1);
	}
	srand(time(NULL)); /* initialisation du générateur pseudo-aléatoire */

	fich.name=argv[1];
	lirefich(&fich);
	fprintf(stderr,"Lecture OK (H=%d L=%d)\n",fich.ysize,fich.xsize);
	xsize=fich.xsize;
	ysize=fich.ysize;
	if ((fich.type!=2)&&(fich.type!=5)) exit(1);

	/*res = demiImageNB(fich.bitmap, xsize, ysize, &xsize2, &ysize2);*/
	/*res = plusClairNB(fich.bitmap, xsize, ysize, &xsize2, &ysize2);*/
	/*res = deriv1xNB(fich.bitmap, xsize, ysize, &xsize2, &ysize2);*/
	/*res = effetFonteNB(fich.bitmap, xsize, ysize, &xsize2, &ysize2);*/
	/*res = quartImageNB(fich.bitmap, xsize, ysize, &xsize2, &ysize2);*/
	/*res = filtrerImageNB(fich.bitmap, xsize, ysize, 5, &xsize2, &ysize2);*/
	res = filtrerImageNB(fich.bitmap, xsize, ysize, 15, &xsize2, &ysize2);
	
	fich2.bitmap=res;
	fich2.xsize=xsize2;
	fich2.ysize=ysize2;
	fich2.maxvalue=255;
	fich2.name=argv[2];
	fich2.type=5;
	sauverfich(&fich2);

	return 0;
}
