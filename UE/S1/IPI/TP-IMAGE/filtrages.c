/*
 
 ######  #     #   ###     #       #     ######
 #       ##    #  #   #                  #
 #       # #   #   #      ##      ##     #
 #####   #  #  #    #      #       #     #####
 #       #   # #     #     #       #     #
 #       #    ##  #   #    #       #     #
 ######  #     #   ###   #####   #####   ######    2017-2018
 
 */


#include <stdlib.h>
#include <string.h>
#include "filtrages.h"

/*!
 \file filtrages.c
 \author 
 \date 6 décembre 2017
 \brief module contenant les fonctions de corrections des défauts d'image, filtrages, convolutions etc.
 */



/*!
 \fn unsigned char * filtrerMedianImageNB(unsigned char *img,int xs,int ys,int tailleFiltre,int *nc,int *nl)
 \brief applique le filtrage médian à tous les pixels de l'image (suffisamment éloignés des bords)
 \param img : adresse du bitmap
 \param xs : largeur de l'image
 \param ys : hauteur de l'image
 \param tailleFiltre : taille de la fenêtre de filtrage (doit être impaire, valeur conseillée 5)
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \return l'image obtenue
 */

unsigned char * filtrerMedianImageNB(unsigned char *img, int xs, int ys, int tailleFiltre,int *nc,int *nl)
{
    unsigned char *res=NULL;
    
    /* ajouter votre code ici */
    
    *nl=ys;
    *nc=xs;
    return res;
}

/*!
 \fn unsigned char * filtrerImageNB(unsigned char *img,int xs,int ys,int tailleFiltre,int *nc,int *nl)
 \brief applique le filtrage moyenne à tous les pixels de l'image (suffisamment éloignés des bords)
 \brief à améliorer en passant le filtre en paramètre
 \param img : adresse du bitmap
 \param xs : largeur de l'image
 \param ys : hauteur de l'image
 \param tailleFiltre : taille de la fenêtre de filtrage (doit être impaire, valeur conseillée 5)
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \return l'image obtenue
 */
unsigned char * filtrerImageNB(unsigned char *img, int xs, int ys, int tailleFiltre,int *nc,int *nl)
{
	unsigned char *res = (unsigned char *) malloc(xs * ys);
	int x, y;
	int pxCount = tailleFiltre * tailleFiltre;
	int depth = tailleFiltre / 2; /* 5 / 2 == 2 */
	memcpy(res, img, xs * ys);
	for (x = depth ; x < xs - depth ; x++) {
		for (y = depth ; y < ys - depth ; y++) {
			unsigned int sum = 0;
			int dx, dy;
			for (dy = -depth; dy <= depth ; dy++) {
				for (dx = -depth ; dx <= depth ; dx++) {
					sum += img[(y + dy) * xs + (x + dx)];
				}
			}
			res[y * xs + x] = sum / pxCount;
		}
	}

	*nl=ys;
	*nc=xs;
	return res;
}


