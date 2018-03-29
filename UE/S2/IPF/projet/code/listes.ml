(**
* File              : listes.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :08 CEST
* Last Modified Date: mer. 28 mars 2018 15:27:03 CEST
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
let rec list_contains =	function l -> function e ->
	match l with
	| []	-> 	false
        | h::t	-> 	if h = e then
         			true
                	else
                		list_contains t e
;;

(**
 *	Fonction 'list_min'
 *
 *	@param  :	int list -> int
 *	@return :	le minimum de la liste d'entier positif fourni
 *			ou -1 si la liste est vide
 *)
let rec list_min = function l ->
	let min =	function x -> function y ->
				if x < y then x else y
			in
			match l with
			| []	-> -1
			| h::t  -> min h (list_min t)
;;

(**
 *	Fonction 'list_generate'
 *
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
 *	Fonction 'list_first'
 *
 *	@param  : 'a list -> 'a
 *	@return : le 1er élément de la liste
 *)
let list_first = function l ->
	match l with
	| []	-> failwith "Empty list!"
	| h::t	-> h
;;

(**
 *	Fonction 'list_split'
 *
 *	@param  :	'a list -> ('a list * 'a list)
 *	@return :	Deux listes contenant le même nombre d'élément (à 1 prêt),
 *			et dont la concaténation contient tous les éléments de 'l'
 *)
let list_split = function l ->
	let rec aux = function lst -> function l1 -> function l2 ->
		match lst with
		| []		-> (l1, l2)
		| x::y::t	-> aux t (x::l1) (y::l2)
		| x::t		-> aux t (x::l1) l2
		in
	aux l [] []
;;

(**
 *	Fonction 'list_reverse'
 *
 *	@param  :	'a list -> 'a list
 *	@return :	La liste des éléments inversés de 'l'
 *)
let list_reverse = function l ->
	List.fold_left (function l -> function x -> x::l) [] l
;;

(**
 *	Fonction 'list_print'
 *
 *	@param  : 'a list -> ('a -> unit) -> unit
 *	@ensure : affiche la liste donne en parametre, en appliquant
 *						la fonction 'print_elem' a chacun de ces elements
 *)
let list_print = function l -> function print_elem ->
	let rec list_print_rec = function lrec ->
	match lrec with
	| []	->	Printf.printf "]"
	| [x]	->	print_elem x ; Printf.printf "]"
	| h::t	->	print_elem h ; Printf.printf "; " ;
			list_print_rec t
	in
	Printf.printf "[" ; list_print_rec l
;;

(**
 *	Fonction 'list_int_print'
 *
 *	@param  : int list -> unit
 *	@ensure : affiche la liste d'entier
 *)
let list_int_print = function l -> list_print l (function x -> Printf.printf "%d" x) ;;

(**
 *	Fonction 'list_list_int_print'
 *
 *	@param  : int list -> unit
 *	@ensure : affiche la liste de liste d'entier donne en parametre
 *)
let list_int_list_print = function ll -> list_print ll list_int_print ;;
