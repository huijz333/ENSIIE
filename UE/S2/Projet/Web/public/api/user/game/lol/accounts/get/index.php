<?php

/**
 *  @file
 *  @brief Renvoie la liste des comptes League of Legend associé à un utilisateur du site
 *  @param :
 *      - GET \a pseudo : le pseudo de l'utilisateur du site
 *  @return
 *		- la liste des comptes League of Legend lié à l'utilisateur.
 *      - code reponse:
 *                      - 200 : l'utilisateur a été enregistré avec succès
 *                      - 401 : erreur de la requête (paramètre(s) manquant(s) ou invalide(s))
 *                      - 503 : erreur connection à la base de données
 */

// /@cond INTERNAL

/* include path */
use Model\ULC\BDD\BDD;
use Model\ULC\BDD\ConnectionException;
use Model\ULC\LOL\API;
use Model\ULC\Utilisateur\Utilisateur;

require ('../../../../../../../vendor/autoload.php');

$user = Utilisateur::instance ();

if (! isset ( $_GET ['pseudo'] )) {
	http_response_code ( 400 );
	echo 'erreur requete.';
} else {
	try {
		$pseudo = filter_input ( INPUT_GET, 'pseudo', FILTER_SANITIZE_STRING );
		
		$pdo = BDD::instance ()->getConnection ( "ulc" );
		
		$riot = API::riot ();
		
		$stmt = $pdo->prepare ( "SELECT * FROM utilisateur_lol JOIN utilisateur ON utilisateur_id=id WHERE pseudo=:pseudo" );
		$stmt->bindParam ( ':pseudo', $pseudo, PDO::PARAM_STR );
		$stmt->execute ();
		$summoners = array ();
		
		while ( ($entry = $stmt->fetch ()) != NULL ) {
			array_push ( $summoners, $riot->getSummoner ( $entry ['summoner_id'] ) );
		}
		
		http_response_code ( 200 );
		echo json_encode ( $summoners );
		
	} catch ( ConnectionException $e ) {
		http_response_code ( 503 );
		echo 'erreur serveur';
	}
}

// /@endcond INTERNAL

?>