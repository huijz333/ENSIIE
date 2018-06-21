<?php

/**
 *  @file
 *  @brief Génère un \a third-party-code permettant de lié un compte League of Legend à un compte utilisateur du site.
 *	@details \ref api/user/game/lol/third-party-code/link/index.php
 *  @param :
 *      - COOKIE \a PHPSESSID : le cookie de session PHP \ref api/user/account/connect/index.php
 *  @return
 *		- le code généré
 *      - code reponse:
 *                      - 200 : le code a été généré avec succès
 *                      - 400 : erreur de la requête (paramètre(s) manquant(s) ou invalide(s))
 */

// /@cond INTERNAL

/* include path */
require '../../../../../../../vendor/autoload.php';

use Model\ULC\Utils;
use Model\ULC\Utilisateur\Utilisateur;

$user = Utilisateur::instance ();

// si l'utilisateur n'est pas connecté
if (! $user->isConnected ()) {
	http_response_code ( 400 );
	echo 'Non connecté.';
	// si la requete est invalide
} else {
	http_response_code ( 200 );
	$code = Utils::random_str ( 32 );
	$_SESSION ['third-party-code'] = $code;
	echo $code;
}

// /@endcond INTERNAL

?>