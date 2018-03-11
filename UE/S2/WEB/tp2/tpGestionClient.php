<?php

include("tpModele.php");
include("tpVue.php");

verif_authent();

enTete("Gestion des clients");

/* recupération du numéro de client entré dans le formulaire de menu */
$numCli = $_POST['numCli'];
// si aucune case n'a été cochée
if(! isset($_POST['choix']) ){
    affiche_erreur("Merci de cocher une case");
}
// si le numéro de client n'a pas été renseigné
elseif ((! isset($numCli)) | $numCli == "" | ! is_numeric($numCli)) {
    affiche_erreur("Vous devez impérativement taper un numéro de client");
} else {
    $tab = get_client($numCli);
    switch ($_POST['choix']) {
        /* Affichage d'un client */
        case "v":
            affiche_info("Consultation du numéro client : $numCli");
            if ($tab[0] == 0) {
                affiche_info("Nom: " . $tab[1] . " , Débit: " . $tab[2]);
            } else {
                affiche_erreur_client($tab[0]);
            }
            break ;

        /* Modification d'un client */
        case "m" :
    	    affiche_info("Modification du client no : $numCli");
            if($tab[0] == 0) {
                    affiche_form_modif($numCli, $tab[1], $tab[2]);
            } else {
                    affiche_erreur_client($tab[0]);
            }
            break ;
        /* Enregistrement d'un achat */
        case "a" :
            affiche_info("Achat du client no : $numCli");
            if($tab[0] == 0) {
                    affiche_form_achat($numCli, $tab[2]);
            } else {
                    affiche_erreur_client($tab[0]);
            }
            break;

        /* Création d'un client */
        case "c" :
            affiche_info("Création du numéro client : $numCli");
            if ($tab[0] == 0) {
                affiche_erreur("Erreur: le client existe deja.");
            } else if ($tab[0] == -1) {
                affiche_form_creation($numCli);
            } else {
                affiche_erreur_client($tab[0]);
            }
            break;

        /* ne devrait jamais se produire... */
        default : 
            affiche_erreur("Cocher une case");
            break;
    }//fin switch
	
}// fin else
	
retour_menu();
pied();

?>



