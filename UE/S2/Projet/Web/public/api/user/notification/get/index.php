<?php

/**
 *  @file
 *  @brief Renvoie toutes les notifications de l'utilisateur du site
 *  @param :
 *      - COOKIE \a PHPSESSID : le cookie de session PHP \ref api/user/account/connect/index.php
 *      - POST \a max (optionnel) : nombre maximum de notifications à afficher (les 'max' plus récentes)
 *  @return
 *      - JSON : les notifications
 *      \code{.json}
 *  {
 *      "notifications": [
 *        {
 *              "id": "1",
 *              "type": "bienvenue",
 *              "content": "L'équipe d'ULC vous souhaite la bienvenue.",
 *              "date": "2018-05-07 14:08:52.671358",
 *              "status": "unseen",
 *              "action": "equipe"
 *          }
 *      ]
 *  }
 *      \endcode
 *      - code reponse:
 *                      - 200 : les notifications ont été généré avec succès
 *                      - 401 : non connecté(e)
 *                      - 503 : erreur de connection à la base de donnée
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
		$max = 999;
		$rows = isset ( $_POST ["max"] ) ? $_POST ["max"] : $max;
		if ($rows < 0) {
			$rows = 0;
		} else if ($rows > $max) {
			$rows = $max;
		}
		$stmt = $pdo->prepare ( 'SELECT * FROM notification WHERE utilisateur_id = :utilisateur_id ORDER BY date_envoie DESC, id DESC LIMIT :rows' );
		/* protège des injections sql */
		$id = $user->getID ();
		$stmt->bindParam ( ':utilisateur_id', $id, PDO::PARAM_INT );
		$stmt->bindParam ( ':rows', $rows, PDO::PARAM_INT );
		
		/* execute la requete sécurisé */
		$stmt->execute ();
		
		http_response_code ( 200 );
		
		/* on enregistre toutes les notifications */
		echo '{"notifications":[';
		if ($stmt->rowCount () > 0) {
			$entry = $stmt->fetch ();
			while ( $entry != NULL ) {
				echo '{';
				echo '"id":"' . $entry ['id'] . '"';
				echo ',';
				echo '"type":"' . $entry ['type'] . '"';
				echo ',';
				echo '"content":"';
				switch ($entry ['type']) {
					case 'bienvenue' :
						echo "L'équipe d'ULC vous souhaite la <strong>bienvenue</strong>.";
						break;
					case 'invitation' :
						echo "Vous avez reçu une <strong>invitation</strong> pour rejoindre une équipe.";
						break;
					default :
						break;
				}
				echo '",';
				echo '"date":"' . $entry ['date_envoie'] . '"';
				echo ',';
				echo '"status":"' . $entry ['status'] . '"';
				echo ',';
				echo '"action":"';
				switch ($entry ['type']) {
					case 'bienvenue' :
						echo "equipe";
						break;
					case 'invitation' :
						echo "invitations";
						break;
					default :
						break;
				}
				echo '"';
				echo '}';
				$entry = $stmt->fetch ();
				if ($entry != NULL) {
					echo ',';
				}
			}
		}
		echo ']}';
	} catch ( \Model\ULC\BDD\ConnectionException $e ) {
		http_response_code ( 503 );
		echo "erreur serveur";
	}
}

// /@endcode

?>