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
