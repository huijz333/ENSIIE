gcc lire.c -o lire
gcc lire3exec.c -o lire3exec
gcc lire3a.c -o lire3a
gcc lire3b.c -o lire3b
gcc lire3c.c -o lire3c
./lire3exec < /pub/ia/sys/syscall/data.in > data.out
./lire3a
./lire3a I_DONT_EXIST NEITHER
./lire3a I_EXIST NOT_ME
./lire3a data.in data_a.out
./lire3b data.in data_b.out
./lire3c data.in data_c.out

diff data.out .data.out
diff data_a.out .data_a.out
diff data_b.out .data_b.out
diff data_c.out .data_c.out

rm lire
rm lire3exec
rm lire3a
rm lire3b
rm lire3c
rm data.out
rm data_a.out
rm data_b.out
rm data_c.out
