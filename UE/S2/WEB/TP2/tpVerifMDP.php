<?php

include("tpVue.php");
include("tpModele.php");


//recuperer la valeur saisie dans le champ "mdp" 
$mdp = $_POST['mdp']; 

enTete("Vérification du mot de passe");


/*
  Si le mot de passe entré est vide ou faux (cf. fonction verif_mdp), afficher une erreur et un lien vers tpConnexion.php
 
  S'il est bon, rediriger vers la page index.php
*/

if (verif_mdp($mdp)) {
	header('Location: index.php');
} else {
	print "une erreur\n" ;
	header('Location: index.php');
}


pied();

?>

