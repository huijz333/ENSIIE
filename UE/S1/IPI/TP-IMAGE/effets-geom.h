#ifndef __EFFETS_GEOM_H
#define __EFFETS_GEOM_H

unsigned char * effetFonteNB(unsigned char *img, int xs, int ys, int *nc, int *nl);
unsigned char * quartImageNB(unsigned char *img, int xs, int ys, int *nc, int *nl);
unsigned char * deriv1xNB(unsigned char *img, int xs, int ys, int *nc, int *nl);
unsigned char * demiImageNB(unsigned char *img, int xs, int ys, int *nc, int *nl);

#endif
