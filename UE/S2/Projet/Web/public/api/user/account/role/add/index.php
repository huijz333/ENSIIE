<?php
use Model\ULC\BDD\BDD;
use Model\ULC\BDD\ConnectionException;
use Model\ULC\Utilisateur\Permission;
use Model\ULC\Utilisateur\Utilisateur;

/**
 *
 *  @file
 *  @brief Ajoutes un rôle à un utilisateur
 * @param
 *        	COOKIE \a PHPSESSID : le cookie de session
 *        	POST \a utilisateur_id : l'id de l'utilisateur
 *        	POST \a role_id : l'id du role
 * @return - code reponse:
 *         200 : le rôle a été attribué avec succès
 *         400 : erreur requête
 *         401 : non autorisé
 *         409 : le role n'a pas pu être attributé (role déjà attribué)
 *         503 : la connection à la base de données a échoué
 */

// /@cond INTERNAL
require '../../../../../../vendor/autoload.php';

if (! isset ( $_POST ['utilisateur_id'] ) || ! isset ( $_POST ['role_id'] )) {
	http_response_code ( 400 );
	echo 'erreur requete';
} else {
	$user = Utilisateur::instance ();
	
	if (! $user->hasPermission ( Permission::$ASSIGN_ROLE )) {
		http_response_code ( 401 );
		echo "Non autorisé";
	} else {
		try {
			$pdo = BDD::instance ()->getConnection ( "ulc" );
			
			$utilisateur_id = filter_input ( INPUT_POST, 'utilisateur_id', FILTER_SANITIZE_NUMBER_INT );
			$role_id = filter_input ( INPUT_POST, 'role_id', FILTER_SANITIZE_NUMBER_INT );
			
			$stmt = $pdo->prepare ( 'INSERT INTO utilisateur_role (utilisateur_id, role_id) VALUES (:utilisateur_id, :role_id)' );
			$stmt->bindParam ( ':utilisateur_id', $utilisateur_id, PDO::PARAM_INT );
			$stmt->bindParam ( ':role_id', $role_id, PDO::PARAM_INT );
			$stmt->execute ();
			http_response_code ( 200 );
			echo "OK";
		} catch ( ConnectionException $e ) {
			http_response_code ( 503 );
			echo "Erreur serveur";
		} catch ( PDOException $e ) {
			http_response_code ( 400 );
			echo "Erreur requete : " . $e;
		}
	}
}

// /@endcode

?>