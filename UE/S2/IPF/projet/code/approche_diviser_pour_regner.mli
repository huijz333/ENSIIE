(**
* File              : approche_diviser_pour_regner.mli
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :58 CET
* Last Modified Date: mar. 27 mars 2018 17:45:57 CEST
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
 *	6. Approche de type diviser pour régner:
 *
 *	Réponses aux questions 8, 9, 10
 *)

(**
 *	Fonction 'is_feasible'
 *
 *	@names	:	s   -> l1       -> l2       -> b
 *	@param  :	int -> int list -> int list -> bool
 *	@return :	Renvoie true si 's' s'écrit comme la somme d'un élément
 *			de 'l1' et d'un élément de 'l2', faux sinon
 *)
val is_feasible : int -> int list -> int list -> bool

(**
 *	Fonction 'best_feasible'
 *
 *	@names	:	s   -> l1       -> l2       -> s'
 *	@param  :	int * int list -> int list -> int list -> int * int list
 *	@return :	Renvoie le plus entier s', qui peut s'écrire comme la
 *			somme d'un élément de de 'l1' et d'un élément de 'l2',
 *			tel que s' < s, ou -1 si un tel s' n'existe pas.
 *)
val best_feasible : int -> (int * int list) list -> (int * int list) list -> int * int list

(**
 *	Fonction 'subset_sum_3'
 *
 *	@param	:	float -> int list -> int -> int * int list
 *	@return :	le résultat du problème SUBSET-SUM-OPT
 *)
val subset_sum_3 : int list -> int -> int * int list
