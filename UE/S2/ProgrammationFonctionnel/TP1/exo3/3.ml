(**
* File              : 3.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :58 CET
* Last Modified Date: mer. 14 févr. 2018 18:18:33 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let rec list_sum lst =
	match lst with
		| []		-> 0.0
		| head::tail	-> head +. (list_sum tail)
	;;

Printf.printf "%f %f\n" (list_sum []) (list_sum [1.0 ; 2.0 ; 3.0 ; 4.0 ; 5.0]) ;;
