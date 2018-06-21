<?php

/**
 *  @file
 *  @brief Crée une session utilisateur.
 *	@details Si succès, génère une session PHP (renvoie un Cookie de session)
 *				Sinon, renvoie le code d'erreur correspondant.
 *	@param :
 *		- POST \a mail
 *		- POST \a pass
 *	@return
 *      - COOKIE \a PHPSESSID : le cookie de session
 *		- code reponse:
 *						- 200 : l'utilisateur est connecté, affiche l'utilisateur au format JSON
 *						- 400 : erreur de la requête : paramètre(s) manquant(s)
 *						- 401 : identifiants incorrects
 *						- 503 : erreur serveur : accès à la base de donnée
 */

// /@cond INTERNAL
use Model\ULC\BDD\ConnectionException;
use Model\ULC\Utilisateur\NoSuchUtilisateurException;
use Model\ULC\Utilisateur\Utilisateur;

require '../../../../../vendor/autoload.php';

$user = Utilisateur::instance ();

if (isset ( $_POST ['mail'] ) && isset ( $_POST ['pass'] )) {
	$mail = filter_input ( INPUT_POST, 'mail', FILTER_SANITIZE_EMAIL );
	$pass = filter_input ( INPUT_POST, 'pass', FILTER_SANITIZE_STRING );
	
	try {
		$user->connectAs ( $mail, $pass );
		http_response_code ( 200 );
	} catch ( NoSuchUtilisateurException $e ) {
		http_response_code ( 400 );
		echo "Identifiants incorrects.";
	} catch ( ConnectionException $e ) {
		http_response_code ( 503 );
		echo "Erreur serveur";
	}
} else {
	http_response_code ( 400 );
	echo "la requête est mal formattée";
}

// /@endcond

?>
