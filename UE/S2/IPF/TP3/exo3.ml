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

(**	list_merge
 *	@type   : 'a list -> 'a list -> 'a list
 *	@return : une liste trié contenant les 2 listes triés passés en parametre
 *)
let rec list_merge =	function l1 ->
				function l2 ->
					match (l1, l2) with
					| ([],     [])		-> 	[]
					| (r1,     [])		->	r1
					| ([],     r2)		->	r2
					| (h1::t1, h2::t2)	->	if h1 < h2 then
										h1::(list_merge t1 (h2::t2))
									else
										h2::(list_merge t2 (h1::t1))
			;;
									
