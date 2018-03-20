<?php

function enTete()
{
    print "<!DOCTYPE html>\n";
    print "<html>\n";
    print " <head>\n";
    print "     <meta charset=\"utf-8\" />\n";
    print "     <title>Liste des services et de leurs employés.</title>\n";
    print "     <link rel=\"stylesheet\" href=\"tpStyle.css\"/>\n";
    print " </head>\n";
    print "<body>\n";
}

function aideURLGet() {
    affiche("       <hr>");
    affiche("       Cette page prends 2 variables 'GET' en paramètre.");
    affiche("       exemple:");
    affiche("       - pgsql.ensiie.fr/~romain.pereira/TP3/index.php?request_mode=1&table_mode=1");
    affiche("       - pgsql.ensiie.fr/~romain.pereira/TP3/index.php?request_mode=1&table_mode=2");
    affiche("       \n");
}

function formatEmploye() {
    affiche("       <hr>");
    affiche("       Format des employés: [nom_employe] [num_employe] [coeff_salaire]");
    affiche("       \n");
    affiche("       <hr>");
}

function affiche($s) {
    print $s . "</br>\n";
}

function affiche_stats($nb_employe, $mean_coeff, $min_coeff, $max_coeff) {
    affiche("       [nb_employe] [mean_coeff] [min_coeff] [max_coeff]");
    affiche("       $nb_employe | $mean_coeff | $min_coeff | $max_coeff");
}

function affiche_en_tete_service($service) {
    affiche("       <b>" . $service["nom_service"] . "</b>");
}

function affiche_pied_service($service) {
    affiche("       \n");
    affiche("       <hr>");
}

function affiche_employe($employe) {
    affiche("           " . $employe["nom_employe"] . " " . $employe["num_employe"]
            . " " . $employe["coeff_salaire"]);
}

function affiche_erreur($str) {
    echo '<p class="erreur">'.$str.'</p>';
}

function pied()
{
    print " </body>\n";
    print "</html>\n";
    /* Compléter cette fonction afin qu'elle ferme les balises body et html */
}

?>
