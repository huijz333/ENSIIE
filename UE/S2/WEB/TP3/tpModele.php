<?php

/** variable global pour debug */
$REQ_MODE       = isset($_GET["request_mode"]) ? $_GET["request_mode"] : 1;
$TABLE_MODE     = isset($_GET["table_mode"]) ? $_GET["table_mode"] : 1;
if ($TABLE_MODE == 1) {
    $EMPLOYE_TABLE = "employe";
    $SERVICE_TABLE = "service";
} else {
    $EMPLOYE_TABLE = "employe2";
    $SERVICE_TABLE = "service2";
}

function listeEmploye() {
    global $REQ_MODE;
    
    if ($REQ_MODE == 1) {
        listeEmploye1();
    } else {
        listeEmploye2();
    }
}

/** affiche la liste des employés avec la méthode '1' vue en tp
 *  (autant de requete qu'il y a d'employés)
 **/
function listeEmploye1()
{
    global $SERVICE_TABLE, $EMPLOYE_TABLE;
    /** recupere la base de données */
    $db = pg_connect("host=pgsql2 user=tpcurseurs dbname=tpcurseurs password=tpcurseurs");
    if (!$db) {
        affiche_erreur("Erreur de connexion à la bdd");
        return ;
    }
    
    
    /** prépare les requetes */
    $prepare_name = "reqES";
    $q = pg_query($db, "SELECT * FROM " . $SERVICE_TABLE . " NATURAL JOIN "
                        . $EMPLOYE_TABLE . " ORDER BY num_service");
    
    /** prepare les variables */
    $nb_employe  = 0;
    $total_coeff = 0;
    $min_coeff   = PHP_INT_MAX;
    $max_coeff   = -PHP_INT_MAX;
    
    /** pour chaque services...*/
    while (($line1 = pg_fetch_array($q)) != NULL) {
        affiche_en_tete_service($line1);
        affiche_employe($line1);
        
        $nb_employe++;
        $coeff = $line1["coeff_salaire"];
        $total_coeff += $coeff;
        if ($coeff > $max_coeff) {
            $max_coeff = $coeff;
        }
        if ($coeff < $min_coeff) {
            $min_coeff = $coeff;
        }
        
        
        while (($line2 = pg_fetch_array($q)) != NULL && $line1["num_service"] == $line2["num_service"]) {
            affiche_employe($line2);
            
            $nb_employe++;
            $coeff = $line2["coeff_salaire"];
            $total_coeff += $coeff;
            if ($coeff > $max_coeff) {
                $max_coeff = $coeff;
            }
            if ($coeff < $min_coeff) {
                $min_coeff = $coeff;
            }
        }
        affiche_pied_service($line1);
    }
    affiche_stats($nb_employe, $total_coeff / $nb_employe, $min_coeff, $max_coeff);
}

/** affiche la liste des employés avec la méthode '2' vue en tp
 *  (autant de requete qu'il y a d'employés)
 **/
function listeEmploye2()
{
    global $SERVICE_TABLE, $EMPLOYE_TABLE;
    
    /** recupere la base de données */
    $db = pg_connect("host=pgsql2 user=tpcurseurs dbname=tpcurseurs password=tpcurseurs");
    if (!$db) {
        affiche_erreur("Erreur de connexion à la bdd");
        return ;
    }
    
    
    /** prépare les requetes */
    $prepare_name = "reqES";
    $services_query = pg_query($db, "SELECT * FROM " . $SERVICE_TABLE);
    pg_prepare($db, $prepare_name, "SELECT * FROM " . $EMPLOYE_TABLE
                    . " WHERE num_service=$1 ORDER BY coeff_salaire");
    
    /** prepare les variables */
    $nb_employe  = 0;
    $total_coeff = 0;
    $min_coeff   = PHP_INT_MAX;
    $max_coeff   = -PHP_INT_MAX;
    
    /** pour chaque services...*/
    while (($service = pg_fetch_array($services_query)) != NULL) {
        $employe_query = pg_execute($db, $prepare_name, array($service["num_service"]));        
        affiche_en_tete_service($service);
        while (($employe = pg_fetch_array($employe_query)) != NULL) {
            affiche_employe($employe);
            
            /** met à jour les variables */
            $nb_employe++;
            $coeff = $employe["coeff_salaire"];
            $total_coeff += $coeff;
            if ($coeff > $max_coeff) {
                $max_coeff = $coeff;
            }
            if ($coeff < $min_coeff) {
                $min_coeff = $coeff;
            }
        }
        affiche_pied_service($service);
    }
    affiche_stats($nb_employe, $total_coeff / $nb_employe, $min_coeff, $max_coeff);
}

?>