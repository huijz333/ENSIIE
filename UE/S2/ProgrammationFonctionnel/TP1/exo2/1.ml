(**
* File              : 1.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :06 CET
* Last Modified Date: mer. 14 févr. 2018 12:23:25 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let rec u n =	if n=0 then
			1.0
		else
			sqrt(u (n-1)) +. 2.0
		;;

Printf.printf "%f %f %f %f\n" (u 0) (u 1) (u 10) (u 100) ;;
