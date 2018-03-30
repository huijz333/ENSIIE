(**
* File              : approche_naive.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :26 CET
* Last Modified Date: mar. 27 mars 2018 17:48:16 CEST
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)
(**
 *	3. Approche Naive:
 *
 *	Réponses aux questions 1, 2, 3
 *)

(**
 *	Fonction 'sum'
 *
 *	@param  : 	int list -> int
 *	@return : 	la somme des éléments de la liste d'entier
 *)
let sum = function l -> List.fold_left (function s -> function x -> s + x) 0 l ;;

(**
 *	Fonction 'powerset'
 *
 *	@param  : 'a list -> ('a list) list
 *	@return : la liste des sous-listes que l'on peut extraire de 'l'
 *)
let rec powerset =	function l ->
				match l with
				| [] 	->	[[]]
				| h::t	->	let ps = powerset t in
						ps@(List.map (function sl -> h::sl) ps)
			;;

(**
 *	Fonction 'subset_sum_0'
 *
 *	@param  : 	int list -> int -> int
 *	@return : 	le résultat du problème SUBSET-SUM-OPT
 *			sur la liste et l'entier donné
 *)
let subset_sum_0 =	function l -> function s ->
				let ps = powerset l in
				(* fonction qui a un entier 'm', et
				   une liste d'entier 'sl', associe
				   un entier s' (égal à 'sum sl' ou à 'm')
				   tel que: m <= s' <= s *)
				let f =	function m -> function sl ->
						let s' = sum sl in
						if (m < s' && s' <= s) then s' else m
					in
				List.fold_left f 0 ps
			;;

