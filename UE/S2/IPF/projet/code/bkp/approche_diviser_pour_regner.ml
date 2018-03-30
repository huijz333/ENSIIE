(**
* File              : approche_diviser_pour_regner.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :42 CET
* Last Modified Date: mer. 28 mars 2018 16:34:40 CEST
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
 *	6. Approche de type diviser pour régner:
 *
 *	Réponses aux questions 8, 9, 10
 *)

open Approche_plus_directe ;;
open Ensembles ;;
open Listes ;;



(** un type utile qui permet de savoir si l'on a réussi à atteindre 's', ou si on l'a depassé *)
(** voir Question 8. du rapport qui détail l'algorithme *)
type is_feasible_result = TOO_SMALL | REACHED | TOO_BIG ;;

(**
 *	Fonction 'is_feasible'
 *
 *	@names	:	s   -> l1       -> l2       -> b
 *	@param  :	int -> int list -> int list -> bool
 *	@return :	Renvoie true si 's' s'écrit comme la somme d'un élément
 *			de 'l1' et d'un élément de 'l2', faux sinon
 *)
let is_feasible = function s -> function l1 -> function l2 ->
	(**
	 *	fonction interne qui test si l'on peut forme 's' en sommant
	 *	l'entier 'x' avec un entier de la liste 'l'.
	 *	On utilise le fait que 'l1' est croissante ici.
	 *)
	let rec test_sum = function l -> function y ->
		match l with
		| []	-> 	TOO_SMALL
		| x::t	->	if x + y = s then
					REACHED
				else if x + y > s then
					TOO_BIG
				else
					test_sum t y
	in
	(** pour chaque élément de 'l2'(du plus grand au plus petit) *)
	let rec is_feasible_rec = function l2_tail ->
		match l2_tail with
		| []	->	false
		| y::t	->	(
					match (test_sum l1 y) with
					 | TOO_SMALL	-> false
					 | REACHED	-> true
					 | TOO_BIG	-> is_feasible_rec t
				)
	in
	is_feasible_rec l2
;;

(**
 *	Fonction 'best_feasible'
 *
 *	@names	:	s   -> l1       -> l2       -> s'
 *	@param  :	int -> int list -> int list -> int
 *	@return :	Renvoie le plus entier s', qui peut s'écrire comme la
 *			somme d'un élément de de 'l1' et d'un élément de 'l2',
 *			tel que s' < s, ou -1 si un tel s' n'existe pas.
 *)
let best_feasible =	function s -> function l1 -> function l2 ->
	(**
	 *	fonction interne qui test si l'on peut forme 's' en sommant
	 *	l'entier 'y' avec un entier de la liste 'l'.
	 *	On utilise le fait que 'l1' est croissante ici.
	 *)
	let rec test_sum = function l -> function y -> function s' ->
		match l with
		| []	-> 	(TOO_SMALL, s')
		| x::t	->	if x + y = s then
					(REACHED, s)
				else if x + y > s then
					(TOO_BIG, s')
				else
					test_sum t y (max (x + y) s')
	in
	(** pour chaque élément de 'l2'(du plus grand au plus petit) *)
	let rec best_feasible_rec = function l2_tail -> function s' ->
		match l2_tail with
		| []	->	s'
		| y::t	->	(
					match (test_sum l1 y s') with
					 | (TOO_SMALL, s'')	-> s''
					 | (REACHED, _)		-> s
					 | (TOO_BIG, s'')	-> best_feasible_rec t s''
				)
	in
	best_feasible_rec l2 (-1)
;;



(**
 *	Fonction 'subset_sum_3'
 *
 *	@param	:	int list -> int -> int
 *	@return :	le résultat du problème SUBSET-SUM-OPT
 *)
let rec subset_sum_3 =	function l -> function s ->
				(**
				 *	N.B : get_all_sums renvoie les sommes accessibles
				 *	dans l'ordre croissant, d'où le 'list_reverse'
				 *)
				let (l1, l2) = list_split l in
				let (s1, s2) = (get_all_sums l1, get_all_sums l2) in
				best_feasible s s1 (list_reverse s2)
			;;


