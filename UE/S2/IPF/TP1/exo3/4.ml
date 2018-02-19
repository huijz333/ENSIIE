(**
* File              : 4.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :45 CET
* Last Modified Date: mer. 14 févr. 2018 18:23:23 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let min a b =	if a < b then a else b ;;

let rec list_min lst =
	match lst with
		| []		-> failwith "empty list"
		| [head]	-> head
		| head::tail	-> min head (list_min tail)
	;;

Printf.printf "%d %d %d\n" (list_min [42 ; 0 ; -42]) (list_min [0]) (list_min [-42 ; 0 ; 42]) ;;
Printf.printf "%d\n" (list_min []) ;;
