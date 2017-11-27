# u s a g e : s 3 6 s t r k1 s1 k2 s2 k3 s3
# imprime l e s c h a i n e s s ( i ) en f o n c t i o n de l ’ o r d r e
# d e s c l e s k ( i ) . Les c l e s s o n t comparees de f a c o n
# lexicographique .

#!/bin/bash

function s36str() {
	if test $# -ep 3 ; then
		s36str "$1" "$1" "$2" "$2" "$3" "$3"
		return 0
	fi


}
