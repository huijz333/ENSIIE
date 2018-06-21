<?php

/**
 *  @file
 *  @brief Renvoie la liste de tous les roles existants au format JSON, avec un tableau contenant l'ID des permissions
 *  @param :
 *  @return
 *      - JSON : les roles
 *      - code reponse:
 *                      - 200 : les roles ont été généré avec succès
 *                      - 503 : base de données innaccessible
 */

// /@cond INTERNAL

/* include path */

use Model\ULC\BDD\BDD;
use Model\ULC\BDD\ConnectionException;

require '../../../../vendor/autoload.php';


/* recuperes les données de l'utilisateur */
try {
	$pdo = BDD::instance ()->getConnection ( "ulc" );
	
	$stmt = $pdo->prepare ( 'SELECT role.id AS id, role.name, permission.id AS permission_id FROM role JOIN role_permission ON role.id=role_permission.role_id JOIN permission ON permission.id=permission_id ORDER BY role.id ;' );
	$stmt->execute ();
	$entry = $stmt->fetch ();
	$roles = array ();
	while ($entry != NULL) {
		$id = $entry['id'];
		$name   = $entry['name'];
		$permissions = array ();
		while ($entry['id'] == $id) {
			array_push ( $permissions, $entry['permission_id'] );
			$entry = $stmt->fetch ();
		}
		array_push ( $roles, array (
				"id" => $id,
				"name" => $name,
				"permissions" => $permissions
		) );
	}
	http_response_code(200);
	echo json_encode ( $roles );
} catch (ConnectionException $exception) {
	http_response_code(503);
}


// /@endcode

?>