#!/bin/bash
#
#usage: punition1.sh n word

if test $# -eq 0 ; then
	punition3.sh 10 3 "je ne bavarde pas en cours"
elif test $# -eq 1 ; then
	punition3.sh 10 3 $1
elif test $# -eq 2 ; then
	punition3.sh 10 $1 $2
else
	punition3.sh $1 $2 $3
fi
