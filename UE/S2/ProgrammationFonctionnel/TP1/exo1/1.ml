(**
* File              : 1.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :58 CET
* Last Modified Date: mer. 14 févr. 2018 12:11:10 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let isEven =	function x ->
			x mod 2 = 0
		;;

Printf.printf "%B\n" (isEven 0) ;;
Printf.printf "%B\n" (isEven 1) ;; 
Printf.printf "%B\n" (isEven 2) ;;
