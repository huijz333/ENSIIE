// fichier: data.h
// auteur:  jean Fleur

typedef struct _Tdata {
    int x,y;
    char data[100];
} Tdata;
Tdata* dt_create(char *name); // utilise strcmp
