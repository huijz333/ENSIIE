(**
* File              : rationel.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :01 CET
* Last Modified Date: mer. 14 févr. 2018 12:45:49 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let print_ratio (p, q) =	if q == 1 then
					Printf.printf "%d\n" p
				else
					Printf.printf "%d/%d\n" p q
			;;

let inverse_ratio (p, q) =	(q, p) ;;

let addition_ratio (a, b) (c, d) = (a*d + b*c , b*d) ;;

let x = (1, 3) ;;
print_ratio x ;;
print_ratio (inverse_ratio x) ;;
print_ratio (addition_ratio x x) ;;
