(**
 *	4. Approche plus directe:
 *
 *	Réponses aux questions 4 et 5
 *)

(**
 *	Fonctions permettant de travailler avec des ensembles mathématiques
 *	(sous forme de liste OCaml sans doublons)
 *)


(**
 *	@param  : 'a list -> 'a -> bool
 *	@return : renvoie vrai si la liste contient l'élément, faux sinon
 *)
let rec list_contains =	function l -> function e ->
													match l with
                					| []	-> false
                					| h::t	-> 	if h = e then
                								true
                							else
                								list_contains t e
                			;;

(**
 *	@param  : 'a list -> 'a -> 'a list
 *	@return : s u {x} ('s union {x}')
 *)
let set_add = function s -> function x -> if list_contains s x then s else x::s ;;

(**
 *	@type   : 'a list -> 'a list
 *	@return : converti la liste en ensemble contenant tous ces éléments
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
 *	Fonction 'get_all_sums'
 *
 *	@param  : int list -> int list
 *	@return : renvoie la liste des sommes atteignables,
 *						en sommant sur les sous ensembles de la liste l
 *
 *)
let rec get_all_sums =	function l ->
													match l with
													| []			->	[0]
													| e::tail	->	let sums = get_all_sums tail in
																				set_union sums (List.map (function t -> e + t) sums)
												;;


(**
 *	Fonction 'subset_sum_1'
 *
 *	@param  : int list -> int -> int
 *	@return : le résultat du problème SUBSET-SUM-OPT sur la liste et l'entier donné
 *)
let subset_sum_1 =	function l -> function n ->
											let sums = get_all_sums l in
											List.fold_left	(function sum1 -> function sum2 ->
																				if (sum1 < sum2 && sum2 <= n) then sum2 else sum1
																			) 0 sums
										;;

(** Test de la fonction 'get_all_sums' *)
"Test de la fonction 'get_all_sums'" ;;
"Sortie attendue:" ;;
[0] ;;
[0; 1] ;;
[0; 1; 2] ;;
[0; 1; 3; 4; 5; 7; 8] ;;
"Sortie obtenue:" ;;
get_all_sums [] ;;
get_all_sums [1] ;;
get_all_sums [1; 1] ;;
List.sort (function x -> function y -> x - y) (get_all_sums [1; 3; 4]) ;;


(** Test de la fonction 'subset_sum_0' *)
"Test de la fonction 'subset_sum_1'" ;;
"Sortie attendue:" ;;
0 ;;
1 ;;
0 ;;
6 ;;
6 ;;
5 ;;
6 ;;
9707 ;;
"Sortie obtenue:" ;;
subset_sum_1 [] 42 ;;
subset_sum_1 [1] 42 ;;
subset_sum_1 [42] 1 ;;
subset_sum_1 [1; 2; 3] 42 ;;
subset_sum_1 [1; 2; 3] 6 ;;
subset_sum_1 [1; 2; 3] 5 ;;
subset_sum_1 [1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1] 6 ;;
subset_sum_1 [42; 666; 007; 24; 12; 30; 31; 27; 28; 1; 2; 3; 5000; 7500 ; 9500] 10000 ;;

