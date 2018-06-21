<?php

/**
 *  @file
 *  @brief Marque toutes les notifications comme ayant été lu
 *  @param :
 *      - COOKIE \a PHPSESSID : le cookie de session PHP \ref api/user/account/connect/index.php
 *      - \a id : l'id de la notification à marquer comme étant lue. \ref api/user/notification/get/index.php
 *  @return
 *      - code reponse:
 *                      - 200 : les notifications ont été marqué comme lu
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
} else {
	
	$bdd = BDD::instance ();
	try {
		$pdo = $bdd->getConnection ( "ulc" );
		
		/* prépares la base de données */
		$stmt = $pdo->prepare ( "UPDATE notification SET status = 'seen' WHERE utilisateur_id = :utilisateur_id" );
		/* protège des injections sql */
		$id = $user->getID ();
		$stmt->bindParam ( ':utilisateur_id', $id, PDO::PARAM_INT );
		
		/* execute la requete sécurisé */
		$stmt->execute ();
		
		echo 'OK';
		http_response_code ( 200 );
	} catch ( \Model\ULC\BDD\ConnectionException $e ) {
		http_response_code ( 503 );
		echo "erreur serveur";
	}
}

// /@endcond INTERNAL

?>