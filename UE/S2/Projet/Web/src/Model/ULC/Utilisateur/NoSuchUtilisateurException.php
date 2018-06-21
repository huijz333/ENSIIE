<?php

namespace Model\ULC\Utilisateur;

/**
 * Exception levé lorsqu'une connection n'a pas abouti
 */
class NoSuchUtilisateurException extends \Exception {
	
	public function __construct() {
	}
}
