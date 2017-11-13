grep Alice /pub/ia/sys/shell/select.dat | head -n 1
echo ""
sed -e /Alice/\!d /pub/ia/sys/shell/select.dat | head -n 1
echo ""
grep Alice /pub/ia/sys/shell/select.dat | head -n 1 | cut -f 3
echo ""
sed -e /Alice/\!d -e "s+\([^\t]*\)\t[^\t]*\t\([^\t]*\)+\2+" /pub/ia/sys/shell/select.dat | head -n 1 | cut -f 3
echo ""
grep Alice /pub/ia/sys/shell/select.dat | tail -n 1 | cut -f 3
