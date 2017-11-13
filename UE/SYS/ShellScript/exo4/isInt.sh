#!/bin/bash

function isInt() {
	n="$1"
	m=$(echo "$n" | sed -e "1s/[+-]\?[0-9]\+//")
	! test $m
}
