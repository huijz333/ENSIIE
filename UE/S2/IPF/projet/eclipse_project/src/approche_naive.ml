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
let sum =	function l -> List.fold_left (function s -> function x -> s + x) 0 l ;;

(**
 *	Fonction 'powerset'
 *
 *	@param  : 'a list -> ('a list) list
 *	@return : la liste des sous-listes que l'on peut extraire de 'l'
 *)
let rec powerset =	function l ->
											match l with
											| [] 		->	[[]]
											| h::t	->	let ps = powerset t in
																	ps@(List.map (function sl -> h::sl) ps)
										;;

(**
 *	Fonction 'subset_sum_0'
 *
 *	@param  : int list -> int -> int
 *	@return : le résultat du problème SUBSET-SUM-OPT sur la liste et l'entier donné
 *)
let subset_sum_0 =	function l -> function n ->
											let ps = powerset l in
											let subset_sum_min =	function m -> function sl ->
																							let s = sum sl in if (m < s && s <= n) then s else m
																						in
											List.fold_left subset_sum_min 0 ps
										;;




(** Test de la fonction 'sum' *)
"Test de la fonction 'sum'" ;;
"Sortie attendue:" ;;
0 ;;
1 ;;
6 ;;
"Sortie obtenue:" ;;
sum [] ;;
sum [1] ;;
sum [1 ; 2 ; 3] ;;




(** Test de la fonction 'powerset' *)
"Test de la fonction 'powerset'" ;;
"Sortie attendue:" ;;
[[]] ;;
[[]; [2]; [1]; [1; 2]] ;;
[[]; [3]; [2]; [2; 3]; [1]; [1; 3]; [1; 2]; [1; 2; 3]] ;;
"Sortie obtenue:" ;;
powerset [] ;;
powerset [1 ; 2] ;;
powerset [1 ; 2 ; 3] ;;



(** Test de la fonction 'subset_sum_0' *)
"Test de la fonction 'subset_sum_0'" ;;
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
subset_sum_0 [] 42 ;;
subset_sum_0 [1] 42 ;;
subset_sum_0 [42] 1 ;;
subset_sum_0 [1; 2; 3] 42 ;;
subset_sum_0 [1; 2; 3] 6 ;;
subset_sum_0 [1; 2; 3] 5 ;;
subset_sum_0 [1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1] 6 ;;
subset_sum_0 [42; 666; 007; 24; 12; 30; 31; 27; 28; 1; 2; 3; 5000; 7500 ; 9500] 10000 ;;

