<?php

/**
 *  @file
 *  @brief Recuperes la liste des écoles
 *	@return
 *		- code reponse:
 *						- 200 : la liste a bien été générée
 						- 503 : erreur de connection à la base de donnée
 */

///@cond INTERNAL

/* include path */
use Model\ULC\BDD\BDD;
require '../../../../vendor/autoload.php'; 

/* on recupere la connection à la pdo */
$bdd = BDD::instance ();

try {
	$pdo = $bdd->getConnection ( "ulc" );
	
	/* prépares la base de données */
	$stmt = $pdo->prepare ( 'SELECT academie, nom, sigle, domaine, ville FROM ecole ORDER BY academie');
	$stmt->execute();
	
	http_response_code ( 200 );
	
	/* on enregistre la liste des écoles */
	echo '{"liste_ecoles":[';
	if ($stmt->rowCount () > 0) {
		$entry = $stmt->fetch ();
		while ( $entry != NULL ) {
			echo '{';
			echo '"academie":"' . $entry ['academie'] . '"';
			echo ',';
			echo '"nom":"' . $entry ['nom'] . '"';
			echo ',';
			echo '"sigle":"' . $entry ['sigle'] . '"';
			echo ',';
			echo '"domaine":"' . $entry ['domaine'] . '"';
			echo ',';
			echo '"ville":"' . $entry ['ville'] . '"';
			echo '}';
			$entry = $stmt->fetch ();
			if ($entry != NULL) {
				echo ',';
			}
		}
	}
	echo ']}';
} catch (\Model\ULC\BDD\ConnectionException $e) {
	http_response_code(503);
	echo "erreur serveur";
}




// /@endcode

?>