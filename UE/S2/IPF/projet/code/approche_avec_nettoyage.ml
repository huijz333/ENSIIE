(**
* File              : approche_avec_nettoyage.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :42 CET
* Last Modified Date: mer. 28 mars 2018 16:22:39 CEST
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
 *	5. Approche avec nettoyage:
 *
 *	Réponses aux questions 6, 7
 *)

open Ensembles ;;
open Listes ;;

(**
 *	Fonction 'clean_up'
 *
 *	@param  :	(int * int list) list -> int -> float -> (int * int list) list
 *	@return :	l'ensemble des entiers 'l' filtré:
 *			- les entiers plus grand que 's' sont supprimés
 *			- lorsque 2 entiers sont 'delta prêt égaux', le plus petit est gardé
 *
 *	'delta prêt égaux' : 2 entiers x < y, sont 'delta prêt égaux', si (1 + delta)*x > y
 *	Autrement dit, si en perturbant le plus petit (par un facteur proche de 1),
 *	il devient plus grand que le 2ème. Ceci à lieu si 2 entiers sont 'très proches'
 *)
let clean_up =	function l -> function s -> function d ->	
	let (s0, e0) = list_first l in
	let filter = function (t, m) -> function (s', e') ->
		if s' != m && (float_of_int s') > (1.0 +. d) *. (float_of_int m) && s' <= s then
			(set_add t (s', e'), s')
		else
			(t, m)
	in
	let (t, _) = List.fold_left
			filter
			([(s0, e0)], s0)
			(List.sort (function (x, _) -> function (y, _) -> x - y) l)
	in
	t
;;

(**
 *	Fonction 'get_all_sums_2'
 *
 *	@param  :	float -> int list -> int -> (int * int list) list
 *	@return :	renvoie la liste des sommes atteignables,
 *			filtrée par la fonction 'clean_up',
 *			en sommant sur les sous ensembles de la liste l
 *)
let rec get_all_sums_2 = function d -> function l -> function s ->
	match l with
	| []	->	[(0, [])]
	| x::f	->	let t = get_all_sums_2 d f s in
		     	let t2 = List.map (function (s, e) -> (x + s, x::e)) t in
			clean_up (set_union t t2) s d
;;

(**
 *	@param	:	float -> int list -> int -> int * int list
 *	@return :	le résultat du problème SUBSET-SUM-OPT
 *)
let subset_sum_2 = function d -> function l -> function s ->
	let sums = get_all_sums_2 d l s in
	let best_sum = function (s', e') -> function (s'', e'') ->
		if (s' < s'' && s'' <= s) then (s'', e'') else (s', e')
	in
	List.fold_left best_sum (0, []) sums
;;
