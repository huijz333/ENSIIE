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
 *	@param  : int list -> int -> float -> int list
 *	@return : TODO
 *)
let clean_up =	function l -> function s -> function d ->
			let sl = List.sort (function x -> function y -> x - y) l in
			let m1 = list_first sl in
			let (t, _) = List.fold_left (function (t, m2) -> function e ->
					if e != m2 && e <= s &&
					(float_of_int e) >= (1.0 +. d) *. (float_of_int m2)
					then
						(set_union t [e], e)
					else
						(t, m2)
					)
			([m1], m1) sl in
			t
		;;

(**
 *	Fonction 'get_all_sums'
 *
 *	@param  :	int list -> int list
 *	@return :	renvoie la liste des sommes atteignables,
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
