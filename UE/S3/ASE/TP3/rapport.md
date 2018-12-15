# Rapport du TP File system (FUSE)

#### Etudiant: Romain PEREIRA
#### Encadrant: Jacques ‐ Charles LAFOUCRIERE

1) 2) préparation de la vm

3) Pour compiler le programme:
```shell
gcc -Wall ASE-fuse.c -o ASE-fuse -D_FILE_OFFSET_BITS=64 -lfuse
```

4)
```shell
mkdir /mnt/ASE
./ASE-fuse /mnt/ASE
```

5)
```shell
> ls -l /mnt/ASE
total 4
drwxr-xr-x 2 root    0 janv.  1  1970 .
drwxr-xr-x 4 root 4096 déc.  15 17:36 ..
-r--r--r-- 1 root   10 déc.  15 17:36 test1
-r--r--r-- 1 root   10 déc.  15 17:36 test2
```

6) 
```shell
> cat /mnt/ASE/test
ABCDEFGHIJ
```

7) voir code source ASE-fuse.c
