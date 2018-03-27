(**
* File              : listes.mli
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :09 CEST
* Last Modified Date: mar. 27 mars 2018 17:38:10 CEST
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)
(**
 *	Un module pratique pour travailler avec les listes OCaml
 *)

(**
 *	Fonction 'list_contains'
 *
 *	@param  : 'a list -> 'a -> bool
 *	@return : renvoie vrai si la liste contient l'element, faux sinon
 *)
val list_contains : 'a list -> 'a -> bool

(**
 *	Fonction 'list_min'
 *
 *	@param  :	int list -> int
 *	@return :	le minimum de la liste d'entier positif fourni
 *			ou -1 si la liste est vide
 *)
val list_min : int list -> int

(**
 *	Fonction 'list_generate'
 *
 *	@param  :	int -> int -> int -> int list
 *	@return :	la liste des entiers allant de x0 à xn (xn inclu),
 *			avec un pas 'step'
 *)
val list_generate : int -> int -> int -> int list

(**
 *	Fonction 'list_first'
 *
 *	@param  : 'a list -> 'a
 *	@return : le 1er élément de la liste
 *)
val list_first : 'a list -> 'a

(**
 *	Fonction 'list_split'
 *
 *	@param  :	'a list -> ('a list * 'a list)
 *	@return :	Deux listes contenant le même nombre d'élément (à 1 prêt),
 *			et dont la concaténation contient tous les éléments de 'l'
 *)
val list_split : 'a list -> 'a list * 'a list

(**
 *	Fonction 'list_print'
 *
 *	@param  : 'a list -> ('a -> unit) -> unit
 *	@ensure : affiche la liste donne en parametre, en appliquant
 *						la fonction 'print_elem' a chacun de ces elements
 *)
val list_print : 'a list -> ('a -> 'b) -> unit

(**
 *	Fonction 'list_int_print'
 *
 *	@param  : int list -> unit
 *	@ensure : affiche la liste d'entier
 *)
val list_int_print : int list -> unit

(**
 *	Fonction 'list_int_list_print'
 *
 *	@param  : int list -> unit
 *	@ensure : affiche la liste de liste d'entier donne en parametre
 *)

val list_int_list_print : int list list -> unit

