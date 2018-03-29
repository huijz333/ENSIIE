(**
* File              : approche_diviser_pour_regner.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :42 CET
* Last Modified Date: mer. 28 mars 2018 17:05:04 CEST
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
 *	6. Approche de type diviser pour régner:
 *
 *	Réponses aux questions 8, 9, 10
 *)
open Ensembles ;;
open Listes ;;
open Approche_avec_nettoyage ;;
open Approche_plus_directe ;;

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
	let rec test_sum = function l1 -> function s2 ->
		match l1 with
		| []		-> 	TOO_SMALL
		| s1::t1	->	if s1 + s2 = s then
						REACHED
					else if s1 + s2 > s then
						TOO_BIG
					else
						test_sum t1 s2
	in
	(** pour chaque élément de 'l2'(du plus grand au plus petit) *)
	let rec is_feasible_rec = function l2_tail ->
		match l2_tail with
		| []		->	false
		| s2::t2	->	(
					match (test_sum l1 s2) with
					 | TOO_SMALL	-> false
					 | REACHED	-> true
					 | TOO_BIG	-> is_feasible_rec t2
				)
	in
	is_feasible_rec l2
;;

(**
 *	Fonction 'best_feasible'
 *
 *	@names	:	s   -> l1       -> l2       -> s'
 *	@param  :	int -> (int * int list) list -> (int * int list) list -> int * int list
 *	@return :	Renvoie le plus entier s', qui peut s'écrire comme la
 *			somme d'un élément de de 'l1' et d'un élément de 'l2',
 *			tel que s' < s, ou -1 si un tel s' n'existe pas.
 *)
let best_feasible = function s -> function l1 -> function l2 ->
	(**
	 *	fonction interne qui test si l'on peut forme 's' en sommant
	 *	l'entier 'y' avec un entier de la liste 'l'.
	 *	On utilise le fait que 'l1' est croissante ici.
	 *)
	let rec test_sum = function l1 -> function (s2, e2) -> function (s', e') ->
		match l1 with
		| []		->	(TOO_SMALL, (s', e'))
		| (s1, e1)::t1	->	if s1 + s2 = s then
						(REACHED, (s, e1@e2))
					else if s1 + s2 > s then
						(TOO_BIG, (s', e'))
					else
						if (s1 + s2) < s' then
							test_sum t1 (s2, e2) (s', e')
						else
							test_sum t1 (s2, e2) (s1+s2, e1@e2)
	in
	(** pour chaque élément de 'l2'(du plus grand au plus petit) *)
	let rec best_feasible_rec = function l2_tail -> function (s', e') ->
		match l2_tail with
		| []		->	(s', e')
		| (s2, e2)::t2	->	(
						match (test_sum l1 (s2, e2) (s', e')) with
						| (TOO_BIG, r)	-> best_feasible_rec t2 r
						| (_, r)	-> r
					)
	in
	best_feasible_rec l2 (0, [])
;;


(**
 *	Fonction 'subset_sum_3'
 *
 *	@param	:	int list -> int -> int
 *	@return :	le résultat du problème SUBSET-SUM-OPT
 *)
let rec subset_sum_3 = function l -> function s ->
	(**
	 *	N.B : get_all_sums renvoie les sommes accessibles
	 *	dans l'ordre croissant, d'où le 'list_reverse'
	 *)
	let (l1, l2) = list_split l in
	let (s1, s2) = (get_all_sums l1, get_all_sums l2) in
	let asc = function (s', _) -> function (s'', _) -> s' - s'' in
	let dsc = function (s', _) -> function (s'', _) -> s'' - s' in
	best_feasible s (List.sort asc s1) (List.sort dsc s2)
;;
