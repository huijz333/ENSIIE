(**
 * File              : approche_plus_directe.ml
 * Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
 * Date              :07 CET
 * Last Modified Date: dim. 18 mars 2018 12:07:30 CET
 * Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
 *)
(**
 *	4. Approche plus directe:
 *
 *	Réponses aux questions 4 et 5
 *)

(** importe les fonctions sur les 'set' *)
open Utils ;;

(**
 *	Fonction 'get_all_sums'
 *
 *	@param  :	int list -> int list
 *	@return :	renvoie la liste des sommes atteignables,
 *			en sommant sur les sous ensembles de la liste l
 *
 *)
let rec get_all_sums =	function l ->
				match l with
				| []		->	[0]
				| e::tail	->	let sums = get_all_sums tail in
				set_union sums (List.map (function t -> e + t) sums)
			;;


(**
 *	Fonction 'subset_sum_1'
 *
 *	@param  :	int list -> int -> int
 *	@return :	le résultat du problème SUBSET-SUM-OPT sur la
 *			liste et l'entier donné
 *)
let subset_sum_1 = function l -> function n ->
			let sums = get_all_sums l in
			List.fold_left	(function sum1 -> function sum2 ->
						if (sum1 < sum2 && sum2 <= n) then
							sum2
						else
							sum1
					) 0 sums
			;;


