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
