(**
* File              : utils.mli
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :11 CET
* Last Modified Date: dim. 18 mars 2018 15:00:56 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)
(**
 *	Module de fonction internes, facilitant le reste du programme.@author 
 *		1 - Fonctions permettant de travailler avec des ensembles mathématiques
 *			(sous forme de liste OCaml sans doublons)
 *
 *		2 - Fonction pratiques sur les listes	
 *)

(**
 *	@param  : 'a list -> 'a -> bool
 *	@return : renvoie vrai si la liste contient l'élément, faux sinon
 *)
val list_contains : 'a list -> 'a -> bool

(**
 *	@param  :	int list -> int
 *	@return :	le minimum de la liste d'entier positif fourni
 *			ou -1 si la liste est vide
 *)
val list_min : int list -> int

(**
 *	@param  : 'a list -> 'a
 *	@return : le 1er élément de la liste
 *)
val list_first : 'a list -> 'a

(**
 *	@param  :	int -> int -> int -> int list
 *	@return :	la liste des entiers allant de x0 à xn, avec un pas 'step'
 *)
val list_generate : int -> int -> int -> int list
	

(**
 *	@param  : 'a list -> 'a -> 'a list
 *	@return : s u {x} ('s union {x}')
 *)
val set_add : 'a list -> 'a -> 'a list

(**
 *	@type   : 'a list -> 'a list
 *	@return : converti la liste en ensemble contenant tous ces éléments
 *)
val list_to_set : 'a list -> 'a list

(**
 *	@type   : 'a list -> 'a list -> 'a list
 *	@return : s1 n s2 ('s1 inter s2')
 *)
val set_intersect : 'a list -> 'a list -> 'a list

(**
 *	@type   : 'a list -> 'a list -> 'a list
 *	@return : s1 u s2 ('s1 union s2')
 *)
val set_union : 'a list -> 'a list -> 'a list

(**
 *	@param  : 'a list -> ('a -> unit) -> unit
 *	@ensure : affiche la liste donné en paramètre, en appliquant
 *						la fonction 'print_elem' a chacun de ces éléments
 *)
val lprint : 'a list -> ('a -> 'b) -> unit

(**
 *	@param  : int list -> unit
 *	@ensure : affiche la liste de liste d'entier donné en paramètre
 *)
val lliprint : int list list -> unit


(**
 *	@param  : int list -> unit
 *	@ensure : affiche la liste d'entier
 *)
val liprint : int list -> unit

(**
 *	@param  : int list -> unit
 *	@ensure : affiche la liste de liste d'entier donne en parametre
 *)
val lliprint : int list list -> unit
