<?php

/**
 *  @file
 *  @brief supprimes une permission d'un rôle
 *  @param :
 *  	COOKIE \a PHPSESSID : le cookie de session
 *  	POST \a role_id : l'id du role
 *  	POST \a permission_id : l'id de la permission
 *  @return
 *      - code reponse:
 *                      - 200 : la permission a été supprimé du rôle avec succès
 *                      - 400 : erreur requête
 *                      - 503 : la connection à la base de données a échoué
 */

// /@cond INTERNAL
use Model\ULC\BDD\BDD;
use Model\ULC\BDD\ConnectionException;
use Model\ULC\Utilisateur\Permission;
use Model\ULC\Utilisateur\Utilisateur;

require '../../../../vendor/autoload.php';

if (! isset ( $_GET ['role_id'] ) || ! isset ( $_GET ['permission_id'] )) {
	http_response_code ( 400 );
	echo 'erreur requete';
} else {
	$user = Utilisateur::instance ();
	
	if (! $user->hasPermission ( Permission::$REMOVE_PERMISSION )) {
		http_response_code ( 400 );
		echo "Non autorisé";
	} else {
		try {
			$pdo = BDD::instance ()->getConnection ( "ulc" );
			
			$role_id = filter_input ( INPUT_GET, 'role_id', FILTER_SANITIZE_NUMBER_INT );
			$permission_id = filter_input ( INPUT_GET, 'permission_id', FILTER_SANITIZE_NUMBER_INT );
			
			$stmt = $pdo->prepare ( 'DELETE FROM role_permission WHERE role_id=:role_id AND permission_id=:permission_id' );
			$stmt->bindParam ( ':role_id', $role_id, PDO::PARAM_INT );
			$stmt->bindParam ( ':permission_id', $permission_id, PDO::PARAM_INT );
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