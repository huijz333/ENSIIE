type forme = Point | Cercle of float | Rectangle of float * float ;;
type surface = S of float ;;
let aire =	function f ->
			match f with
				| Point			-> S 0.0
				| Cercle r		-> S (3.14 *. r *. r)
				| Rectangle (l, h)	-> S (l *. h)
		;;

let c1 = Cercle 3.0 ;;
let r1 = Rectangle (4.0, 1.0) ;;

let (++) = function (S x) -> function (S y) -> S (x +. y) ;;

let somme_aire =	function formes ->
				List.fold_left (function acc -> function f -> acc ++ aire f) (S 0.0) formes
			;;

let est_carre =	function f -> match f with
			| Rectangle (l, h) -> l == h
			| _		-> false
		;;

let rec contient_carre =	function formes ->
					match formes with
					| [] 	->	false
					| h::t	->	(est_carre h) || (contient_carre t)
				;;

let rec contient_carre2 =	function formes ->
					List.fold_left	(function b -> function f ->
								b || (est_carre f)
						       	)
							false formes ;;

type couleur = Rouge | Vert | Bleu ;;
type forme_colore = Point | Cercle of float*couleur | Rectangle of float*float*couleur ;;

let couleurs =	function (formes : forme_colore list) ->
			List.fold_left	(function color -> function forme_colore ->
					match forme_colore with
					| Point			-> []
					| Cercle (r, c)		-> c::color
					| Rectangle (l, h, c)	-> c::color
					)
					[] formes
		;;
			
