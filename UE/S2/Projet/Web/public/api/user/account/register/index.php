<?php

/**
 *  @file
 *  @brief Enregistre un nouvel utilisateur sur le site
 *  @param :
 *      - POST \a mail : l'adresse mail lié au compte
 *      - POST \a pseudo : un pseudo
 *      - POST \a pass : un mot de passe
 *      - POST \a grecaptcha : un code google-recaptcha v2
 *  @return
 *      - code reponse:
 *                      - 200 : l'utilisateur a été enregistré avec succès
 *                      - 400 : erreur de la requête (paramètre(s) manquant(s) ou invalide(s))
 *                      - 503 : erreur serveur (accès à la base de donnée)
 */

// /@cond INTERNAL

/* include path */
use Model\ULC\BDD\BDD;
use Model\ULC\BDD\ConnectionException;
use Model\ULC\Utilisateur\Utilisateur;

require '../../../../../vendor/autoload.php';

/* recupere l'utilisateur */
$bdd = BDD::instance ();

/* recupere l'utilisateur */
$user = Utilisateur::instance ();

/* vérifie que les entrées existent */
if (isset ( $_POST ['mail'] ) && isset ( $_POST ['pseudo'] ) && isset ( $_POST ['pass'] ) && isset ( $_POST ['g-recaptcha'] )) {
	/* verifie la validité des entrées (injections sql ...) */
	$mail = filter_input ( INPUT_POST, 'mail', FILTER_SANITIZE_EMAIL );
	$pseudo = filter_input ( INPUT_POST, 'pseudo', FILTER_SANITIZE_STRING );
	$pass = filter_input ( INPUT_POST, 'pass', FILTER_SANITIZE_STRING );
	$grecaptcha = $_POST ['g-recaptcha'];
	
	/* si le mot de passe est trop court ... */
	if (strlen ( $pass ) < 6) {
		http_response_code ( 400 );
		echo "le mot de passe est trop court.";
	} else {
		/* on vérifie aupres de google que le captcha est correct */
		/* Verify captcha */
		$post_data = http_build_query ( array (
				'secret' => "6LfSvVEUAAAAAOUCTZjrHx_jH4FolYPCtNiOWKRd",
				'response' => $grecaptcha,
				'remoteip' => $_SERVER ['REMOTE_ADDR'] 
		) );
		$opts = array (
				'http' => array (
						'method' => 'POST',
						'header' => 'Content-type: application/x-www-form-urlencoded',
						'content' => $post_data 
				) 
		);
		$context = stream_context_create ( $opts );
		$response = file_get_contents ( 'https://www.google.com/recaptcha/api/siteverify', false, $context );
		$result = json_decode ( $response );
		/* si le captcha est faux */
		if (! $result->success) {
			http_response_code ( 400 );
			echo "Un Robot sauvage apparait.";
		} else {
			/* sinon, on enregistre l'utilisateur */
			try {
				http_response_code ( 200 );
				$user->register ( $mail, $pseudo, $pass );
				echo "OK";
			} catch ( PDOException $e ) {
				http_response_code ( 400 );
				echo "Identifiants déjà utilisés";
			} catch ( ConnectionException $e ) {
				/**
				 * erreur base de donnée
				 */
				http_response_code ( 503 );
				echo "Erreur serveur";
			}
		}
	}
} else {
	http_response_code ( 400 );
	echo "la requête est mal formattée";
}
?>