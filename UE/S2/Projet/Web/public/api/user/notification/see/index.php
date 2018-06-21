<?php

/**
 *  @file
 *  @brief Marque une notification comme ayant été lu
 *  @param :
 *      - COOKIE \a PHPSESSID : le cookie de session PHP \ref api/user/account/connect/index.php
 *      - \a id : l'id de la notification à marquer comme étant lue. \ref api/user/notification/get/index.php
 *  @return
 *      - code reponse:
 *                      - 200 : la notification a été marqué comme lu
 *                      - 400 : erreur de la requête (paramètre(s) manquant(s) ou invalide(s))
 *						- 401 : non connecté(e)
 *						- 503 : erreur de connection à la base de donnée
 */

// /@cond INTERNAL

/* include path */
use Model\ULC\BDD\BDD;
use Model\ULC\Utilisateur\Utilisateur;

require '../../../../../vendor/autoload.php';

/* on recupere l'utilisateur */
$user = Utilisateur::instance ();
if (! $user->isConnected ()) {
	http_response_code ( 401 );
	echo "Non connecté";
} else if (! isset ( $_POST ['id'] )) {
	http_response_code ( 400 );
	echo "Erreur requête";
} else {
	
	$bdd = BDD::instance ();
	try {
		$pdo = $bdd->getConnection ( "ulc" );
		$stmt = $pdo->prepare ( "UPDATE notification SET status = 'seen' WHERE utilisateur_id = :utilisateur_id AND id = :id" );
		
		$utilisateur_id = $user->getID ();
		$stmt->bindParam ( ':utilisateur_id', $utilisateur_id, PDO::PARAM_INT );
		
		$id = filter_input ( INPUT_POST, 'id', FILTER_SANITIZE_STRING );
		$stmt->bindParam ( ':id', $id, PDO::PARAM_STR );
		
		$stmt->execute ();
		
		http_response_code ( 200 );
		echo 'OK';
	} catch ( \Model\ULC\BDD\ConnectionException $e ) {
		http_response_code ( 503 );
		echo "erreur serveur";
	}
}

// /@endcond INTERNAL

?>