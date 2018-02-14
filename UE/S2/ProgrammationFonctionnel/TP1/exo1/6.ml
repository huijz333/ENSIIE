(**
* File              : 6.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :05 CET
* Last Modified Date: mer. 14 févr. 2018 12:18:47 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
 *	return the number of solution in R, of the 2nd degree equation:
 *		ax² + bx + c = 0
 *)
let numSol a b c =	let delta = b *. b -. 4.0 *. a *. c in
				if delta < 0.0 then
					0
				else if delta = 0.0 then
					1
				else
					2
		;;

Printf.printf "%d %d %d\n" (numSol (-1.0) 2.0 3.0) (numSol 0.0 0.0 0.0) (numSol 1.0 2.0 3.0) ;;
