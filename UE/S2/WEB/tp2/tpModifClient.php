<?php

include("tpModele.php");
include("tpVue.php");

verif_authent();

$numCli=$_POST['numCli'];
$nomMod=$_POST['nomMod'];
$debitMod=$_POST['debitMod'];

enTete("Modification d'un client");


if (set_client($numCli, $nomMod, $debitMod)) {
	print "Client modifié.\n";
} else {
	print "Erreur lors de la modification du client.\n";
}
/*
 Faire appel à la fonction set_client, et afficher un message indiquant que la mise à jour a été effectuée, ou un message d'erreur
*/


retour_menu();
pied();
?>


