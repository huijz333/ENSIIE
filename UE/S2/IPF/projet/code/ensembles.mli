(**
* File              : ensembles.mli
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              : mar. 27 mars 2018 17:39:32 CEST
* Last Modified Date: mar. 27 mars 2018 17:39:32 CEST
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
 *	Un module pratique pour travailler avec des
 *	ensembles mathématiques (listes OCaml sans doublons)
 *)


(**
 *	Fonction 'set_add'
 *
 *	@param  : 'a list -> 'a -> 'a list
 *	@return : s u {x} ('s union {x}')
 *)
val set_add : 'a list -> 'a -> 'a list

(**
 *	Fonction 'list_to_set'
 *
 *	@type   : 'a list -> 'a list
 *	@return : converti la liste en ensemble contenant tous ces elements
 *)
val list_to_set : 'a list -> 'a list

(**
 *	Fonction 'set_intersect'
 *
 *	@type   : 'a list -> 'a list -> 'a list
 *	@return : s1 n s2 ('s1 inter s2')
 *)
val set_intersect : 'a list -> 'a list -> 'a list

(**
 *	Fonction 'set_union'
 *
 *	@type   : 'a list -> 'a list -> 'a list
 *	@return : s1 u s2 ('s1 union s2')
 *)
val set_union : 'a list -> 'a list -> 'a list

