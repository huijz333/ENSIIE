#!/bin/bash

if test $# -ne 1 -o ! -d $1; then
	echo "usage: $0 [DIRECTORY]" >&2
	exit 1
fi

find $1 -type f -exec stat -c %U {} \; | sort | uniq 
