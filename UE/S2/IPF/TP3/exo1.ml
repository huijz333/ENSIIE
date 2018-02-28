(**
* File              : exo1.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :42 CET
* Last Modified Date: mer. 28 févr. 2018 11:57:52 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(** 1 : voir ../TD2 *)

(**	repeat
 *	@type   : 'int -> 'a -> 'a list
 *	@return :  crée une liste contenant n fois l'élément x
 *)
let rec repeat =	function n ->
				function x ->
					if n = 1 then
						[x]
					else
						x::(repeat (n-1) x)
			;;

(**	nb_occ
 *	@type   : 'a -> 'a list
 *	@return : le nombre d’occurrences de x dans la liste l
 *)
let rec nb_occ =	function x ->
				function lst ->
					match lst with
					| []	-> 0
					| h::t	-> (if h = x then 1 else 0) + nb_occ x t
			;;

Printf.printf "24 == %d\n" (nb_occ 42 (repeat 24 42)) ;;

(**	last
 *	@type   : 'a list -> 'a
 *	@return : le dernier élément de la liste lst
 *)
let rec last =		function lst ->
				match lst with
				| []	-> failwith "Empty list has no last element!"
				| [x]	-> x
				| h::t	-> last t
			;;
Printf.printf "4 == %d\n" (last [0 ; 1 ; 2 ; 4]) ;;

(**	is_increasing
 *	@type   : 'a list -> bool
 *	@return : vrai si la liste est croissante (ou vide), faux sinon
 *)
let rec is_increasing =	function lst ->
				match lst with
				| []		-> 	true
				| [x]		-> 	true
				| x::y::t	->	if x < y then
								is_increasing (y::t)
							else
								false
			;;
Printf.printf "true == %s\n" (string_of_bool (is_increasing [] )) ;;
Printf.printf "true == %s\n" (string_of_bool (is_increasing [1] )) ;;
Printf.printf "true == %s\n" (string_of_bool (is_increasing [1 ; 2 ; 3 ; 4 ; 5] )) ;;
Printf.printf "false == %s\n" (string_of_bool (is_increasing [1 ; 2 ; 3 ; 5 ; 4] )) ;;

(**	moyenne
 *	@type   : float list -> float
 *	@return : la moyenne de la liste
 *)
# use "../TP2/exercices.ml"
let moyenne =		function lst ->
				let length = list_length lst in
				if length = 0 then
					failwith "Liste vide, pas de moyenne"
				else
					list_sum lst /. (float_of_int length)
			;;
Printf.printf "20.0 == %f\n" (moyenne [10.0 ; 20.0 ; 30.0] ) ;;

let moyenne =		function lst ->
				let rec accumulator =
					function l   ->
					function sum ->
					function n   ->
						match l with
						| []	-> (sum, n)
						| h::t	-> accumulator t (sum +. h) (n + 1)
					in
				let (sum, n) = accumulator lst 0.0 0 in
				sum /. (float_of_int n)
			;;
Printf.printf "20.0 == %f\n" (moyenne [10.0 ; 20.0 ; 30.0] ) ;;

