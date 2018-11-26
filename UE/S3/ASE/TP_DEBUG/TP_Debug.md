**1.1** (voir top.txt)
 ```
top
```

**1.2** (voir strace_1_2.txt)
 ```
strace -e open ps```
```
'ps' ouvre les fichiers:
   - dossier de chaque processus ('/proc/*')
   - '/proc/meminfo'
   - des bibliothèques dynamiques ('/lib64')
   - '/proc/tty/drivers'

**1.3** (voir 'strace_1_3.txt')
```
strace -c ps -elf
```
**1.4**

(voir 'strace_1_4.txt')
```
strace -c lsof
```

Liste des fichiers ouverts
```
[romain.pereira@vm0 tp]$ strace -e open lsof
open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
open("/lib64/libselinux.so.1", O_RDONLY|O_CLOEXEC) = 3
open("/lib64/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
open("/lib64/libpcre.so.1", O_RDONLY|O_CLOEXEC) = 3
open("/lib64/libdl.so.2", O_RDONLY|O_CLOEXEC) = 3
open("/lib64/libpthread.so.0", O_RDONLY|O_CLOEXEC) = 3
open("/dev/null", O_RDWR)               = 3
open("/usr/lib/locale/locale-archive", O_RDONLY|O_CLOEXEC) = 3
open("/", O_RDONLY)                     = 3
open("/proc/1601/fdinfo/3", O_RDONLY)   = 4
open("/proc/mounts", O_RDONLY)          = 3
--- SIGALRM {si_signo=SIGALRM, si_code=SI_KERNEL} ---
```
Les '.so' correspondent aux bibliothèques dynamiques chargées par le programme

Pour lister et compter les appels aux bibliothèques externes:
```
ltrace -c lsof -e /mnt
```
Ce qui donne
```
------ ----------- ----------- --------- --------------------
 69.81   25.964421    12982210         2 __libc_start_main
  8.24    3.064955         100     30379 _IO_putc
  6.62    2.462183          99     24744 __ctype_b_loc
  6.15    2.287891          99     23026 mblen
  1.38    0.512824          99      5160 strlen
  1.26    0.469682         100      4662 __printf_chk
  1.02    0.378582          99      3789 snprintf
  0.87    0.323133         464       696 read
  0.64    0.237606         100      2358 __snprintf_chk
  0.53    0.195386         210       928 write
  0.49    0.183180          99      1846 malloc
  0.32    0.120258         117      1027 close
  0.31    0.114058          99      1152 strncpy
  0.25    0.094602          99       951 __ctype_get_mb_cur_max
  0.23    0.083991          99       843 strtol
  0.19    0.072408         100       724 __errno_location
  0.19    0.071107         105       672 readdir64
  0.17    0.061831         110       559 fgets
  0.14    0.053802         115       466 alarm
  0.14    0.053761         115       466 signal
  0.14    0.052886         129       408 readlink
  0.13    0.049429         100       492 strerror
  0.11    0.042654         151       282 fopen64
  0.08    0.029105         120       241 opendir
  0.08    0.027897         119       234 fclose
  0.07    0.024771          99       248 strtoull
  0.06    0.023120          99       233 _setjmp
  0.06    0.020999          99       212 setvbuf
  0.05    0.020110         179       112 __xstat64
  0.05    0.019155          99       193 strcasecmp
  0.04    0.013767         117       117 closedir
  0.04    0.013076         100       130 strrchr
  0.02    0.009074        1134         8 getpwuid
  0.02    0.008843          97        91 __strncpy_chk
  0.02    0.008818          97        90 strcmp
  0.02    0.008296          98        84 strncmp
  0.01    0.003960          99        40 strchr
  0.01    0.003090          99        31 realloc
  0.01    0.002674         102        26 strtoul
    0.00    0.000405         101         4 free
  0.00    0.000341         341         1 fork
  0.00    0.000305         305         1 exit
  0.00    0.000266         133         2 open64
  0.00    0.000259         259         1 qsort
  0.00    0.000250         125         2 pipe
  0.00    0.000230         230         1 setlocale
  0.00    0.000219         109         2 calloc
  0.00    0.000158         158         1 exit_group
  0.00    0.000140         140         1 wait
  0.00    0.000139         139         1 umask
  0.00    0.000128         128         1 getgid
  0.00    0.000118         118         1 __lxstat64
  0.00    0.000113         113         1 getpagesize
  0.00    0.000113         113         1 lseek64
  0.00    0.000112         112         1 getuid
  0.00    0.000112         112         1 geteuid
  0.00    0.000111         111         1 getdtablesize
  0.00    0.000108         108         1 getegid
  0.00    0.000104         104         1 getpid
  0.00    0.000101         101         1 is_selinux_enabled
  0.00    0.000097          97         1 strncasecmp
```

**2.1**
```
[romain.pereira@vm0 tp]$ ulimit -a
core file size          (blocks, -c) 0
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 33151
max locked memory       (kbytes, -l) 64
max memory size         (kbytes, -m) unlimited
open files                      (-n) 1024
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 8192
cpu time               (seconds, -t) unlimited
max user processes              (-u) 4096
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```
La taille de la pile des processus est réglé sur 8192 kbytes

Voici 2 programmes qui font un dépassement de pile:

```C
void main() {
    char c[8192 * 1024];
}
```


```C
void main() {
    main();
}
```

Compilation puis execution, le programme plante
```
[romain.pereira@vm0 tp]$ gcc stackoverflow.c
[romain.pereira@vm0 tp]$ ./a.out 
Erreur de segmentation
```
=> Il y a eu un dépassement de pile

*Correction:*
Dans le cas d'une récursion infini : mettre une condition sortie dans la récursion

Dans le cas d'une pile trop petite, on peut aggrandir dynamiquement la taille de la pile:
```
ulimit -s {TAILLE_EN_KBYTES}
```
