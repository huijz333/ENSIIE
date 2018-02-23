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
}
else{	
    switch ($_POST['choix'])
    {
        /* Affichage d'un client */
        case "v":
	    affiche_info("Consultation du numéro client : $numCli");
	    $tab=get_client($numCli);
		
	/*
	  Compléter pour afficher les informations demandées ou le cas d'erreur approprié
	*/
        break;

        /* Modification d'un client */
        case "m" :
	    affiche_info("Modification du client no : $numCli");
        $clt = get_client($numCli);
        if ($clt[0] == 0) {
            affiche_form_modif($numCli, $clt[1], $clt[2]);
        } else {
            print "Error 'm' " . $clt[0] . "\n";
        }
	    /* Compléter pour faire appel à la fonction affiche_form_modif() */
        break;

        /* Enregistrement d'un achat */
        case "a" :
	    affiche_info("Achat du client no : $numCli");
        $clt = get_client($numCli);
        if ($clt[0] == 0) {
            affiche_form_achat($numCli, 42); /** le montant va ici */
        } else {
            print "Error 'a' " . $clt[0] . "\n";
        }
	    /*Compléter : récupérer les infos sur le client et appeler la fonction affiche_form_achat() */
        break;

        /* Création d'un client */
        case "c" :
            affiche_info("Création du numéro client : $numCli");
	    /*Compléter : vérifier si le client existe, et appeler la fonction affiche_form_creation() si ce n'est pas le cas */
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



