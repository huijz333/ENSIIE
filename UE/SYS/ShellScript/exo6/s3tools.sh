
function s36str() {
	if test $# -eq 3 ; then
		s36str "$1" "$1" "$2" "$2" "$3" "$3"
		return 0
	fi

	if test ! " $1 " \> " $3 " -a ! " $3 " \> " $5 " ; then
		echo $2 ; echo $4 ; echo $6
	elif test ! " $1 " \> " $5 " -a ! " $5 " \> " $3 " ; then
		echo $2 ; echo $6 ; echo $4
	elif test ! " $3 " \> " $1 " -a ! " $1 " \> " $5 " ; then
		echo $4 ; echo $2 ; echo $6
	elif test ! " $3 " \> " $5 " -a ! " $5 " \> " $1 " ; then
		echo $4 ; echo $6 ; echo $2
	elif test ! " $5 " \> " $1 " -a ! " $1 " \> " $3 " ; then
		echo $6 ; echo $2 ; echo $4
	else
		echo $6 ; echo $4 ; echo $2
	fi
}

function isInt() {
	n="$1"
	m=$(echo "$n" | sed -e "1s/[+-]\?[0-9]\+//")
	! test $m
}

function s3s() {
	if test $# -ne 3 ; then
		echo "Fatal: wrong argument number, 3 strings expected." >&2
		return 1
	fi
	LD_LIBRARY_PATH=/pub/ia/sys/shell/iacmp/lib IacmpDir=/pub/ia/sys/shell/iacmp /pub/ia/sys/shell/iacmp/iacmp "$1" "$2" "$3"
}

function s3e() {
	if test $# -ne 3 ; then
		echo "Fatal: wrong argument number, 3 integers expected." >&2
		return 1
	fi

	if ! isInt $1 -or ! isInt $2 -or isInt $3 ; then
		echo "Fatal: one argument isnt a valid integer." >&2
	fi
	echo $@ | tr " " "\n" | sort -n
	return 0
}

function s3f() {
	if test $# -ne 3 ; then
		echo "Fatal: wrong argument number, 3 files expected." >&2
		return 1
	fi

	if ! test -f $1 -a -f $2 -a -f $2 ; then
		echo "Fatal: one argument isnt a regular file." >&2
		return 1
	fi
	s1=`stat -c %s $1`
	s2=`stat -c %s $2`
	s3=`stat -c %s $3`
	printf "$1 $s1\n$2 $s2\n$3 $s3\n" | sort -k2 -n | cut -d ' ' -f 1
	return 0
}

function s3d() {
	if test $# -ne 3 ; then
		echo "Fatal: wrong argument number, 3 files expected." >&2
		return 1
	fi

	if ! test -d $1 -a -d $2 -a -d $3 ; then
		echo "Fatal: one argument isnt a directory." >&2
		return 1
	fi
	s1=`ls -1a $1 | wc -l`
	s2=`ls -1a $2 | wc -l`
	s3=`ls -1a $3 | wc -l`
	printf "$1 $s1\n$2 $s2\n$3 $s3\n" | sort -k2 -n | cut -d ' ' -f1
	return 0
}

function s3f1m() {
	if test $# -ne 3 ; then
		echo "Fatal: wrong argument number, 3 files expected." >&2
		return 1
	fi
	w1=`cat $1 | head -n 1 | cut -d ' ' -f2`
	w2=`cat $2 | head -n 1 | cut -d ' ' -f2`
	w3=`cat $3 | head -n 1 | cut -d ' ' -f2`
	printf "$1 $w1\n$2 $w2\n$3 $w3\n" | sort -k2 | cut -d ' ' -f1
	return 0
}
