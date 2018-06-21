<?php

/**
 *  @file
 *	@brief Modifie le mot de passe de l'utilisateur à partir d'un \a token généré au préalable.
 *	@param :
 *		- POST \a token : un token généré via \ref api/user/account/password/reset/index.php
 *		- POST \a mail
 *		- POST \a pass
 *	@return
 *		- code reponse:
 *						- 200 : le mot de passe a été modifié avec succès
 *						- 400 : erreur de la requête (paramètre(s) manquant(s) ou invalide(s))
 *						- 401 : token incorrect
 *						- 503 : erreur serveur (accès à la base de donnée)
 */

// /@cond INTERNAL

/* include path */
use Model\ULC\Utilisateur\Utilisateur;

require '../../../../../../vendor/autoload.php';

// si l'utilisateur n'est pas connecté
if (! isset ( $_POST ['mail'] ) || ! isset ( $_POST ['token'] ) || ! isset ( $_POST ['pass'] )) {
	http_response_code ( 400 );
	echo "Requete invalide";
} else {
	$user = Utilisateur::instance ();
	$mail = filter_input ( INPUT_POST, 'mail', FILTER_SANITIZE_EMAIL );
	$token = filter_input ( INPUT_POST, 'token', FILTER_SANITIZE_STRING );
	$pass = filter_input ( INPUT_POST, 'pass', FILTER_SANITIZE_STRING );
	
	try {
		if ($user->isTokenValid ( $mail, $token )) {
			$user->modifyPassword ( $mail, $pass );
			http_response_code ( 200 );
			echo "OK";
		} else {
			http_response_code ( 401 );
			echo "Token invalide";
		}
	} catch ( Exception $e ) {
		http_response_code ( 503 );
		echo "Erreur serveur";
	}
}

// /@endcond

?>