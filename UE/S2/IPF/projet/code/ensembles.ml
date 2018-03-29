(**
* File              : ensembles.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :12 CET
* Last Modified Date: mar. 27 mars 2018 17:35:47 CEST
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
 *	Un module pratique pour travailler avec des
 *	ensembles mathématiques (listes OCaml sans doublons)
 *)
open Listes ;;

(**
 *	Fonction 'set_add'
 *
 *	@param  : 'a list -> 'a -> 'a list
 *	@return : s u {x} ('s union {x}')
 *)
let set_add = function s -> function x -> if list_contains s x then s else x::s ;;

(**
 *	Fonction 'list_to_set'
 *
 *	@type   : 'a list -> 'a list
 *	@return : converti la liste en ensemble contenant tous ces elements
 *)
let rec list_to_set =	function l ->
				match l with
                    		| []	-> []
                    		| h::t	-> set_add (list_to_set t) h
			;;

(**
 *	Fonction 'set_intersect'
 *
 *	@type   : 'a list -> 'a list -> 'a list
 *	@return : s1 n s2 ('s1 inter s2')
 *)
let rec set_intersect =	function s1 -> function s2 ->
                        	match s1 with
                        	| []	->	[]
                        	| h::t	->	if list_contains s2 h then 
							h::(set_intersect t s2)
                        			else
                        				set_intersect t s2
                       	;;

(**
 *	Fonction 'set_union'
 *
 *	@type   : 'a list -> 'a list -> 'a list
 *	@return : s1 u s2 ('s1 union s2')
 *)
let set_union = function s1 -> function s2 -> list_to_set (s1 @ s2) ;;

