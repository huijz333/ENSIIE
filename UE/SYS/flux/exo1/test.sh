
gcc -Wall -Werror -Wextra suplastbyte.c

./a.out testfile

echo "ABC" > testfile

chmod 000 testfile
./a.out testfile

chmod 666 testfile
./a.out testfile
cat testfile

./a.out testfile
cat testfile

echo ""
echo "done"

rm testfile
rm a.out
