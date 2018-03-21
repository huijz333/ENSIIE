(** definition du type arbre *)
type 'a arbre_bin = 
|	Vide
|	Noeud of 'a arbre_bin * 'a * 'a arbre_bin
;;

(** arbre de test *)
let a3	= Noeud(Vide,	3,	Vide) ;;
let a5	= Noeud(Vide,	5,	Vide) ;;
let a4	= Noeud(a3,	4,	a5) ;;

(**
 *	@type	: int arbre_bin -> int
 *	@return : la somme des valeurs de l'arbre
 *)
let rec ab_sum =	function a ->
				match a with
				|	Vide		-> 0
				|	Noeud (l, v, r)	-> (ab_sum l) + v + (ab_sum r)
			;;

(**
 *	@type	: 'a arbre_bin -> ('a -> 'b) -> 'b arbre_bin
 *	@return : l'arbre f(a)
 *)
let rec ab_map =	function a -> function f ->
				match a with
				| Vide			-> Vide
				| Noeud (l, v, r)	-> Noeud((ab_map l f), (f v), (ab_map r f))
			;;


(** INFIX/PREFIX/POSTFIX avec '@', i.e, concatenation de listes *)

(**
 *	@type	: 'a arbre_bin -> 'a list
 *	@return : la liste des valeurs de l'arbre, dans l'ordre infix
 *)
let rec ab_infix =	function a ->
				match a with
				| Vide			->	[]
				| Noeud (l, v, r)	->	(ab_infix l)@v::(ab_infix r)
			;;

(**
 *	@type	: 'a arbre_bin -> 'a list
 *	@return : la liste des valeurs de l'arbre, dans l'ordre prefix
 *)
let rec ab_prefix =	function a ->
				match a with
				| Vide			->	[]
				| Noeud (l, v, r)	->	v::(ab_infix l)@(ab_infix r)
			;;
(**
 *	@type	: 'a arbre_bin -> 'a list
 *	@return : la liste des valeurs de l'arbre, dans l'ordre prefix
 *)
let rec ab_postfix =	function a ->
				match a with
				| Vide			->	[]
				| Noeud (l, v, r)	->	(ab_infix l)@(ab_infix r)@[v]

(**
 *	@type	: 'a arbre_bin -> 'a
 *	@return : la plus grande valeur de l'arbre
 *)
let rec ab_max =	function a ->
				let max = function x -> function y -> if x > y then x else y in
				match a with
				| Vide			-> failwith "arbre vide"
				| Noeud (Vide, v, Vide)	-> v
				| Noeud (Vide, v, r)	-> max v (ab_max r)
				| Noeud (l,    v, Vide)	-> max v (ab_max l)
				| Noeud (l, v, r)	-> max (max v (ab_max l)) (ab_max r)
			;;
(**
 *	@type	: 'a arbre_bin -> 'a
 *	@return : la plus grande valeur de l'arbre de recherche
 *)
let rec abr_max =	function a ->
				match a with
				| Vide -> failwith "arbre vide"
				| Noeud (_, v, Vide)	-> v
				| Noeud (_, v, r)	-> abr_max r
			;;

(**
 *	@type	: 'a arbre_bin -> 'a arbre_bin
 *	@return : la plus grande valeur de l'arbre de recherche
 *)
let rec abr_insert =	function x -> function a ->
				let f = Noeud(Vide, x, Vide) in
				match a with
				| Vide -> f
				| Noeud (Vide, v, Vide)	->	if x < v then
									Noeud(f, v, Vide)
								else
									Noeud(Vide, v, f)
				| Noeud (l, v, Vide)	->	if x < v then
									Noeud (abr_insert x l, v, Vide)
								else
									Noeud(Vide, x, f)	
				| Noeud (Vide, v, r)	->	if x < v then
									Noeud(f, x, r)
								else
									Noeud(Vide, v, abr_insert x r)
				| Noeud (l, v, r)	->	if x < v then
									Noeud(abr_insert x l, v, r)
								else
									Noeud(l, v, abr_insert x r)
			;;




