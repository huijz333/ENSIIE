#!/bin/bash
n=$#
args=("$@")
while test $n -gt 0 ; do
	max=0
	for i in `seq 1 $n` ; do
		if test "${args[$i]}" \> "${args[$max]}" ; then
			max=$i
		fi	
	done
	#echo ${args[$max]}
	n=$[$n-1]
	delete=( ${array[$max]} )
	args=( "${args[@]/$delete}" )
	echo ${args}
done
