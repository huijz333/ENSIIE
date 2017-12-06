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
#include <string.h>

#include "effets-photom.h"

/*!
 \file effets-photom.c
 \author
 \date
 \brief module des fonctions de correction de la couleur et de la luminosité des images
 */

/*!
 \fn unsigned char *inversion_video(unsigned char *img, int xs, int ys, int *nc, int *nl)
 \brief transforme l'image en "négatif"
 \param img : adresse du bitmap
 \param xs : largeur de l'image
 \param ys : hauteur de l'image
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \return l'image obtenue
 */

unsigned char * inversion_videoNB(unsigned char *img, int xs, int ys, int *nc, int *nl)
{
    unsigned char *res=NULL;
    int i;
    
    res = calloc(xs*ys,1);
    for(i=0;i<xs*ys;i=i+1)
        res[i]=255-img[i];
    
    *nc=xs;
    *nl=ys;
    return res;
}

/*!
 \fn unsigned char *mixageNB(unsigned char *img1,int xs1,int ys1,unsigned char *img2,int xs2,int ys2,int *nc,int *nl)
 \brief calcule une image en faisant la moyenne de 2 images, pixel par pixel
 \param img1 : adresse du bitmap 1
 \param xs1 : largeur de l'image 1
 \param ys1 : hauteur de l'image 1
 \param img2 : adresse du bitmap 2
 \param xs2 : largeur de l'image 2
 \param ys2 : hauteur de l'image 2
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \return l'image obtenue
 */

unsigned char * mixageNB(unsigned char *img1,int xs1,int ys1,unsigned char *img2,int xs2,int ys2,int *nc,int *nl)
{
    unsigned char *res=NULL;
    
    /* ajouter votre code ici */
    
    *nl=ys1;
    *nc=xs1;
    return res;
}


/*!
 \fn unsigned char *plusClair(unsigned char *img, int xs, int ys, int *nc, int *nl)
 \brief rend l'image plus claire en augmentant les niveaux de 5%. Les niveaux sont tronqués à 255.
 \param img : adresse du bitmap
 \param xs : largeur de l'image
 \param ys : hauteur de l'image
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \return l'image obtenue
 */

unsigned char * plusClairNB(unsigned char *img, int xs, int ys, int *nc, int *nl)
{
	unsigned char * res = (unsigned char *) malloc(xs * ys);
	int nbPixels = xs * ys;
	int i;
	for (i = 0 ; i < nbPixels ; i++) {
		unsigned int v = img[i] + img[i] / 2;
		if (v > 255) {
			v = 255;
		}
		res[i] = v;
	}
	*nc = xs; 
	*nl = ys;
	return res;
}


/*!
 \fn unsigned char *bruiteNB(unsigned char *img, int xs, int ys, double proportion int *nc, int *nl)
 \brief rend l'image plus claire en augmentant les niveaux de 5%. Les niveaux sont tronqués à 255.
 \param img : adresse du bitmap
 \param xs : largeur de l'image
 \param ys : hauteur de l'image
 \param proportion : part des pixels afféctés par le bruit (entre 0 et 1)
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \return l'image obtenue
 */

unsigned char * bruiteNB(unsigned char *img, int xs, int ys, double proportion, int *nc, int *nl)
{
    unsigned char *res=NULL;
    int i,adr;
    
    res = calloc(xs*ys,1);
    for(i=0;i<xs*ys*proportion;i=i+1) {
        adr=random()%(xs*ys);
        if (res[adr]<128) res[adr]=255;
        else res[adr]=0;
    }
    *nc=xs;
    *nl=ys;
    return res;
}




