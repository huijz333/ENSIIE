<?php

/**
 *  @file
 *  @brief Supprimes la liaison entre un compte League of Legends et l'utilisateur du site
 *  @param :
 *      - POST \a summonerName : le nom d'invocateur à dé-lier
 *      - COOKIE \a PHPSESSID  : le cookie de session PHP \ref api/user/account/connect/index.php
 *  @return
 *      - code reponse:
 *                      - 200 : l'invocateur a été de-lié avec succès
 *                      - 400 : erreur de la requête (paramètre(s) manquant(s) ou invalide(s))
 *						- 404 : l'invocateur fourni n'était pas lié à l'utilisateur
 */

// /@cond INTERNAL

/* include path */
require '../../../../../../../vendor/autoload.php';

use Model\ULC\Utilisateur\Utilisateur;

$user = Utilisateur::instance ();

if (! $user->isConnected () || ! isset ( $_POST ['summonerName'] )) {
	http_response_code ( 400 );
} else {
	try {
		$user->unlinkAccount ( $_POST ['summonerName'] );
	} catch ( Exception $e ) {
		http_response_code ( 404 );
	}
}

// /@endcond INTERNAL

?>