(**
* File              : 2.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :34 CET
* Last Modified Date: mer. 14 févr. 2018 12:26:10 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let rec sum n =	if n <= 1 then
			n
		else
			sum (n-1) + n
		;;

Printf.printf "%d %d %d\n" (sum 0) (sum 3) (sum 100) ;;
