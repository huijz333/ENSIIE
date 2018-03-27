(**
* File              : tests.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :31 CET
* Last Modified Date: mar. 27 mars 2018 21:41:30 CEST
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


(** Debut des tests des fonctions auxiliaires sur les ensembles *)
printf "######################################\n" ;;
printf "#    Ensembles : debut des tests     #\n" ;;
printf "######################################\n" ;;


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
assert (subset_sum_0 [] 42 = 0) ;;
assert (subset_sum_0 [1] 42 = 1) ;;
assert (subset_sum_0 [42] 1 = 0) ;;
assert (subset_sum_0 [1; 2; 3] 42 = 6) ;;
assert (subset_sum_0 [1; 2; 3] 6 = 6) ;;
assert (subset_sum_0 [1; 2; 3] 5 = 5) ;;
assert (subset_sum_0 [1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1] 6 = 6);;
assert (subset_sum_0 [42; 666; 007; 24; 12; 30; 31; 27; 28; 1; 2; 3; 5000; 7500 ; 9500] 10000 = 9707) ;;
printf "Succes\n" ;;


(** Debut des tests de la partie 'Approche directe' *)
open Approche_plus_directe ;;

printf "##################################################\n" ;;
printf "#    Approche plus directe : debut des tests     #\n" ;;
printf "##################################################\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'get_all_sums'\n%!" ;;
assert (get_all_sums [] = [0]);;
assert (get_all_sums [1] = [0; 1]) ;;
assert (get_all_sums [1; 1] = [0; 1; 2]);;
assert (List.sort (function x -> function y -> x - y)
	(get_all_sums [1; 3; 4]) = [0; 1; 3; 4; 5; 7; 8]);;
printf "Succes\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'subset_sum_1'\n%!" ;;
assert (subset_sum_1 [] 42 = 0) ;;
assert (subset_sum_1 [1] 42 = 1) ;;
assert (subset_sum_1 [42] 1 = 0) ;;
assert (subset_sum_1 [1; 2; 3] 42 = 6) ;;
assert (subset_sum_1 [1; 2; 3] 6 = 6) ;;
assert (subset_sum_1 [1; 2; 3] 5 = 5) ;;
assert (subset_sum_1 [1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1] 6 = 6);;
assert (subset_sum_1 [42; 666; 007; 24; 12; 30; 31; 27; 28; 1; 2; 3; 5000; 7500 ; 9500] 10000 = 9707) ;;
printf "Succes\n" ;;

(** Debut des tests de la partie 'Approche avec nettoyage' *)
open Approche_avec_nettoyage ;;

printf "##################################################\n" ;;
printf "#    Approche avec nettoyage: debut des tests    #\n" ;;
printf "##################################################\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'clean_up'\n%!" ;;
assert ((clean_up (list_generate 1 100 1) 90 0.1) = [1; 2; 3; 4; 5; 6; 7; 8; 9; 10; 12; 14; 16; 18; 20; 23; 26; 29; 32; 36; 40; 45; 50; 56; 62; 69; 76; 84] );;
printf "Succes\n" ;;

printf "-------------------------------------------\n" ;;
printf "Tests de la fonction 'subset_sum_2'\n%!" ;;
assert (subset_sum_2 0.01 [] 42 = 0) ;;
assert (subset_sum_2 0.01 [1] 42 = 1) ;;
assert (subset_sum_2 0.01 [42] 1 = 0) ;;
assert (subset_sum_2 0.01 [1; 2; 3] 42 = 6) ;;
assert (subset_sum_2 0.01 [1; 2; 3] 6 = 6) ;;
assert (subset_sum_2 0.01 [1; 2; 3] 5 = 5) ;;
assert (subset_sum_2 0.01 [1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1] 6 = 6);;

assert (subset_sum_2 0.01 [1 ; 2 ; 3 ; 1 ; 2 ; 3 ; 42; 666; 007] 712 = 708);;
assert (subset_sum_2 0.001 [1 ; 2 ; 3 ; 1 ; 2 ; 3 ; 42; 666; 007] 712 = 712);;
assert (subset_sum_2 0.0001 [1 ; 2 ; 3 ; 1 ; 2 ; 3 ; 42; 666; 007] 712 = 712) ;;
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
assert (subset_sum_3 [] 42 = 0) ;;
assert (subset_sum_3 [1] 42 = 1) ;;
assert (subset_sum_3 [42] 1 = 0) ;;
assert (subset_sum_3 [1; 2; 3] 42 = 6) ;;
assert (subset_sum_3 [1; 2; 3] 6 = 6) ;;
assert (subset_sum_3 [1; 2; 3] 5 = 5) ;;
assert (subset_sum_3 [1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1] 6 = 6);;
assert (subset_sum_3 [42; 666; 007; 24; 12; 30; 31; 27; 28; 1; 2; 3; 5000; 7500 ; 9500] 10000 = 9707) ;;
printf "Succes\n" ;;



