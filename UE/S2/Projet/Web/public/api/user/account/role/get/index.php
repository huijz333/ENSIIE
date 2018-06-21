<?php
use Model\ULC\BDD\BDD;
use Model\ULC\BDD\ConnectionException;
use Model\ULC\Utilisateur\Permission;
use Model\ULC\Utilisateur\Utilisateur;

/**
 *
 *  @file
 *  @brief Recupères les roles d'un utilisateur
 * @param
 *        	GET \a pseudo : le pseudo du joueur
 * @return - JSON : les roles
 *         - code reponse:
 *         - 200 : les rôles ont été renvoyé avec succès
 *         - 400 : erreur requête
 *         - 503 : la connection à la base de données a échoué
 */

// /@cond INTERNAL
require '../../../../../../vendor/autoload.php';

if (! isset ( $_GET ['pseudo'] ) || ! isset ( $_GET ['roleName'] )) {
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
			
			$pseudo	= filter_input ( INPUT_GET, 'pseudo',   FILTER_SANITIZE_STRING );
			$name	= filter_input ( INPUT_GET, 'roleName', FILTER_SANITIZE_STRING );
			
			$stmt = $pdo->prepare ( 'INSERT INTO utilisateur_role (utilisateur_id, role_id) SELECT utilisateur_id, role_id FROM (SELECT id as utilisateur_id FROM utilisateur WHERE pseudo=:pseudo) a, (SELECT id as role_id FROM role WHERE name=:name) b ');
			$stmt->bindParam ( ':pseudo', 	$pseudo,	PDO::PARAM_STR );
			$stmt->bindParam ( ':name',  	$name,		PDO::PARAM_STR );
			$stmt->execute ();
			http_response_code ( 200 );
			echo "OK";
		} catch (ConnectionException $e) {
			http_response_code ( 503 );
			echo "Erreur serveur";
		} catch (PDOException $e) {
			http_response_code ( 400 );
			echo "Erreur requete : " . $e;
		}
	}
}

// /@endcode

?>