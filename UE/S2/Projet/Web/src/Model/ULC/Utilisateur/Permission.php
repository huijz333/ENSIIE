<?php

namespace Model\ULC\Utilisateur;

/**
 * Représente une permission de la bdd
 */
class Permission {
	
	/**
	 * les différentes permissions
	 */
	public static $CREATE_TEAM;
	public static $CREATE_TOURNAMENT;
	public static $CREATE_ROLE;
	public static $DELETE_ROLE;
	public static $ASSIGN_ROLE;
	public static $UNASSIGN_ROLE;
	public static $ADD_PERMISSION;
	public static $REMOVE_PERMISSION;
	
	/**
	 * le tableau qui contient tous les permissions
	 */
	private static $PERMISSIONS_BY_ID = array ();
	private static $PERMISSIONS_BY_NAME = array ();
	
	/**
	 * initialises les variables statiques
	 */
	public static function __init() {
		Permission::$CREATE_TEAM = new Permission ( "create_team", "Créer une équipe" );
		Permission::$CREATE_TOURNAMENT = new Permission ( "create_tournament", "Créer un tournoi" );
		Permission::$CREATE_ROLE = new Permission ( "create_role", "Créer un nouveau rôle" );
		Permission::$DELETE_ROLE = new Permission ( "delete_role", "Supprimer un rôle" );
		Permission::$ASSIGN_ROLE = new Permission ( "assign_role", "Assigner un rôle à un utilisateur" );
		Permission::$UNASSIGN_ROLE = new Permission ( "unassign_role", "Supprimer un rôle à un utilisateur" );
		Permission::$ADD_PERMISSION = new Permission ( "add_permission", "Ajouter une permission à un rôle" );
		Permission::$REMOVE_PERMISSION = new Permission ( "remove_permission", "Supprimer une permission d'un rôle" );
	}
	
	/**
	 *
	 * @return array la liste de tous les permissions
	 */
	public static function getPermissions() {
		return (Permission::$PERMISSIONS_BY_ID);
	}
	
	/**
	 *
	 * @return Permission la permission par son ID (clef primaire)
	 */
	public static function getPermissionByID($permissionID) {
		return (isset ( Permission::$PERMISSIONS_BY_ID [$permissionID] ) ? Permission::$PERMISSIONS_BY_ID [$permissionID] : NULL);
	}
	
	/**
	 *
	 * @return Permission la permission role par son nom
	 */
	public static function getPermissionByName($permissionName) {
		return (isset ( Permission::$PERMISSIONS_BY_NAME [$permissionName] ) ? Permission::$PERMISSIONS_BY_NAME [$permissionName] : NULL);
	}
	
	/** @var number la clef primaire de la permission */
	private $id;
	
	/** @var string le nom de la permission */
	private $name;
	
	/** @var string description de la permission */
	private $description;
	
	/**
	 * constructeur
	 *
	 * @param number $id
	 * @param string $name
	 * @param array $permissions
	 */
	public function __construct($name, $description) {
		$this->name = $name;
		$this->description = $description;
		$this->id = count ( Permission::$PERMISSIONS_BY_ID ) + 1;
		Permission::$PERMISSIONS_BY_ID [$this->id] = $this;
		Permission::$PERMISSIONS_BY_NAME [$this->name] = $this;
	}
	
	/**
	 * Renvoie l'id de la permission
	 *
	 * @return number l'id de la permission
	 */
	public function getID() {
		return ($this->id);
	}
	
	/**
	 * Renvoie le nom de la permission
	 *
	 * @return number le nom de la permission
	 */
	public function getName() {
		return ($this->name);
	}
	
	/**
	 * Renvoie la description
	 *
	 * @return string la description de la permission
	 */
	public function getDescription() {
		return ($this->description);
	}
}

Permission::__init ();




