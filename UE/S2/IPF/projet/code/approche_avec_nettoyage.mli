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
 *	@param  : int list -> int -> float -> int list
 *	@return : TODO
 *)
val clean_up : int list -> int -> float -> int list

(**
 *	@param	:	float -> int list -> int -> int
 *	@return :	le résultat du problème SUBSET-SUM-OPT
 *)
val subset_sum_2 : float -> int list -> int -> int
