<?php

namespace Model\ULC\Utilisateur;

use Model\ULC\BDD\BDD;
use Model\ULC\BDD\ConnectionException;
use PDO;
use PDOException;

/**
 * Représente l'utilisateur sur le site.
 */
class Utilisateur {
	
	/**
	 * l'unique instance Utilisateur
	 */
	private static $instance = NULL;
	
	/**
	 *
	 * @return Utilisateur l'instance correspondant à l'utilisateur du site
	 */
	public static function instance() {
		if (Utilisateur::$instance == NULL) {
			Utilisateur::$instance = new Utilisateur ();
			Utilisateur::$instance->loadSession ();
		}
		return (Utilisateur::$instance);
	}
	
	/**
	 * clef primaire de l'utilisateur dans la base de données
	 * (enregistré dans la variable de session, ou NULL si non connecté)
	 *
	 * @var number id dans la bdd
	 */
	private $uuid;
	
	/**
	 *
	 * @var string pseudo du joueur
	 */
	private $pseudo;
	
	/**
	 *
	 * @var string mail du joueur
	 */
	private $mail;
	
	/**
	 *
	 * @var array roles du joueur
	 */
	private $permissions;
	
	/**
	 * constructeur
	 */
	private function __construct() {
		$this->uuid = NULL;
	}
	
	/**
	 *
	 * @internal : export l'utilisateur vers la variable session
	 */
	private function saveSession() {
		if (session_status () == PHP_SESSION_NONE) {
			session_start ();
		}
		if ($this->uuid == NULL) {
			session_destroy ();
		} else {
			$_SESSION ['uuid'] = $this->uuid;
		}
	}
	
	/**
	 *
	 * @internal : charges l'utilisateur à partir de sa session
	 */
	private function loadSession() {
		if (session_status () == PHP_SESSION_NONE) {
			session_start ();
		}
		$this->uuid = isset ( $_SESSION ['uuid'] ) ? $_SESSION ['uuid'] : NULL;
		if ($this->uuid) {
			$this->update ();
		}
	}
	
	/**
	 * deconnectes l'utilisateur (detruit sa session)
	 */
	public function disconnect() {
		$this->uuid = NULL;
		$this->saveSession ();
	}
	
	/**
	 * Enregistres l'utilisateur dans la base de donnée
	 *
	 * @param string $mail
	 * @param string $pseudo
	 * @param string $pass
	 * @return bool : bool true on success or false on failure
	 * @throws PDOException : si une erreur a lieu
	 * @throws ConnectionException : si la connection à la base de donnée a echoué
	 */
	public function register($mail, $pseudo, $pass) {
		/* on recupere la connection à la pdo */
		$pdo = BDD::instance ()->getConnection ( "ulc" );
		
		/* prépares la base de données */
		$stmt = $pdo->prepare ( "INSERT INTO utilisateur (mail, pseudo, pass) VALUES (:mail, :pseudo, :pass)" );
		/* protège des injections sql */
		$hashedPass = password_hash ( $pass, PASSWORD_DEFAULT );
		$stmt->bindParam ( ':mail', $mail, PDO::PARAM_STR );
		$stmt->bindParam ( ':pseudo', $pseudo, PDO::PARAM_STR );
		$stmt->bindParam ( ':pass', $hashedPass, PDO::PARAM_STR );
		
		/* execute la requete sécurisé */
		return ($stmt->execute ());
	}
	
	/**
	 * Modifie le mot de passe de l'utilisateur
	 *
	 * @param string $mail
	 * @param string $pass
	 * @return bool true on success or false on failure
	 * @throws ConnectionException : si une erreur a lieu
	 */
	public function modifyPassword($mail, $pass) {
		/* on recupere la connection à la pdo */
		$pdo = BDD::instance ()->getConnection ( "ulc" );
		
		$stmt = $pdo->prepare ( "UPDATE utilisateur SET pass=:pass WHERE mail=:mail" );
		
		$stmt->bindParam ( ':mail', $mail, PDO::PARAM_STR );
		
		$hashedPass = password_hash ( $pass, PASSWORD_DEFAULT );
		$stmt->bindParam ( ':pass', $hashedPass, PDO::PARAM_STR );
		
		return ($stmt->execute ());
	}
	
	/**
	 * Vérifie que le token passé en paramètre, pour l'adresse mail
	 * de l'utilisateur associé est valide.
	 *
	 * @param string $mail
	 * @param string $token
	 * @return TRUE si le token est valid, FALSE sinon
	 * @throws ConnectionException : si la connection à la base de donnée a echoué
	 */
	public function isTokenValid($mail, $token) {
		$pdo = BDD::instance ()->getConnection ( "ulc" );
		
		$stmt = $pdo->prepare ( "SELECT * FROM reset_token
                                    JOIN utilisateur ON utilisateur_id=id
                                        WHERE mail=:mail AND token=:token
                                            AND (NOW() BETWEEN date_generation AND date_generation + '15 minutes'::interval)
                            " );
		$stmt->bindParam ( ':mail', $mail, PDO::PARAM_STR );
		$stmt->bindParam ( ':token', $token, PDO::PARAM_STR );
		$stmt->execute ();
		return ($stmt->rowCount () == 1);
	}
	
	/**
	 * Connecte l'utilisateur sur le site.
	 * Exemple d'utilisation:
	 * <pre> <code>
	 * $mail = filter_input(INPUT_POST, 'mail', FILTER_SANITIZE_EMAIL);
	 * $pass = filter_input(INPUT_POST, 'pass', FILTER_SANITIZE_STRING);
	 * if ($utilisateur->connectAs($db, $mail, $pass)) {
	 * print("Vous etes connecté");
	 * } else {
	 * print("Erreur d'identification");
	 * }
	 * </code> </pre>
	 *
	 * @param PDO $db
	 * @param string $mail
	 * @param string $pass
	 * @return bool true si la connection a eu lieu avec succès
	 * @throws NoSuchUtilisateurException : si les identifiants sont invalides
	 * @throws ConnectionException : si la connection à la base de donnée a échouée
	 *  @seeall http://www.phptherightway.com/#databases_interacting
	 *  @seeall http://php.net/manual/fr/filter.filters.sanitize.php
	 *  @seeall http://php.net/manual/fr/pdostatement.bindparam.php
	 */
	public function connectAs($mail, $pass) {
		$pdo = BDD::instance ()->getConnection ( "ulc" );
		
		$stmt = $pdo->prepare ( 'SELECT id, pass FROM utilisateur WHERE mail = :mail' );
		$stmt->bindParam ( ':mail', $mail, PDO::PARAM_STR );
		
		$stmt->execute ();
		if ($stmt->rowCount () == 0) {
			throw new NoSuchUtilisateurException ( "Addresse mail erronée" );
		}
		/* renvoie un tableau associatif de l'entrée dans la table */
		$entry = $stmt->fetch ();
		/* si le mot de passe est juste */
		if (password_verify ( $pass, $entry ['pass'] )) {
			$this->uuid = $entry ['id'];
			echo $this->uuid;
			$this->saveSession ();
			return (true);
		}
		throw new NoSuchUtilisateurException ( "Mot de passe erroné" );
	}
	
	/**
	 *
	 * @return true si l'utilisateur est connecté, false sinon
	 */
	public function isConnected() {
		return ($this->uuid != NULL);
	}
	
	/**
	 *
	 * @return int la clef primaire du utilisateur dans la base de donnée
	 */
	public function getID() {
		return ($this->uuid);
	}
	
	/**
	 * Recuperes l'entrée dans la base de données pour cette utilisateur, et met à jour les attributs
	 *
	 * @throws ConnectionException : la connection à la BDD n'a pas abouti
	 */
	public function update() {
		$pdo = BDD::instance ()->getConnection ( "ulc" );
		
		/* recuperes les données de l'utilisateur */
		$stmt = $pdo->prepare ( 'SELECT mail, pseudo FROM utilisateur WHERE id = :id' );
		$stmt->bindParam ( ':id', $this->uuid, PDO::PARAM_INT );
		$stmt->execute ();
		if ($stmt->rowCount () == 0) {
			throw new NoSuchUtilisateurException ( "L'utilisateur a été supprimé de la base de donnée" );
		}
		$entry = $stmt->fetch ();
		$this->mail = $entry ['mail'];
		$this->pseudo = $entry ['pseudo'];
		
		/* recuperes toutes les permissions de l'utilisateur */
		$this->permissions = array ();
		$stmt = $pdo->prepare ( 'SELECT DISTINCT permission_id FROM role_permission JOIN utilisateur_role ON role_permission.role_id = utilisateur_role.role_id WHERE utilisateur_id = :id' );
		$stmt->bindParam ( ':id', $this->uuid, PDO::PARAM_INT );
		$stmt->execute ();
		while ( ($entry = $stmt->fetch ()) != NULL ) {
			$this->permissions [$entry ['permission_id']] = true;
		}
	}
	
	/**
	 *
	 * @param
	 *        	Role le role
	 * @return true si l'utilisateur possède le role
	 */
	public function hasRole($role) {
		return (in_array ( $role->getID (), $this->roles ));
	}
	
	/**
	 *
	 * @param
	 *        	Permission la permission
	 * @return true si l'utilisateur possède la permission
	 */
	public function hasPermission($permission) {
		return (isset ( $this->permissions [$permission->getID ()] ));
	}
	
	/**
	 *
	 * @return string l'adresse mail du utilisateur
	 */
	public function getMail() {
		// l'utilisateur n'est pas connecté
		if (! $this->isConnected ()) {
			throw new NotConnectedException ();
		}
		return ($this->mail);
	}
	
	/**
	 *
	 * @return string le pseudo du utilisateur
	 */
	public function getPseudo() {
		// l'utilisateur n'est pas connecté
		if (! $this->isConnected ()) {
			throw new NotConnectedException ();
		}
		return ($this->pseudo);
	}
	
	/**
	 *
	 * @return string l'ecole à laquelle le utilisateur appartient
	 */
	public function getEcole() {
		return ($this->get ( 'ecole' ));
	}
	
	/**
	 * renvoie le utilisateur sous une string au format JSON
	 */
	public function toJSON() {
		return json_encode ( $this->entry );
	}
	
	/**
	 * Lie l'utilisateur du site au compte League of Legend
	 *
	 * @param integer $summonerID
	 * @throws PDOException : si le summonerID est déjà lié à un autre utilisateur
	 * @throws ConnectionException : connection echouée
	 * @throws NotConnectedException : si l'utilisateur n'est pas connecté
	 */
	public function linkLolAccount($summonerID) {
		if (! $this->isConnected ()) {
			throw new NotConnectedException ();
		}
		
		$pdo = BDD::instance ()->getConnection ( "ulc" );
		
		$stmt = $pdo->prepare ( "INSERT INTO utilisateur_lol (utilisateur_id, summoner_id) VALUES (:utilisateur_id, :summoner_id)" );
		$stmt->bindParam ( ':utilisateur_id', $this->uuid, PDO::PARAM_INT );
		$stmt->bindParam ( ':summoner_id', $summonerID, PDO::PARAM_INT );
		$stmt->execute ();
		return (true);
	}
}

?>