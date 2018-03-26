(**
* File              : approche_avec_nettoyage.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :42 CET
* Last Modified Date: dim. 18 mars 2018 19:31:39 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
 *	5. Approche avec nettoyage:
 *
 *	Réponses aux questions 6, 7
 *)

open Utils ;;

(**
 *	Fonction 'clean_up'
 *
 *	@param  :	int list -> int -> float -> int list
 *	@return :	l'ensemble des entiers 'l' filtré:
 *			- les entiers plus grand que 's' sont supprimés
 *			- lorsque 2 entiers sont 'delta prêt égaux', le plus petit est gardé
 *
 *	'delta prêt égaux' : 2 entiers x < y, sont 'delta prêt égaux', si (1 + delta)*x > y
 *	Autrement dit, si en perturbant le plus petit (par un facteur proche de 1),
 *	il devient plus grand que le 2ème. Ceci à lieu si 2 entiers sont 'très proches'
 *)
let clean_up =	function l -> function s -> function d ->
			let sl =  l in
			let m_min = list_first sl in
			let (t, _) = List.fold_left (function (t, m) -> function e ->
					if  (float_of_int e) > (1.0 +. d) *. (float_of_int m) && e <= s then
						(set_union t [e], e)
					else
						(t, m)
					)
			([m_min], m_min) sl in
			t
		;;

(**
 *	Fonction 'get_all_sums_2'
 *
 *	@param  :	float -> int list -> int list
 *	@return :	renvoie la liste des sommes atteignables, filtrer par la fonction 'clean_up'
 *			en sommant sur les sous ensembles de la liste l
 *)
let rec get_all_sums_2 = function d -> function l -> function s ->
				match l with
				| []	->	[0]
				| e::f	->	let t = get_all_sums_2 d f s in
					     	let t2 = List.map (function x -> x + e) t in
						clean_up (set_union t t2) s d
				;;
(**
 *	@param	:	float -> int list -> int -> int
 *	@return :	le résultat du problème SUBSET-SUM-OPT
 *)
let subset_sum_2 =	function d -> function l -> function n ->
				let sums = get_all_sums_2 d l n in
				List.fold_left	(function sum1 -> function sum2 ->
							if (sum1 < sum2 && sum2 <= n) then
								sum2
							else
								sum1
						) 0 sums
			;;
