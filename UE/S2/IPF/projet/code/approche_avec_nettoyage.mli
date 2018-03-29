(**
* File              : approche_avec_nettoyage.mli
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :58 CET
* Last Modified Date: dim. 18 mars 2018 19:23:30 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
 *	5. Approche avec nettoyage:
 *
 *	Réponses aux questions 6, 7
 *)

(**
 *	Fonction 'clean_up'
 *
 *	@param  : (int * int list) list -> int -> float -> (int * int list) list
 *	@return : TODO
 *)
val clean_up : (int * int list) list -> int -> float -> (int * int list) list

(**
 *	Fonction 'get_all_sums_2'
 *
 *	@param  :	float -> int list -> int -> (int * int list) list
 *	@return :	renvoie la liste des sommes atteignables,
 *			filtrée par la fonction 'clean_up',
 *			en sommant sur les sous ensembles de la liste l
 *)
val get_all_sums_2 : float -> int list -> int -> (int * int list) list
	

(**
 *	@param	:	float -> int list -> int -> int * int list
 *	@return :	le résultat du problème SUBSET-SUM-OPT
 *)
val subset_sum_2 : float -> int list -> int -> int * int list
