#ifndef __EFFETS_COULEUR_H
#define __EFFETS_COULEUR_H

unsigned char * inversion_videoNB(unsigned char *img, int xs, int ys, int *nc, int *nl);
unsigned char * plusClairNB(unsigned char *img, int xs, int ys, int *nc, int *nl);
unsigned char * mixageNB(unsigned char *img1,int xs1,int ys1,unsigned char *img2,int xs2,int ys2,int *nc,int *nl);
unsigned char * fishEyeNB(unsigned char *img,int xs,int ys,int *nc,int *nl);
unsigned char * bruiteNB(unsigned char *img, int xs, int ys, double proportion, int *nc, int *nl);

#endif
