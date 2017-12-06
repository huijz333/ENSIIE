/*****************************************************************************/
/*                                                                           */
/* lecture et ecriture de fichiers pnm                                       */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>


#include "pnm.h"


static unsigned char masks[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

void lireheader(PnmFile *fich)
{
    char str[4096];
    
    fgets(str,4096,fich->pnmfile);
    while (str[0] == '#')
        fgets(str,4096,fich->pnmfile);
    sscanf(str,"%s",fich->header);
    if (fich->header[0] == 'P')
        sscanf(&fich->header[1],"%d",&fich->type);
    else{
        fprintf(stderr,"format du fichier %s inconnu\n",fich->name);
        exit(-1);
    }
}

void liretaille(PnmFile *fich)
{
    char str[4096];
    
    fgets(str,4096,fich->pnmfile);
    while (str[0] == '#')
        fgets(str,4096,fich->pnmfile);
    sscanf(str,"%d %d",&fich->xsize, &fich->ysize);
}

void liremaxval(PnmFile *fich)
{
    char str[4096];
    
    fgets(str,4096,fich->pnmfile);
    while (str[0] == '#')
        fgets(str,4096,fich->pnmfile);
    sscanf(str,"%d",&fich->maxvalue);
}

void lirerawbitmap(PnmFile *fich, int linesize)
{
    int  i,j,adr,ls,thebyte;
    unsigned char *ligne;
    
    fich->bitmap = (unsigned char *)calloc(fich->ysize * linesize,1);
    if (fich->type==4) {
        adr = 0;
        ls = ((fich->xsize - 1) / 8) + 1;
        ligne = (unsigned char *)calloc(ls, 1);
        for (i=0;i<fich->ysize;i++) {
            fread(ligne, ls, 1, fich->pnmfile);
            for (j=0;j<fich->xsize;j++) {
                thebyte = (int)(j / 8);
                if (ligne[thebyte]&masks[j%8])
                    fich->bitmap[adr]=0;
                else
                    fich->bitmap[adr]=255;
                adr++;
            }
        }
        free(ligne);
    }
    else
        fread(fich->bitmap, sizeof(unsigned char), fich->ysize * linesize, fich->pnmfile);
}

void lireasciibitmap(PnmFile *fich, int linesize)
{
    int  i, j, k, thebit;
    unsigned char val;
    int adr;
    
    fich->bitmap = (unsigned char *)calloc(fich->ysize * linesize,1);
    if (fich->type == 1) {
        adr = 0;
        for (i=0;i<fich->ysize;i++) {
            for (j=0;j<linesize;j++) {
                val = 0;
                for (k=0;k<8;k++) {
                    fscanf(fich->pnmfile,"%d",&thebit);
                    if (thebit) val |= masks[k];
                }
                fich->bitmap[adr] = val;
                adr ++;
            }
        }
    }
    else {
        adr = 0;
        for (i=0;i<fich->ysize;i++) {
            for (j=0;j<linesize;j++) {
                fscanf(fich->pnmfile,"%c",&fich->bitmap[adr]);
                adr++;
            }
        }
    }
}

void lirebitmap(PnmFile *fich)
{
    unsigned int linesize;
    
    switch (fich->type) {
        case 1:
            linesize = ((fich->xsize - 1) / 8) + 1;
            linesize = fich->xsize;
            lireasciibitmap(fich,linesize);
            break;
        case 2:
            linesize = fich->xsize;
            lireasciibitmap(fich,linesize);
            break;
        case 3:
            linesize = 3 * fich->xsize;
            lireasciibitmap(fich,linesize);
            break;
        case 4:
            linesize = ((fich->xsize - 1) / 8) + 1;
            linesize = fich->xsize;
            lirerawbitmap(fich,linesize);
            break;
        case 5:
            linesize = fich->xsize;
            lirerawbitmap(fich,linesize);
            break;
        case 6:
            linesize = fich->xsize * 3;
            lirerawbitmap(fich,linesize);
            break;
        default:
            fprintf(stderr,"format inconnu\n");
            exit(-1);
    }
}

void ouvrirlec(PnmFile *fich)
{
    extern FILE     *fopen();
    
    fich->pnmfile = fopen(fich->name,"rb");
    if (fich->pnmfile == NULL) {
        fprintf(stderr,"Impossible d'ouvrir : %s en lecture\n",fich->name);
        exit(-1);
    }
}

void lirefich(PnmFile *fich)
{
    ouvrirlec(fich);
    lireheader(fich);
    liretaille(fich);
    if ((fich->type != 1) && (fich->type != 4)) liremaxval(fich);
    lirebitmap(fich);
}

void fermerpnm(PnmFile *fich)
{
    free(fich->bitmap);
    fclose(fich->pnmfile);
}

void ecrirerawbitmap(PnmFile *fich, int linesize)
{
    fwrite(fich->bitmap, sizeof(unsigned char), linesize*fich->ysize, fich->pnmfile);
}

void ecrireasciibitmap(PnmFile *fich, int linesize)
{
    int  i, j, thebyte,thepixel;
    int adr;
    
    if (fich->type == 1) {
        adr = 0;
        for (i=0;i<fich->ysize;i++) {
            for (j=0;j<fich->xsize;j++) {
                thepixel = (i * fich->xsize) + j;
                thebyte = (int)(thepixel / 8);
                if (fich->bitmap[thebyte]&masks[thepixel%8])
                    fprintf(fich->pnmfile,"1 ");
                else fprintf(fich->pnmfile,"0 ");
            }
            fprintf(fich->pnmfile,"\n");
        }
    }
    else {
        adr = 0;
        for (i=0;i<fich->ysize;i++) {
            for (j=0;j<linesize;j++) {
                fprintf(fich->pnmfile,"%d ",fich->bitmap[adr]);
                adr++;
            }
            fprintf(fich->pnmfile,"\n");
        }
    }
}

void ecrirebitmap(PnmFile *fich)
{
    unsigned int linesize;
    
    switch (fich->type) {
        case 1:
            linesize = ((fich->xsize - 1) / 8) + 1;
            ecrireasciibitmap(fich,linesize);
            break;
        case 2:
            linesize = fich->xsize;
            ecrireasciibitmap(fich,linesize);
            break;
        case 3:
            linesize = 3 * fich->xsize;
            ecrireasciibitmap(fich,linesize);
            break;
        case 4:
            linesize = ((fich->xsize - 1) / 8) + 1;
            ecrirerawbitmap(fich,linesize);
            break;
        case 5:
            linesize = fich->xsize;
            ecrirerawbitmap(fich,linesize);
            break;
        case 6:
            linesize = fich->xsize * 3;
            ecrirerawbitmap(fich,linesize);
            break;
         default:
            fprintf(stderr,"format inconnu\n");
            exit(-1);
    }
}

void alloue_bitmap(PnmFile *fich)
{
    unsigned int linesize;
    
    switch (fich->type)
    {
        case 1:
            linesize = ((fich->xsize - 1) / 8) + 1;
            linesize = fich->xsize;
            break;
        case 2:
            linesize = fich->xsize;
            break;
        case 3:
            linesize = 3 * fich->xsize;
            break;
        case 4:
            linesize = ((fich->xsize - 1) / 8) + 1;
            linesize = fich->xsize;
            break;
        case 5:
            linesize = fich->xsize;
            break;
        case 6:
            linesize = fich->xsize * 3;
            break;
        default:
            fprintf(stderr,"format inconnu\n");
            exit(-1);
    }
    fich->bitmap = (unsigned char *)calloc(fich->ysize * linesize,1);
    if (fich->bitmap == NULL) {
        fprintf(stderr,"Impossible d'allouer bitmap\n");
        exit(-1);
    }
}

void ouvrirecr(PnmFile *fich)
{
    sprintf(fich->header,"P%d",fich->type);
    fich->pnmfile = fopen(fich->name,"wb");
    if (fich->pnmfile == NULL)
    {
        fprintf(stderr,"Impossible d'ouvrir : %s en ecriture\n",fich->name);
        exit(-1);
    }
    alloue_bitmap(fich);
}

void ecrirefich(PnmFile *fich)
{
    fprintf(fich->pnmfile,"%s\n",fich->header);
    fprintf(fich->pnmfile,"# created using pnm.c\n");
    fprintf(fich->pnmfile,"%d %d\n",fich->xsize,fich->ysize);
    if ((fich->type != 1) && (fich->type != 4))
        fprintf(fich->pnmfile,"%d\n",fich->maxvalue);
    ecrirebitmap(fich);
}


void sauverfich(PnmFile *fich)
{
    fich->pnmfile = fopen(fich->name,"wb");
    if (fich->pnmfile == NULL)
    {
        fprintf(stderr,"Impossible d'ouvrir : %s en ecriture\n",fich->name);
        exit(-1);
    }
    sprintf(fich->header,"P%d",fich->type);
    ecrirefich(fich);
    fermerpnm(fich);
}





