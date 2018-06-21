<?php
/**
 *  @file
 *  @brief Génère un token de réinitialisation de mot de passe
 *  @details Ce token pourra être utilisé pendant 15 minutes pour une requête de modification.
 *           \ref api/user/account/password/modify/index.php
 *  @param :
 *      - POST \a mail : l'adresse mail lié au compte
 *  @return
 *      - un mail contenant le token est envoyé à l'adresse fournie.
 *      - code reponse:
 *                      - 200 : le token a été généré et envoyé
 *                      - 400 : erreur de la requête (paramètre(s) manquant(s) ou invalide(s))
 *                      - 503 : erreur serveur (accès à la base de donnée)
 */

// /@cond INTERNAL

/* include path */
use Model\ULC\Utils;
use Model\ULC\BDD\BDD;
use Model\ULC\BDD\ConnectionException;

require '../../../../../../vendor/autoload.php';

// si l'utilisateur n'est pas connecté
if (! isset ( $_POST ['mail'] )) {
	http_response_code ( 400 );
	echo "Pas d'adresse mail";
	// crée un token de reinitialisation
} else {
	http_response_code ( 200 );
	$mail = $_POST ['mail'];
	$token = Utils::random_str ( 32 );
	$bdd = BDD::instance ();
	try {
		$pdo = $bdd->getConnection ( "ulc" );
		
		/* insert or update */
		$stmt = $pdo->prepare ( "WITH updated AS (UPDATE reset_token SET token=:token FROM utilisateur WHERE mail=:mail AND reset_token.utilisateur_id=id returning *) INSERT INTO reset_token (utilisateur_id, token) SELECT id, :token FROM utilisateur WHERE NOT EXISTS (SELECT * FROM updated) AND mail=:mail ;" );
		$stmt->bindParam ( ':token', $token, PDO::PARAM_STR );
		$stmt->bindParam ( ':mail', $mail, PDO::PARAM_STR );
		$stmt->execute ();
		
		/* envoie du mail */
		$subject = "ULC : Réinitialisation mot de passe";
		$content = "Pour réintialiser votre mot de passe, merci de cliquer sur le lien suivant: http://localhost:8080/index.php?page=reset&token=" . $token;
		$headers = 'From: webmaster@example.com' . "\r\n" . 'Reply-To: webmaster@example.com' . "\r\n" . 'X-Mailer: PHP/' . phpversion ();
		mail ( $mail, $subject, $content, $headers );
	} catch ( PDOException $e ) {
		http_response_code ( 400 );
		echo 'Requete invalide';
	} catch ( ConnectionException $e ) {
		http_response_code ( 503 );
		echo 'Erreur serveur';
	}
}

// /@endcond

?>
