<?php

include("tpModele.php");
include("tpVue.php");

verif_authent();

$numCli=$_POST['numCli'];
$nomCli=$_POST['nomCli'];
$debitInit=$_POST['debitInit'];

enTete("Modification d'un client");

if (create_client($numCli,$nomCli,$debitInit))
{
	affiche_info("Modification enregistrée");
}
else {
	affiche_erreur("Impossible de se connecter");
}
retour_menu();
pied();
?>
