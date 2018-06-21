<?php

/**
 *  @file
 *  @brief Renvoie la liste de toutes les permissions du joueur
 *  @param :
 *  	GET \a pseudo : le pseudo du joueur
 *  @return
 *      - JSON : les permissions
 *      - code reponse:
 *                      - 200 : les permissions ont été récupéré avec succès
 *                      - 400 : requete mal formatté (parametre manquant)
 *                      - 503 : la connection à la base de données a échoué
 */

// /@cond INTERNAL

/* include path */
use Model\ULC\BDD\BDD;
use Model\ULC\BDD\ConnectionException;

require '../../../../../../vendor/autoload.php';

if (isset ( $_GET ['pseudo'] )) {
	
	/* recuperes les données de l'utilisateur */
	try {
		$pdo = BDD::instance ()->getConnection ( "ulc" );
		
		$pseudo = filter_input ( INPUT_GET, 'pseudo', FILTER_SANITIZE_STRING );
		
		$permissions = array ();
		$stmt = $pdo->prepare ( 'SELECT DISTINCT permission_id FROM role_permission JOIN utilisateur_role ON role_permission.role_id = utilisateur_role.role_id JOIN utilisateur ON utilisateur.id=utilisateur_id WHERE pseudo = :pseudo ;' );
		$stmt->bindParam ( ':pseudo', $pseudo, PDO::PARAM_STR );
		$stmt->execute ();
		while ( ($entry = $stmt->fetch ()) != NULL ) {
			array_push ( $permissions, $entry ['permission_id'] );
		}
		http_response_code ( 200 );
		echo json_encode ( $permissions );
	} catch ( ConnectionException $exception ) {
		http_response_code ( 503 );
	}
} else {
	http_response_code ( 400 );
	echo 'Pseudo manquant';
}
// /@endcode

?>