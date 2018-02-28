(**
* File              : exo2.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :55 CET
* Last Modified Date: mer. 28 févr. 2018 15:25:05 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let sq = function x -> x * x ;;
let lst = [3 ; 12 ; 3 ; 40 ; 6 ; 4 ; 6 ; 0] ;;

(** 1 *)
let lstsq = List.map sq lst ;;


(** 2 *)
(**
*	@type    : 'a list -> 'a
*	@ensure  : renvoie la somme des éléments de la liste
*)
let list_sum = function lst -> List.fold_right (+) lst 0;;
Printf.printf "6 == %d\n" (list_sum [1 ; 2 ; 3]) ;;

(**
*	@type    : 'a list -> int
*	@ensure  : renvoie le nombre d'élément de la liste
*)
let list_length = function lst -> List.fold_right (function _ -> function n -> n + 1) lst 0;;
Printf.printf "3 == %d\n" (list_length [1 ; 2 ; 3]) ;;

(**
*	@type    : int list -> int
*	@ensure  : renvoie le nombre d'élément pair de la liste
*)

let list_count_even = function lst ->
		let f = (function t -> function n -> if t mod 2 = 0 then n + 1 else n) in
		List.fold_right f lst 0
	;;
Printf.printf "2 == %d\n" (list_count_even [0 ; 1 ; 2 ; 3]) ;;
(* ou : *)
let list_count_even lst = List.fold_right (function t -> function n -> if t mod 2 = 0 then n + 1 else n) lst 0
	;;
Printf.printf "2 == %d\n" (list_count_even [0 ; 1 ; 2 ; 3]) ;;


(**
*	@type    : 'a list -> int
*	@ensure  : renvoie le plus petit element de la liste
*)
let list_min = function lst -> List.fold_right (function t -> function m -> if t < m then t else m) lst max_int ;;
Printf.printf "-42 == %d\n" (list_min [-42 ; 0 ; 42]) ;;
Printf.printf "-42 == %d\n" (list_min [42 ; 0 ; -42]) ;;

(**	nb_occ
 *	@type   : 'a -> 'a list
 *	@return : le nombre d’occurrences de x dans la liste l
 *)
let nb_occ =	function x -> function lst ->	List.fold_right
						(function t -> function n ->
							(if x = t then 1 else 0) + n)
						lst
						0
		;;
Printf.printf "2 == %d\n" (nb_occ 1 [1 ; 0 ; 0 ; 1]) ;;

(**	moyenne
 *	@type   : float list -> float
 *	@return : la moyenne de la liste
 *)
let moyenne =	function lst -> 
			let sum_and_n = function lst ->
				List.fold_right
					(function t -> function (sum, n) -> (sum +. t, n + 1))
					lst
					(0.0, 0)
				in
			let (sum, n) = sum_and_n lst in
			sum /. float_of_int(n)
		;;
Printf.printf "20.0 == %f\n" (moyenne [10.0 ; 20.0 ; 30.0] ) ;;

(**	list_map
 *	@type   : 'a list -> f('a) list
 *	@return : la liste [f(x1), ..., f(xn)] si lst = [x1, ..., xn]
 *)
let rec list_map =	function lst ->
				function f ->
					match lst with
					| []	-> []
					| h::t	-> [f h]@(list_map t f)
			;;

