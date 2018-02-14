(**
* File              : 4.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :09 CET
* Last Modified Date: mer. 14 févr. 2018 12:31:11 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let rec pow_dic a k =	if k = 0 then
				1
			else if k mod 2 = 0 then
				pow_dic (a * a) (k / 2)
			else
				pow_dic (a * a) ((k - 1) / 2) * a
			;;

Printf.printf "%d %d %d\n" (pow_dic 2 2) (pow_dic 2 3) (pow_dic 2 32) ;;
