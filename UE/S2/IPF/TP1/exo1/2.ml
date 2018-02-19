(**
* File              : 2.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :05 CET
* Last Modified Date: mer. 14 févr. 2018 12:15:05 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let signum =	function x ->
			if x < 0 then
				(-1)
			else if x = 0 then
				0
			else
				1
		;;

Printf.printf "%d %d %d\n" (signum (-42)) (signum 0) (signum 42) ;;
