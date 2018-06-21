<?php

/**
 *  @file
 *  @brief Ajoutes une permission à un rôle
 *  @param :
 *  	COOKIE \a PHPSESSID : le cookie de session
 *  	POST \a role_id : l'id du role
 *  	POST \a permission_id : l'id de la permission
 *  @return
 *      - code reponse:
 *                      - 200 : la permission a été ajouté au rôle avec succès
 *                      - 400 : erreur requête
 *                      - 503 : la connection à la base de données a échoué
 */

// /@cond INTERNAL
use Model\ULC\BDD\BDD;
use Model\ULC\BDD\ConnectionException;
use Model\ULC\Utilisateur\Permission;
use Model\ULC\Utilisateur\Utilisateur;

require '../../../../vendor/autoload.php';

if (! isset ( $_POST ['role_id'] ) || ! isset ( $_POST ['permission_id'] )) {
	http_response_code ( 400 );
	echo 'erreur requete';
} else {	
	$user = Utilisateur::instance ();
	
	if (! $user->hasPermission ( Permission::$ADD_PERMISSION )) {
		http_response_code ( 400 );
		echo "Non autorisé";
	} else {
		try {
			$pdo = BDD::instance ()->POSTConnection ( "ulc" );
			
			$role_id		= filter_input ( INPUT_POST, 'role_id',			FILTER_SANITIZE_NUMBER_INT );
			$permission_id	= filter_input ( INPUT_POST, 'permission_id',	FILTER_SANITIZE_NUMBER_INT );
			
			$stmt = $pdo->prepare ( 'INSERT INTO role_permission (role_id, permission_id) VALUES (:role_id, :permission_id)' );
			$stmt->bindParam ( ':role_id',			$role_id,			PDO::PARAM_INT );
			$stmt->bindParam ( ':permission_id',	$permission_id,		PDO::PARAM_INT );
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