<?php

namespace Controller\Content;

/**
 * Représente la page d'erreur de redirections
 */
class Erreur extends \Controller\Content {
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::getTitle()
	 */
	public function getTitle() {
		return ("Erreur");
	}
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::getPHTML()
	 */
	public function getPHTML() {
		return ('/erreur.phtml');
	}
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::requireConnection()
	 */
	public function requiteAuthentification() {
		return (false);
	}
}

?>