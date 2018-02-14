(**
* File              : 4.ml
* Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
* Date              :10 CET
* Last Modified Date: mer. 14 févr. 2018 11:53:52 CET
* Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>
*)

let max2 = function (x, y)	-> if x > y then x else y ;;
let max3 = function (x, y, z)	-> max2 (max2 (x, y), z) ;;

Printf.printf "%d\n" (max3 ( (-42), 0, 42)) ;;
