(**
* File              : 5.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :36 CET
* Last Modified Date: mer. 14 févr. 2018 12:07:44 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

(**
*	@Require: une chaine s, un cadre, et une symetrie (0, 1, 2, ou 3)
*)
let exclamation_sym =	function (s, cadre, sym) ->
				if sym=0 then
					s
				else if sym=1 then
					cadre ^ s
				else if sym=2 then
					s ^ cadre
				else if sym=3 then
					cadre ^ s ^ cadre
				else
					failwith (Printf.sprintf "invalid sym : %d" sym)
			;;


(**
*	@Require: une chaine s, un cadre
*)
let exclamation_cadre =	function (s, cadre) ->
				exclamation_sym (s, cadre, 3)
			;;


(**
*	@Require: une chaine s
*)
let exclamation =	function s ->
				exclamation_cadre (s, "!!")
			;;

Printf.printf "%s\n" (exclamation	"hello world") ;;
Printf.printf "%s\n" (exclamation_cadre	("hello world", " $$ ") ) ;;
Printf.printf "%s\n" (exclamation_sym	("hello world", " !! ", 0) ) ;;
Printf.printf "%s\n" (exclamation_sym	("hello world", " !! ", 1) ) ;;
Printf.printf "%s\n" (exclamation_sym	("hello world", " !! ", 2) ) ;;
Printf.printf "%s\n" (exclamation_sym	("hello world", " !! ", 3) ) ;;
Printf.printf "%s\n" (exclamation_sym	("hello world", " !! ", 42) ) ;;
