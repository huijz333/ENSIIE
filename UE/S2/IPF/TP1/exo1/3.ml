(**
* File              : 3.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :14 CET
* Last Modified Date: mer. 14 févr. 2018 11:45:54 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let volumeSphere =	function r ->
				let pi = 3.141592653589793 in
					4. /. 3. *. pi *. r *. r *. r
			;;

Printf.printf "%f %f %f\n" (volumeSphere (0.0)) (volumeSphere (1.0)) (volumeSphere (-1.0)) ;;
		
