tar -cf burger.tar `find /pub/ia/sys/shell/projet/ -type f -name *.c -exec grep -q "Patrice Burger" {} \; -print | tr '\n' ' '`
