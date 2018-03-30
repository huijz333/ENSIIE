(**
* File              : tests.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :31 CET
* Last Modified Date: mer. 28 mars 2018 17:23:23 CEST
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(** Tests de des différentes fonctions implémentés *)
open Printf ;;
open Listes ;;
open Ensembles ;;

(** Debut des tests des fonctions auxiliaires sur les listes *)
printf "###################################\n" ;;
printf "#    Listes : debut des tests     #\n" ;;
printf "###################################\n" ;;
assert (list_contains [1;2;3] 2) ;;
assert (list_contains [1;2;3] 0 = false) ;;
assert (list_min [5;1;2] = 1) ;;
assert (list_generate 1 10 1 = [1;2;3;4;5;6;7;8;9;10]) ;;
assert (list_generate 1 10 2 = [1;3;5;7;9]) ;;
assert (list_first [42; 43] = 42) ;;
printf "Succes\n" ;;

(** Debut des tests des fonctions auxiliaires sur les ensembles *)
printf "######################################\n" ;;
printf "#    Ensembles : debut des tests     #\n" ;;
printf "######################################\n" ;;
assert (set_add [] 1 = [1]) ;;
assert (set_add [1] 1 = [1]) ;;
assert (list_to_set [1;1;1;2;2;1;2] = [1;2]) ;;
assert (set_intersect [1;2] [2;3] = [2]) ;;
assert (set_intersect [1;2] [3;4] = []) ;;
assert (set_union [1;2;3;4] [3;4;5;6] = [1;2;3;4;5;6]) ;;
printf "Succes\n" ;;


(** Debut des tests de la partie 'Approche naive' *)
open Approche_naive ;;

printf "###########################################\n" ;;
printf "#    Approche naive : debut des tests     #\n" ;;
printf "###########################################\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'sum'\n%!" ;;
assert (sum [] = 0) ;;
assert (sum [1] = 1) ;;
assert (sum [1; 2; 3] = 6) ;;
printf "Succes\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'powerset'\n%!" ;;
assert (powerset [] = [[]]) ;;
assert (powerset [1] = [[]; [1]]) ;;
assert (powerset [1; 2] = [[]; [2]; [1]; [1; 2]]) ;;
assert (powerset [1; 2; 3] = [[]; [3]; [2]; [2; 3]; [1]; [1; 3]; [1; 2]; [1; 2; 3]]) ;;
printf "Succes\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'subset_sum_0'\n%!" ;;
assert (subset_sum_0 [] 42 = (0, [])) ;;
assert (subset_sum_0 [1] 42 = (1, [1])) ;;
assert (subset_sum_0 [42] 1 = (0, [])) ;;
assert (subset_sum_0 [1; 2; 3] 42 = (6, [1; 2; 3])) ;;
assert (subset_sum_0 [1; 2; 3] 6  = (6, [1; 2; 3])) ;;
assert (subset_sum_0 [1; 2; 3] 5  = (5, [2; 3])) ;;
assert (subset_sum_0 [1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1] 6 = (6, [1; 1; 1; 1; 1; 1]));;
assert (
		let (s, e) = subset_sum_0 [42; 666; 007; 24; 12; 30; 7500 ; 9500] 10000 in
		s = 9615 &&
		(List.sort (function x -> function y -> x - y) e) = [7; 12; 24; 30; 42; 9500]
	) ;;
printf "Succes\n" ;;


(** Debut des tests de la partie 'Approche directe' *)
open Approche_plus_directe ;;

printf "##################################################\n" ;;
printf "#    Approche plus directe : debut des tests     #\n" ;;
printf "##################################################\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'get_all_sums'\n%!" ;;
assert (get_all_sums []  = [(0, [])]);;
assert (get_all_sums [1] = [(1, [1]); (0, [])]) ;;
assert ((List.sort (function (s, _) -> function (s', _) -> s - s') (get_all_sums [1; 3; 4]))
		= [(0, []); (1, [1]); (3, [3]); (4, [4]);
			(5, [1; 4]); (7, [3; 4]); (8, [1; 3; 4])] );;
printf "Succes\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'subset_sum_1'\n%!" ;;
assert (subset_sum_1 [] 42 = (0, [])) ;;
assert (subset_sum_1 [1] 42 = (1, [1])) ;;
assert (subset_sum_1 [42] 1 = (0, [])) ;;
assert (subset_sum_1 [1; 2; 3] 42 = (6, [1; 2; 3])) ;;
assert (subset_sum_1 [1; 2; 3] 6  = (6, [1; 2; 3])) ;;
assert (subset_sum_1 [1; 2; 3] 5  = (5, [2; 3])) ;;
assert (
		let (s, e) = subset_sum_1 [42; 666; 007; 24; 12; 30; 7500 ; 9500] 10000 in
		s = 9615 &&
		(List.sort (function x -> function y -> x - y) e) = [7; 12; 24; 30; 42; 9500]
	) ;;
printf "Succes\n" ;;

(** Debut des tests de la partie 'Approche avec nettoyage' *)
open Approche_avec_nettoyage ;;

printf "##################################################\n" ;;
printf "#    Approche avec nettoyage: debut des tests    #\n" ;;
printf "##################################################\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'clean_up'\n%!" ;;
let list_add_empty_sum = function l ->
	List.map (function x -> (x, [])) l
;;
assert ((clean_up (list_add_empty_sum (list_generate 1 100 1)) 90 0.1) =
		
		(list_add_empty_sum (list_reverse [1; 2; 3; 4; 5; 6; 7; 8; 9; 10;
		 			12; 14; 16; 18; 20; 23;
					26; 29; 32; 36; 40; 45;
					50; 56; 62; 69; 76; 84]))
	);;
printf "Succes\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'subset_sum_2'\n%!" ;;
assert (subset_sum_2 0.01 [] 42 = (0, [])) ;;
assert (subset_sum_2 0.01 [1] 42 = (1, [1])) ;;
assert (subset_sum_2 0.01 [42] 1 = (0, [])) ;;
assert (subset_sum_2 0.01 [1; 2; 3] 42 = (6, [1; 2; 3])) ;;
assert (subset_sum_2 0.01 [1; 2; 3] 6  = (6, [1; 2; 3])) ;;
assert (subset_sum_2 0.01 [1; 2; 3] 5  = (5, [2; 3])) ;;
assert (
		let (s, e) = subset_sum_2 0.00001 [42; 666; 007; 24; 12; 30; 7500 ; 9500] 10000 in
		s = 9615 &&
		(List.sort (function x -> function y -> x - y) e) = [7; 12; 24; 30; 42; 9500]
	) ;;
printf "Succes\n" ;;

(** Debut des tests de la partie 'Approche de type diviser pour régner' *)
open Approche_diviser_pour_regner ;;
printf "#######################################################\n" ;;
printf "#    Approche diviser pour régner: debut des tests    #\n" ;;
printf "#######################################################\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'is_feasible'\n%!" ;;
assert (is_feasible 12 [1; 2; 4; 7] [10; 8; 2] = true) ;; (** 4 + 8 == 12 *)
assert (is_feasible 16 [1; 2; 4; 7] [10; 8; 2] = false) ;;
printf "Succes\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'subset_sum_3'\n%!" ;;
assert (subset_sum_3 [] 42 = (0, [])) ;;
assert (subset_sum_3 [1] 42 = (1, [1])) ;;
assert (subset_sum_3 [42] 1 = (0, [])) ;;
assert (subset_sum_3 [1; 2; 3] 42 = (6, [3; 1; 2])) ;;
assert (subset_sum_3 [1; 2; 3] 6  = (6, [3; 1; 2])) ;;
assert (subset_sum_3 [1; 2; 3] 5  = (5, [3; 2])) ;;
assert (
		let (s, e) = subset_sum_3 [42; 666; 007; 24; 12; 30; 7500 ; 9500] 10000 in
		s = 9615 &&
		(List.sort (function x -> function y -> x - y) e) = [7; 12; 24; 30; 42; 9500]
	) ;;
printf "Succes\n" ;;



(** Début de la derniere partie avec des tests aléatoires *)
printf "####################################################################\n" ;;
printf "#    Comparaison des approches et amélioration: debut des tests    #\n" ;;
printf "####################################################################\n" ;;

(** fonction interne qui génère une liste de n entiers aléatoires dans [0, m - 1] *)
let rec gen_random = function n -> function m ->
	if n <= 0 then
		[]
	else
		(Random.int m)::(gen_random (n - 1) m)
;;

(**
 *	génère des ensembles de i, i+1, ... j nombres aléatoires,
 *	et génère un problème SUBSET_SUM_OPT à résoudre.
 *	ce problème est résolu avec chaque approche,
 *	et on teste si les 4 approches trouvent bien le même résultat.
 *)
let rec test_rand = function i -> function j ->
	if i <= j then
		let n = i in
		let m = n * 2 in
		let s = (n * m) / 2 in
		let l = list_to_set (gen_random n m) in
		let (s0, e0) = subset_sum_0 l s in
		let (s1, e1) = subset_sum_1 l s in
		let (_, _) = subset_sum_2 0.01 l s in
		let (s3, e3) = subset_sum_3 l s in
		assert (s0 = s1) ;
		(* assert (s0 = s2) ; pas toujours vrai! *)
		assert (s0 = s3) ;
		test_rand (i + 1) j
;;

printf "----------------------------------------------\n" ;;
printf "Tests des approches avec des listes aléatoires\n%!" ;;
printf "----------------------------------------------\n" ;;
test_rand 1 15 ;;
printf "Succes\n" ;;

printf "----------------------------------------------------\n" ;;
printf "Comparaison des approches avec des listes aléatoires\n%!" ;;
printf "----------------------------------------------------\n" ;;

(** code pour générer les .csv *)
(**
(**
 *	calcul le temps d'execution en 'ms' de chaque algorithme
 * 	pour le meme probleme subset_sum_opt, avec un ensemble
 *	à i élément, puis i + 1 éléments, puis ... puis j éléments.
 *	Un temps '-1' indique que l'approche a fait un dépassement de pile
 *)
let rec time_rand = function i -> function j ->
	if i <= j then
		let n = i in
		let m = n * 2 in
		let s = (n * m) / 2 in
		let l = gen_random n m in
		let time = function subset_sum ->
			try
				let t  = Sys.time () in
				let _ = subset_sum l s in
				int_of_float ((Sys.time () -. t) *. 1000.0)
			with Stack_overflow -> -1
		in
(** code permettant de génerer les .csv *)
		printf "%d, %d, %d, %d, %d\n%!"
			n
			(time subset_sum_0)
			(time subset_sum_1)
			(time (subset_sum_2 0.01))
			(time subset_sum_3) ;
		(time_rand (i + 1) j)
;;

(** test les performances de l'approche avec clean_up *)
let rec time_rand_clean_up = function i -> function j -> function step -> function delta ->
	if i <= j then
		let n = i in
		let m = n * 2 in
		let s = (n * m) / 2 in
		let l = gen_random n m in
		let time = function subset_sum ->
			try
				let t  = Sys.time () in
				let (s', _) = subset_sum l s in
				let dt = int_of_float ((Sys.time () -. t) *. 1000.0) in
				let f  = (float_of_int s) /. (float_of_int s') /. 100.0 in
				(dt, f)
			with Stack_overflow -> (-1, -1.0)
		in
		let (dt, f) = time (subset_sum_2 delta) in
		printf "%d, %d, %f\n%!" n dt f ;
		(time_rand_clean_up (i + step) j step delta)
;;

printf "Les temps sont en millisecondes, -1 indique un dépassement de pile\n" ;;
printf "[Card(E); subset_sum_0; subset_sum_1; subset_sum_2; subset_sum_3]\n";;
time_rand 1 30 ;;
time_rand_clean_up 100 100000 1000 0.000001 ;;
time_rand_clean_up 100 140000 10000 0.000001 ;;
*)
printf "Succes\n" ;;

