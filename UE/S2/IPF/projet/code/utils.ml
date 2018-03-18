(**
* File              : utils.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :12 CET
* Last Modified Date: dim. 18 mars 2018 15:05:35 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)
(**
 *	Module de fonction internes, facilitant le reste du programme.@author 
 *		1 - Fonctions permettant de travailler avec des ensembles mathematiques
 *			(sous forme de liste OCaml sans doublons)
 *
 *		2 - Fonction pratiques sur les listes	
 *)

(**
 *	@param  : 'a list -> 'a -> bool
 *	@return : renvoie vrai si la liste contient l'element, faux sinon
 *)
let rec list_contains =	function l -> function e ->
				match l with
                		| []	-> 	false
                		| h::t	-> 	if h = e then
                					true
                				else
                					list_contains t e
                	;;

(**
 *	@param  :	int list -> int
 *	@return :	le minimum de la liste d'entier positif fourni
 *			ou -1 si la liste est vide
 *)
let rec list_min =	function l ->
				let min =	function x -> function y ->
							if x < y then x else y
				in
				match l with
				| []	-> -1
				| h::t  -> min h (list_min t)
			;;

(**
 *	@param  :	int -> int -> int -> int list
 *	@return :	la liste des entiers allant de x0 à xn (xn inclu),
 *			avec un pas 'step'
 *)
let rec list_generate =	function x0 -> function xn -> function step ->
				if x0 <= xn then
					x0::(list_generate (x0 + step) xn step)
				else
					[]
			;;

(**
 *	@param  : 'a list -> 'a
 *	@return : le 1er élément de la liste
 *)
let list_first =	function l ->
				match l with
				| []	-> failwith "Empty list!"
				| h::t	-> h
			;;
(**
 *	@param  : 'a list -> 'a -> 'a list
 *	@return : s u {x} ('s union {x}')
 *)
let set_add = function s -> function x -> if list_contains s x then s else x::s ;;

(**
 *	@type   : 'a list -> 'a list
 *	@return : converti la liste en ensemble contenant tous ces elements
 *)
let rec list_to_set =	function l ->
				match l with
                    		| []	-> []
                    		| h::t	-> set_add (list_to_set t) h
			;;

(**
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
 *	@type   : 'a list -> 'a list -> 'a list
 *	@return : s1 u s2 ('s1 union s2')
 *)
let set_union = function s1 -> function s2 -> list_to_set (s1 @ s2) ;;

(**
 *	@param  : 'a list -> ('a -> unit) -> unit
 *	@ensure : affiche la liste donne en parametre, en appliquant
 *						la fonction 'print_elem' a chacun de ces elements
 *)
let lprint =	function l -> function print_elem ->
			let rec lprint_rec = function lrec ->
				match lrec with
				| []	->	Printf.printf "]"
				| [x]	->	print_elem x ; Printf.printf "]\n"
				| h::t	->	print_elem h ; Printf.printf "; " ;
						lprint_rec t
			in
			Printf.printf "[" ; lprint_rec l
		;;

(**
 *	@param  : int list -> unit
 *	@ensure : affiche la liste d'entier
 *)
let liprint = function l -> lprint l (function x -> Printf.printf "%d" x) ;;

(**
 *	@param  : int list -> unit
 *	@ensure : affiche la liste de liste d'entier donne en parametre
 *)
let lliprint =	function ll ->
			lprint ll liprint
		;;
