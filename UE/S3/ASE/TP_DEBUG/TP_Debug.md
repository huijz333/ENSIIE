**1.1** (voir top.txt)
 ```
top > top.txt
```

**1.2** (voir strace_ps.txt)
 ```
strace -e open ps 2> strace_ps.txt```
```
'ps' ouvre les fichiers:
   - dossier de chaque processus ('/proc/*')
   - '/proc/meminfo'
   - des bibliothèques dynamiques ('/lib64')
   - '/proc/tty/drivers'
