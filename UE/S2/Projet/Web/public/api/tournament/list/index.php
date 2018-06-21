<?php

/**
 *  @file
 *  @brief Recuperes la liste des tournois
 *	@return
 *		- code reponse:
 *						- 200 : la liste a bien été générée
 *						- 400 : erreur requête (paramètre(s) manquant(s))
 - 500 : erreur de connection à la base de donnée
 */

// /@cond INTERNAL

/* include path */
use Model\ULC\BDD\BDD;
require '../../../../vendor/autoload.php';

/* on recupere la connection à la pdo */
$bdd = BDD::instance ();

try {
	$pdo = $bdd->getConnection ( "ulc" );
	
	/* prépares la base de données */
	$stmt = $pdo->prepare ( 'SELECT nom,description,jeu,debut_inscriptions,fin_inscriptions FROM tournoi' );
	$stmt->execute ();
	
	http_response_code ( 200 );
	
	/* on enregistre la liste des tournois */
	echo '{"liste_tournois":[';
	if ($stmt->rowCount () > 0) {
		$entry = $stmt->fetch ();
		while ( $entry != NULL ) {
			echo '{';
			echo '"nom":"' . $entry ['nom'] . '"';
			echo ',';
			echo '"description":"' . $entry ['description'] . '"';
			echo ',';
			echo '"jeu":"' . $entry ['jeu'] . '"';
			echo ',';
			echo '"debut_inscriptions":"' . $entry ['debut_inscriptions'] . '"';
			echo ',';
			echo '"fin_inscriptions":"' . $entry ['fin_inscriptions'] . '"';
			echo '}';
			$entry = $stmt->fetch ();
			if ($entry != NULL) {
				echo ',';
			}
		}
	}
	echo ']}';
} catch ( \Model\ULC\BDD\ConnectionException $e ) {
	http_response_code ( 503 );
	echo "erreur serveur";
}

// /@endcode

?>