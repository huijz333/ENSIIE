#!/bin/bash
#
#usage: punition1.sh n word

if test $# != 2 ; then
  echo 1>&2 "$0:FATAL: $# invalid argument number (expected 2)"
  exit 1
fi

n=$1
word=$2

i=0
while test $i -lt $n ; do
  echo $word
  i=$[$i+1]
done

