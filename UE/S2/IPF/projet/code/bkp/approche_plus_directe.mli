(**
* File              : approche_plus_directe.mli
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :02 CET
* Last Modified Date: dim. 18 mars 2018 12:04:26 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)
(**
 *	4. Approche plus directe:
 *
 *	Réponses aux questions 4 et 5
 *)

(**
 *	Fonction 'get_all_sums'
 *
 *	@param  : 	int list -> int list
 *	@return : 	renvoie la liste des sommes atteignables,
 *			en sommant sur les sous ensembles de la liste l
 *
 *)
val get_all_sums : int list -> int list

(**
 *	Fonction 'subset_sum_1'
 *
 *	@param  :	int list -> int -> int
 *	@return :	le résultat du problème SUBSET-SUM-OPT sur la liste et l'entier donné
 *)
val subset_sum_1 : int list -> int -> int
