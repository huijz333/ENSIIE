<?php
session_start();
include("config.php");
include("db.php");

function verif_authent(){
    global $AUTHENT;
    /*
      Complétez cette fonction pour que si la variable AUTHENT est positionnée à 1 et que la variable de session 
      comprenant le nom d'utilisateur nomuser n'est pas positionnée, l'utilisateur soit redirigé vers la page tpConnexion.php
      (voir fonction header() de php)
    */
}

function config() {
    global $nom_hote, $nom_user, $nom_base, $mdp;
    $_SESSION['nomhote'] = $nom_hote;
    $_SESSION['nombase'] = $nom_base;
    $_SESSION['nomuser'] = $nom_user;
    $_SESSION['mdp'] = $mdp;
}



function get_client($numCli)
{

/*
Cette fonction doit exécuter une requête SELECT permettant de récupérer le nom et le débit d'un client de numéro donné
et retourner un tableau comprenant trois valeurs:
- une première valeur donnant une information sur l'exécution: 0 si tout s'est bien passé, -1 si aucun client n'a été trouvé avec le numéro donné, -2 si la requête n'a pas pu être exécutée, -3 si la connexion à la BD n'a pas réussi
- la seconde valeur donne le nom du client si tout s'est bien passé, 0 sinon
- la troisième valeur donne le débit du client si tout s'est bien passé, 0 sinon
*/

  if ( $db = db_connect()) {
        $num = test_input($numCli);
        $req = "SELECT nom_client, debit_client FROM client WHERE num_client=$num";
        $result = db_query($db,$req);
        if (!$result) {
            return array(-2, 0, 0);
        }
        if (db_count($res) == 0) {
            return array(-1, 0, 0);
        }
        db_close( $db );
        $rows = db_fetch($result);
        return array(0, $rows['nom_client'], $rows['debit_client']);
  }
  return array(-3, 0, 0);
}

function insert_achat($numCli,$montant) {
/*
  Cette fonction doit effectuer une transaction qui:
  - met à jour le débit du client du numéro concerné (après avoir testé la valeur entrée par l'utilisateur si possible)
  - insère un nouvel achat dans la base
  Il faudra utiliser une transaction pour garder une base cohérente même en cas d'erreur.

*/

  if ( $db = db_connect()) {
        $num = test_input($numCli);
        $req1 = "UPDATE client SET débit_client=41 WHERE num_client=$num";
        $req2 = "INSERT INTO achat(montant_achat, client)
                        VALUES ($montant, $num)";
        db_transaction($db, array($req1, $req2));
        db_close( $db );
        return true ;
  }
  return false ;
}


function create_client($numCli,$nomCli,$debitInit) {

    if ( $db = db_connect()) {
		$req = "INSERT INTO client(num_client,nom_client,debit_client) VALUES (".test_input($numCli).",'".test_input($nomCli)."','".format_number(test_input($debitInit))."')";
		db_query($db,$req);
		db_close( $db );
		return true;
	}
	else {
		return false;
	}
}


function set_client($numCli,$nomMod,$debitMod) {
    if ( $db = db_connect()) {
        $req = 'UPDATE client SET nom_client = \''.test_input($nomMod).'\', debit_client = \''.format_number(test_input($debitMod)).'\' WHERE num_client = '.test_input($numCli);

        $rep = db_query($db,$req);
        db_close( $db );

        return true;
    }
    else {
        return false;
    }

}

function verif_mdp($mdp) {

	if ($db = db_connect()) {

    $pass = test_input($mdp);
    $user = $_SESSION['nomuser'];
    $req = "SELECT password FROM users WHERE name=$user";
    $result = db_query($db,$req);
    if (!$result) {
      return false ;
    }
    if (db_count($res) == 0) {
      return false ;
    }
    $rows = db_fetch($result);
    $realpass = $rows['password'];
    if ($realpass == $pass) {
      db_close( $db );
      $_SESSION['mdp']=$pass;
      return true;
    }
    db_close( $db );
		return false;
	} else {
		return false;
  }

}

function detruire_session() {
	session_destroy();
}




function test_input($data) {
  $data = trim($data);
  $data = stripslashes($data);
  $data = htmlspecialchars($data);
  return $data;
}





function format_number($str) {
  return str_replace(',','.',$str);
}







