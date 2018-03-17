(**
 *	3. Approche Naive:
 *
 *	R�ponses aux questions 1, 2, 3
 *)

(**
 *	Fonction 'sum'
 *
 *	@param  : int list -> int
 *	@return : la somme des �l�ments de la liste d'entier
 *)
let sum =	function l -> List.fold_left (function s -> function x -> s + x) 0 l ;;
