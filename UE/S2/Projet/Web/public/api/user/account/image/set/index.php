<?php

/**
 *  @file
 *  @brief Définit l'image de profil d'un utilisateur
 *  @param :
 *      - COOKIE \a PHPSESSID : le cookie de session
 *      - FILES \a image      : l'image du profil utilisateur
 *  @return
 *      - code reponse:
 *                      - 200 : l'image a été trouvé et renvoyé
 *                      - 400 : erreur requête (paramètre(s) manquant(s), invalide(s))
 *                      - 401 : utilisateur non identifié
 */

// /@cond INTERNAL
use Model\ULC\Utilisateur\Utilisateur;

require '../../../../../../vendor/autoload.php';

session_start ();

$user = Utilisateur::instance ();

if (! $user->isConnected ()) {
	http_response_code ( 400 );
	echo "Non connecté";
	return;
}

if (! isset ( $_FILES ['image'] ) || $_FILES ['image'] ['error'] > 0 || $_FILES ['image'] ['size'] > 10485760) {
	http_response_code ( 401 );
	echo "la requête est mal formattée";
	return;
}

// TODO : gérer plus de formats, mais toujours convertir vers '.png'

$info = getimagesize ( $_FILES ['image'] ['tmp_name'] );
if (! $info || $info [2] != IMAGETYPE_PNG) {
	http_response_code ( 400 );
	echo "Le fichier n'est pas une image valide. Le seul format supporté est '.png'";
	return;
}

http_response_code ( 200 );
$dst = "../get/" . $user->getPseudo () . ".png";
move_uploaded_file ( $_FILES ["image"] ["tmp_name"], $dst );

// /@endcond

?>
