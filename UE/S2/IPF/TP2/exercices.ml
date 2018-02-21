(**
* File              : exercices.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :36 CET
* Last Modified Date: mer. 21 févr. 2018 13:44:12 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(** EXO 1 *)

(**
*	@type    : 'a list -> int
*	@ensure  : renvoie le nombre d'élément de la liste
*)
let rec list_length l =
		match l with
		| []	-> 0
		| _::t 	-> 1 + (list_length t)
	;;

(**
*	@type    : int list -> int
*	@ensure  : renvoie le nombre d'élément pair de la liste
*)
let rec list_count_even l =
		match l with
		| []	->	0
		| h::t	->	(if h mod 2 = 0 then 1 else 0) + (list_count_even t)
	;;

(**
*	@type    : 'a list -> float
*	@ensure  : renvoie la somme des éléments de la liste
*)
let rec list_sum l =
		match l with
		| []	-> 0.0
		| h::t	-> h +. (list_sum t)
	;;

(**
*	@type    : 'a -> 'a -> 'a
*	@ensure  : renvoie le plus petit des 2 element
*)
let min a b = if a < b then a else b ;;

(**
*	@type    : 'a list -> int
*	@ensure  : renvoie le plus petit element de la liste
*)
let rec list_min l =
		match l with
		| []	->	failwith "liste vide"
		| [h]	->	h
		| h::t	->	min h (list_min t)
	;;

(**
*	@type    : int -> int -> int list
*	@ensure  : renvoie la liste des entiers entre n et m
*)
let rec intervalle n m =
		if n > m then
			[]
		else if n = m then
			[n]
		else
			n::(intervalle (n + 1) m)
	;;
(**
*	@type    : int -> int -> int list
*	@ensure  : renvoie la liste des entiers entre (n et m)
*			ou (m et n)
*)
let rec intervalle_signed n m =
		if n = m then
			[n]
		else if n > m then
			n::(intervalle_signed (n - 1) m)
		else
			n::(intervalle_signed (n + 1) m)
	;;

(**
*	@type    : int list -> int list
*	@ensure  : renvoie une liste contenant les entiers pairs de la liste
*)
let rec list_evens l =
		match l with
		| []	->	[]
		| h::t	->	if h mod 2 = 0 then
					h::(list_evens t)
				else
					list_evens t
	;;

(**
*	@type    : 'a list -> 'a -> bool
*	@ensure  : renvoie une liste contenant les entiers pairs de la liste
*)
(* let rec list_contains l x = *)
let rec list_contains =	function l ->
				function e ->
					match l with
					| []	-> false
					| h::t	-> 	if h = e then
								true
							else
								list_contains t e
								
			;;

(**
*	@type    : 'a list -> 'a -> bool
*	@ensure  : renvoie une liste contenant les entiers pairs de la liste
*)
(* let rec list_contains l x = *)
let list_indexof =
	let rec list_indexof_rec l e i =
		match l with
			| []	->	0
			| h::t	-> 	if h=e then
						i
					else
						list_indexof_rec t e (i + 1)
	in function l -> function e -> list_indexof_rec l e 1
;;


(**
*
*	Preuve mathématique, par récurrence sur la longueur de la liste 'n'
*
*	On note l(n) une liste à 'n' éléments, et p(n) la liste des pairs correspondant
*
*	Initialisation : n = 0
*		l(0) = [] => p(0) = []
*		0 <= 0 ... ok
*
*	Récurrence     : On suppose l(n) = h::t et longueur(pair(t)) <= longueur(t)
*			
*		si h est pair => ok
*		si h est impair => ok
*)





(** EXO 2 *)

(**
*	@type    : 'a list -> 'a -> 'a list
*	@ensure  : ajoutes l'élément à l'ensemble
*)
let set_add s x =
	if list_contains s x then
		s
	else
		x::s
;;

(**
*	@type    : 'a list -> 'a list
*	@ensure  : converti la liste en ensemble contenant tous ces éléments
*)
let rec list_to_set l =
		match l with
		| []	-> []
		| h::t	-> set_add (list_to_set t) h
;;

(**
*	@type    : 'a list -> 'a list -> 'a list
*	@ensure  : return s1 n s2
*)
let rec set_intersect s1 s2 =
	match s1 with
	| []	->	[]
	| h::t	->	if list_contains s2 h then
				h::(set_intersect t s2)
			else
				set_intersect t s2
;;

(**
*	@type    : 'a list -> 'a list -> 'a list
*	@ensure  : return s1 u s2
*)
let set_union s1 s2 =
	list_to_set (s1 @ s2)
;;



(** EXO 3 *)
let print_ratio (p, q) =	if q == 1 then
					Printf.printf "%d\n" p
				else
					Printf.printf "%d/%d\n" p q
			;;
(**
*	@type    : int * int -> int * int
*	@ensure  : inverse the rationnal
*)
let inverse_ratio (p, q) = (q, p) ;;

(**
*	@type    : int * int -> int * int -> int * int
*	@ensure  : do the addition of the 2 rationals
*)
let addition_ratio (a, b) (c, d) = (a*d + b*c , b*d) ;;


(** TODO : 3. et 4. *)

