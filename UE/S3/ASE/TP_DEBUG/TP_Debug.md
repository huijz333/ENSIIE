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
