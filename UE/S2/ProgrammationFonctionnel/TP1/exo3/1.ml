(**
* File              : 1.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :22 CET
* Last Modified Date: mer. 14 févr. 2018 18:18:20 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let rec list_length lst =
	match lst with
		| []		-> 0
		| head::tail 	-> 1 + (list_length tail)
	;;

Printf.printf "%d %d\n" (list_length []) (list_length [1 ; 2 ; 3]) ;;
