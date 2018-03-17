(**
* File              : exo3.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :12 CET
* Last Modified Date: mer. 28 févr. 2018 15:40:30 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**	list_insert
 *	@type   : 'a list -> a -> 'a list
 *	@return : une liste trié contenant 'lst' et 'x'
 *)
let rec list_insert =	function lst ->
				function x ->
					match lst with
					| []	->	[x]
					| h::t	->	if x < h then
								x::h::t
							else
								h::(list_insert t x)
			;;

(**	list_append
 *	@type	: 'a list -> 'a list -> 'a list
 *	@return : la liste concatenenant les 2 listes
 *)
let list_concat =	function l1 ->
				function l2 ->
					List.fold_right	(function x ->
								 function y ->
								 	x::y
							) l1 l2
	;;

(**
 *	list_partition
 *	@return 2 listes qui sont la partition de 'l' selon le predicat 'p'
 *)
let list_partition = 	function p -> function l ->
				let rec partition_rec = function lst -> function left -> function right ->
					match lst with
					| []	->	(left, right)
					| h::t	->	if p(h) then
								partition_rec t (h::left) right
							else
								partition_rec t left (h::right)
				in
				partition_rec l [] []
			;;

(**
 *	list_partition2 : pas d'appels récursifs => pas de stackoverflow
 *)
let list_partition2 = 	function p ->
				List.fold_left (function (left, right) -> function x ->
							if p(x) then
								(x::left, right)
							else
								(left, x::right)
						) ([], [])
			;;
(**
 *	list_quicksort
 *)
let rec list_quicksort = function l ->
				match l with
				| []	-> []
				| h::t  -> 	let (left, right) = list_partition2 (function x -> x < h) t in
						let l1 = list_quicksort left in
						let l2 = list_quicksort right in
						l1@(h::l2)
			;;

let lst = [8;1;2;0;6;7;8;9;5;132;6;65] ;;
let (l, r) = list_partition (function x -> x < 5) lst ;;
let s = list_quicksort lst ;;








