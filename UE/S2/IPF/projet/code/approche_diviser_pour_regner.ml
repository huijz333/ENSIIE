(**
* File              : approche_diviser_pour_regner.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :42 CET
* Last Modified Date: mar. 27 mars 2018 17:57:17 CEST
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
 *	6. Approche de type diviser pour r�gner:
 *
 *	R�ponses aux questions 8, 9, 10
 *)

open Approche_plus_directe ;;
open Ensembles ;;
open Listes ;;



(** un type utile qui permet de savoir si l'on a r�ussi � atteindre 's', ou si on l'a depass� *)
(** voir Question 8. du rapport qui d�tail l'algorithme *)
type is_feasible_result = TOO_SMALL | REACHED | TOO_BIG ;;

(**
 *	Fonction 'is_feasible'
 *
 *	@names	:	s   -> l1       -> l2       -> b
 *	@param  :	int -> int list -> int list -> bool
 *	@return :	Renvoie true si 's' s'�crit comme la somme d'un �l�ment
 *			de 'l1' et d'un �l�ment de 'l2', faux sinon
 *)
let rec is_feasible =	function s -> function l1 -> function l2 ->
				(**
				 *	fonction interne qui test si l'on peut forme 's' en sommant
				 *	l'entier 'x' avec un entier de la liste 'l'.
				 *	On utilise le fait que 'l1' est croissante ici.
				 *)
				let rec test_sum =	function l -> function y ->
								match l with
								| []	-> 	TOO_SMALL
								| x::t	->	if x + y = s then
											REACHED
										else if x + y > s then
											TOO_BIG
										else
											test_sum t y
							in
				(** pour chaque �l�ment de 'l2'(du plus grand au plus petit) *)
				match l2 with
				| []	->	false
				| y::t	->	if y > s then
							is_feasible s l1 t
						else
							let r = test_sum l1 y in
							(
							 match r with
							 | TOO_SMALL	-> false
							 | REACHED	-> true
							 | TOO_BIG	-> is_feasible s l1 t
							)
			;;

(**
 *	Fonction 'best_feasible'
 *
 *	@names	:	s   -> l1       -> l2       -> s'
 *	@param  :	int -> int list -> int list -> int
 *	@return :	Renvoie le plus entier s', qui peut s'�crire comme la
 *			somme d'un �l�ment de de 'l1' et d'un �l�ment de 'l2',
 *			tel que s' < s, ou -1 si un tel s' n'existe pas.
 *)
let rec best_feasible =	function s -> function l1 -> function l2 -> s + 1;;


(**
 *	Fonction 'subset_sum_3'
 *
 *	@param	:	int list -> int -> int
 *	@return :	le r�sultat du probl�me SUBSET-SUM-OPT
 *)
let rec subset_sum_3 =	function l -> function s ->
				let (l1, l2) = list_split l in
				let s1 = get_all_sums l1 in
				let s2 = get_all_sums l1 in
				best_feasible s s1 s2
			;;


