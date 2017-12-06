#ifndef __PPM_H
#define __PPM_H

typedef struct t_pnmfile
{
    char *name;
    int type;
    char header[3];
    int xsize,ysize;
    int maxvalue;
    FILE *pnmfile;
    unsigned char *bitmap;
}
PnmFile;

extern void ecrirefich(PnmFile *fich);
extern void ouvrirecr(PnmFile *fich);
extern void fermerpnm(PnmFile *fich);
extern void lirefich(PnmFile *fich);
extern void sauverfich(PnmFile *fich);

#endif
