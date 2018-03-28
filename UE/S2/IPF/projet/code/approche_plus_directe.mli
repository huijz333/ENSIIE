(**
* File              : approche_plus_directe.mli
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :02 CET
* Last Modified Date: mer. 28 mars 2018 17:09:11 CEST
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
 *	@names  :	l -> ret
 *	@param  : 	int list -> (int * int list) list
 *	@return : 	renvoie la liste des couples (somme, ensembles) = (s, e)
 *			ou 's' elle est la somme des éléments de 'e' c 'l'
 *
 *)
val get_all_sums : int list -> (int * int list) list

(**
 *	Fonction 'subset_sum_1'
 *
 *	@param  :	int list -> int -> int * int list
 *	@return :	le résultat du problème SUBSET-SUM-OPT sur la
 *			liste et l'entier donné
 *)
val subset_sum_1 : int list -> int -> int * int list
