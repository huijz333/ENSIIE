(**
* File              : 3.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :34 CET
* Last Modified Date: mer. 14 févr. 2018 12:26:52 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let rec sum2 n =	if n <= 1 then
				n
			else
				sum2 (n-1) + n*n
			;;

Printf.printf "%d %d %d\n" (sum2 0) (sum2 3) (sum2 100) ;;
