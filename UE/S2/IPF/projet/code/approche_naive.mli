(**
 *	3. Approche Naive:
 *
 *	Réponses aux questions 1, 2, 3
 *)


(**
 *	Fonction 'sum'
 *
 *	@param  : int list -> int
 *	@return : la somme des éléments de la liste d'entier
 *)
val sum : int list -> int

(**
 *	Fonction 'powerset'
 *
 *	@param  : 'a list -> ('a list) list
 *	@return : la liste des sous-listes que l'on peut extraire de 'l'
 *)
val powerset : 'a list -> 'a list list

(**
 *	Fonction 'subset_sum_0'
 *
 *	@param  : int list -> int -> int
 *	@return : le résultat du problème SUBSET-SUM-OPT sur la liste et l'entier donné
 *)
val subset_sum_0 : int list -> int -> int
