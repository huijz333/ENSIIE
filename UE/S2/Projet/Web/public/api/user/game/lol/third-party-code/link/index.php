<?php

/**
 *
 *  @file
 *  @brief Lis un compte League of Legends à l'utilisateur connecté du site. \ref api/user/game/lol/third-party-code/generate/index.php
 * @param
 *        	:
 *        	- COOKIE \a PHPSESSID : le cookie de session PHP \ref api/user/account/connect/index.php
 *        	- POST \a summonerName : le nom d'invocateur du compte League of Legends à associer. (e.x: 'PCF toss')
 * @return - le code généré
 *         - code reponse:
 *         - 200 : le code a été généré avec succès
 *         - 400 : erreur de la requête (paramètre(s) manquant(s) ou invalide(s))
 *         - 408: aucun third-party-code n'est défini.
 *         - 409: le compte est déjà lié à un autre utilisateur du site.
 *         - 424: le code entré dans League of Legend n'est pas bon
 */

// /@cond INTERNAL

/* include path */
require '../../../../../../../vendor/autoload.php';

use Model\ULC\LOL\API;
use Model\ULC\Utilisateur\Utilisateur;

$user = Utilisateur::instance ();

// si l'utilisateur n'est pas connecté
if (! $user->isConnected ()) {
	http_response_code ( 400 );
	echo 'Non connecté.';
	// si la requete est invalide
} else if (! isset ( $_POST ['summonerName'] )) {
	http_response_code ( 400 );
	echo "'summonerName' manquant";
	// si le third-party-code n'existe pas (ou à expiré...)
} else if (! isset ( $_SESSION ['third-party-code'] )) {
	http_response_code ( 408 );
	echo 'code expiré.';
	// sinon tout est bien formatté
} else {
	$riot = API::riot ();
	try {
		$summoner = $riot->getSummonerByName ( $_POST ['summonerName'] );
		$code = $riot->getThirdPartyCodeBySummonerId ( $summoner->id );
		if (strcmp ( $_SESSION ['third-party-code'], $code ) != 0) {
			http_response_code ( 424 );
			echo 'code entré invalide';
		} else {
			try {
				$user->linkLolAccount ( $summoner->id );
				http_response_code ( 200 );
				echo 'OK';
			} catch ( Exception $e ) {
				http_response_code ( 409 );
				echo 'Compte déjà lié à un autre utilisateur!';
			}
		}
	} catch ( Exception $e ) {
		http_response_code ( 408 );
		echo 'summonerName invalide';
	}
}
// /@endcond INTERNAL
?>