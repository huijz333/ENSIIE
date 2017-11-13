#!/bin/bash

if test $# -ne 3 ; then
	echo 1>&2 "usage: punition3.sh n m word"
	exit 1
fi

str=''
i=0
n=$1
m=$2
word=$3
while test $i -lt $m ; do
	str="$word $str"
	i=$[$i+1]
done

punition1.sh "$n" "$str"
