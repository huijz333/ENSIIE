<?php

/**
 *  @file
 *  @brief Recuperes la liste des joueurs
 *	@return
 *		- code reponse:
 *						- 200 : la liste a bien été générée
 *						- 400 : erreur requête (paramètre(s) manquant(s))
 *						- 503 : erreur de connection à la base de donnée
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
	$stmt = $pdo->prepare ( 'SELECT pseudo, mail FROM Utilisateur' );
	$stmt->execute ();
	
	http_response_code ( 200 );
	
	/* on enregistre la liste des joueurs */
	echo '{"liste_joueurs":[';
	if ($stmt->rowCount () > 0) {
		$entry = $stmt->fetch ();
		while ( $entry != NULL ) {
			echo '{';
			echo '"pseudo":"' . $entry ['pseudo'] . '"';
			echo ',';
			echo '"mail":"' . $entry ['mail'] . '"';
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