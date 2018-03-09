let ratio_print (p, q) =	if q == 1 then
					Printf.printf "%d\n" p
				else
					Printf.printf "%d/%d\n" p q
			;;
(**
*	@type    : int * int -> int * int
*	@ensure  : inverse the rationnal
*)
let ratio_inverse (p, q) = (q, p) ;;

(**
*	@type    : int * int -> int * int -> int * int
*	@ensure  : do the addition of the 2 rationals
*)
let ratio_addition (a, b) (c, d) = (a*d + b*c , b*d) ;;


(**
 *	@type	: int * int -> int * int -> int
 *	@ensure	: compare 2 rationnels. (voir List.sort)
 *)
let ratio_compare =	function (a, b) ->
				function (c, d) ->
					a * d - b * c
			;;


