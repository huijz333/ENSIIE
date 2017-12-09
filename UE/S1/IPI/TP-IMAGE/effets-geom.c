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
#include <math.h>

#include "effets-geom.h"

/*!
 \file effets-geom.c
 \author 
 \date 6 décembre 2017
 \brief module contenant les fonctions de traitement géométrique des images
 */



/*!
 \fn void effetFonteNB(unsigned char *img, int xs, int ys, int *nc, int *nl)
 \brief Principe : choisir aléatoirement (xs*ys) pixels (certains seront choisis plusieurs fois, d'autres 0)
 \brief si le pixel en dessous est plus clair, alors le remplacer par le pixel choisi \param img : adresse du bitmap
 \param xs : largeur de l'image
 \param ys : hauteur de l'image
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \brief transforme l'image pour donner l'illusion que les parties sombres "fondent"
 \return l'image obtenue

 */


unsigned char * effetFonteNB(unsigned char *img, int xs, int ys, int *nc, int *nl)
{
	unsigned char * res = (unsigned char *) malloc(xs * ys);
	int N = xs * ys;
	memcpy(res, img, N);
	int i;
	for (i = 0 ; i < N ; i++) {
		int x = rand() % xs;
		int y = rand() % (ys - 2) + 1; /* on s'assure que y est dans ]0, ys - 1[ */
		int index = y * xs + x;
		if (img[index] < img[(y + 1) * xs + x]) {
			res[index] = img[(y - 1) * xs + x];
		}
	}
	*nc = xs; 
	*nl = ys; 
    return res;
}


/*!
 \fn unsigned char * quartImageNB(unsigned char *img, int xs, int ys, int *nc, int *nl)
 \brief Crée une image dehauteur ys/2 et de largeur ys/2 (en prenant un pixel sur 2)
 \param img : adresse du bitmap
 \param xs : largeur de l'image
 \param ys : hauteur de l'image
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \return l'image obtenue
 */


unsigned char * quartImageNB(unsigned char *img, int xs, int ys, int *nc, int *nl)
{
    unsigned char * res = (unsigned char *) malloc(xs * ys);
    int x, y;
    int xs2 = xs / 2;
    int ys2 = ys / 2;
    for (y = 0 ; y < ys2 ; y++) {
    	for (x = 0 ; x < xs2 ; x++) {
		int i = y * xs2 + x;
		int j = y * 2 * xs + x * 2;
		res[i] = img[j];
	}
    }
   
    *nl=ys2;
    *nc=xs2;
    return res;
}



/*!
 \fn unsigned char *deriv1x(unsigned char *img, int xs, int ys, int *nc, int *nl)
 \brief remplace chaque pixel à partir de la 2ème colonne par sa différence avec son voisin de gauche
 \brief les valeurs <= -128 sont ramenées à 0, celles >=127 à 255.
 \brief si image couleur traitement de chaque composante.
 \param img : adresse du bitmap
 \param xs : largeur de l'image
 \param ys : hauteur de l'image
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \return l'image obtenue
 */


unsigned char * deriv1xNB(unsigned char *img, int xs, int ys, int *nc, int *nl)
{
	unsigned char * res = (unsigned char *) malloc(xs * ys);
	memcpy(res, img, xs * ys);
	int i, j;
	for (i = 0 ; i < ys ; i++) {
		for (j = xs - 1 ; j > 0 ; j--) {
			int index = i * xs + j;
			int left  = img[index - 1];
			int right = img[index];
			int d = right - left;
			res[index] = d < -128 ? -128 : d > 127 ? 127 : d;
		}
	}
	*nc = xs; 
	*nl = ys;   
	return res;
}



/*!
 \fn unsigned char *fishEyeNB(unsigned char *img, int xs, int ys, int *nc, int *nl)
 \brief réalise un effet "fish eye" sur une image :
 \brief calcule les coordonnées polaires angle rayon de chaque pixel par rapport au centre de l'image
 \brief et modifie le rayon. Pour éviter les pixels manquants, réalise la fonction inverse.
 \param img : adresse du bitmap
 \param xs : largeur de l'image
 \param ys : hauteur de l'image
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \return l'image obtenue
 */


unsigned char * fishEyeNB(unsigned char *img, int xs, int ys, int *nc, int *nl) {
    unsigned char *res=NULL;
    
    int i,j;
    double r,R,a,x,y,rr;
    
    res=calloc(xs*ys, sizeof(unsigned char));
    R=sqrt((double)((ys/2)*(ys/2)));
    for(i=0;i<ys;i++)
        for(j=0;j<xs;j++) {
            r = sqrt((double)((i-ys/2)*(i-ys/2) +(j-xs/2)*(j-xs/2)));
            a = atan2((double)(i-ys/2),(double)(j-xs/2));
            rr = r*r/R;
            x = (double)xs*0.5 + rr*cos(a);
            y = (double)ys*0.5 + rr*sin(a);
            if (y<0) y=0; else if (y>ys-1) y=ys-1;
            if (x<0) x=0; else if (x>xs-1) x=xs-1;
            if (r<R)
                res[i*xs+j]=img[(int)y*xs+(int)x];
        }
    *nl=ys;
    *nc=xs;
    return res;
}

/*!
 \fn unsigned char * quartImageNB(unsigned char *img, int xs, int ys, int *nc, int *nl)
 \brief Crée une image de hauteur ys/2 et de largeur xs (en prenant une ligne sur 2) 
 \param img : adresse du bitmap
 \param xs : largeur de l'image
 \param ys : hauteur de l'image
 \param nc : pointeur sur le nombre de colonnes (modifiable) de l'image résultat
 \param nl : pointeur sur le nombre de lignes (modifiable) de l'image résultat
 \return l'image obtenue
 */


unsigned char * demiImageNB(unsigned char *img, int xs, int ys, int *nc, int *nl)
{
    unsigned char * res = NULL;
    int i,i2,j, adr1, adr2=0;
    res=calloc(xs*(ys/2), sizeof(unsigned char));
    for(i=0;i<ys;i+=2)
        for(j=0;j<xs;j++) {
            adr1 = i*xs+j;
            res[adr2]=img[adr1];
            adr2 += 1;
        }
    
    *nl=ys/2;
    *nc=xs;
    return res;
}


