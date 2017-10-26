#!/bin/zsh

if test "$#" -ne "2" ; then
	err="error: error usage: $0 file1 file2"
elif ! (test -e $1 -a -e $2) ; then
	err="error: $1 or $2 doesnt exist"
elif test -d $1 -o -d $2 ; then
	err="$1 or $2 is a directory."
else
	n=`cat $1 | cut -d ' ' -f 1`
	if test `wc -l < $2` -ge $n ; then
		m=`cat $1 | cut -d ' ' -f 2`
		echo $[$m * (`cat $2 | tr "\n" "+" | rev | cut -c2- | rev`) / $n]
		exit 0
	fi
	err="$2 is too short"
fi

echo $err >&2
exit 1


