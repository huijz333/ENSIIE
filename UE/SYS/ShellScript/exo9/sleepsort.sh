#!/bin/sh

function sleepsort() {
	pids=()
	i=0
	ns=$(echo $@ | tr " " "\n" | sort -n)
	for n in $ns ; do
		( sleep $n ; echo "slept for $n seconds" ) &
		pids[$i]=$!
		i=$[$i+1]
	done
	
	for i in $(seq 0 $[$#-1])  ; do
		wait ${pids[$i]}
		i=$[$i+1]
	done
}
