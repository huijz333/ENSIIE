
function shzip() {
	if test $# -eq 1 ; then
		decodedir
	elif test $# -eq 2 ; then
		encodedir
	else
		echo "error usage: $0 [DST] [SRC]" >&2 
		return 1
	fi
}

function encodedir() {
	echo "base64 < $1"
	chmod 644 un
}
