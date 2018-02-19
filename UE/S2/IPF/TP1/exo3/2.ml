(**
* File              : 2.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :37 CET
* Last Modified Date: mer. 14 févr. 2018 18:18:29 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let rec list_count_even lst =
	match lst with
		| []		-> 0
		| head::tail	->
			if head mod 2 == 0 then
				1 + (list_count_even tail)
			else
				0 + (list_count_even tail)
		;;

Printf.printf "%d %d\n" (list_count_even []) (list_count_even [1 ; 2 ; 3 ; 4 ; 5 ; 6]) ;;
