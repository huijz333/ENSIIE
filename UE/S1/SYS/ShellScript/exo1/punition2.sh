#!/bin/bash
#
#usage: punition1.sh n word

if test $# != 3 ; then
	echo 1>&2 "$0:FATAL: $# invalid argument number (expected 3)"
	exit 1
fi

n=$1
m=$2
word=$3

for i in `seq 1 1 $n` ; do
	for j in `seq 1 1 $m` ; do
		echo -n $word ' '
	done
	echo ""
done

