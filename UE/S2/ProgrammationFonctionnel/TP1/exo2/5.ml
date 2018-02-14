(**
* File              : 5.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :09 CET
* Last Modified Date: mer. 14 févr. 2018 12:34:22 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let rec pow_dic_k a k =	if k = 0 then
				1
			else if k mod 2 = 0 then
				let x = pow_dic_k a (k / 2) in
					x * x
			else
				let x = pow_dic_k a ((k - 1) / 2) in
					x * x * a
			;;

Printf.printf "%d %d %d\n" (pow_dic_k 2 2) (pow_dic_k 2 3) (pow_dic_k 2 32) ;;
