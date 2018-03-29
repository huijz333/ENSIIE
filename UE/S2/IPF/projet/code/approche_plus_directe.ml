(**
 * File              : approche_plus_directe.ml
 * Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
 * Date              :07 CET
 * Last Modified Date: mer. 28 mars 2018 17:23:22 CEST
 * Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
 *)
(**
 *	4. Approche plus directe:
 *
 *	Réponses aux questions 4 et 5
 *)

(** importe les fonctions sur les 'set' *)
open Ensembles ;;

(**
 *	Fonction 'get_all_sums'
 *
 *	@names  :	l -> ret
 *	@param  : 	int list -> (int * int list) list
 *	@return : 	renvoie la liste des couples (somme, ensembles) = (s, e)
 *			ou 's' elle est la somme des éléments de 'e' c 'l'
 *
 *)
let rec get_all_sums =	function l ->
	match l with
	| []	->	[(0, [])]
	| x::t	->	let sums = get_all_sums t in
			set_union sums (List.map (function (s, e) -> (x + s, x::e)) sums)
;;

(**
 *	Fonction 'subset_sum_1'
 *
 *	@param  :	int list -> int -> int * int list
 *	@return :	le résultat du problème SUBSET-SUM-OPT sur la
 *			liste et l'entier donné
 *)
let subset_sum_1 = function l -> function s ->
			let sums = get_all_sums l in
			List.fold_left	(function (s', e') -> function (s'', e'') ->
						if (s' < s'' && s'' <= s) then
							(s'', e'')
						else
							(s', e')
					) (0, []) sums
			;;


