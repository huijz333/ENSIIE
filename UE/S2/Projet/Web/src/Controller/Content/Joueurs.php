<?php

namespace Controller\Content;

/**
 * Représente la page de la liste des joueurs
 */
class Joueurs extends \Controller\Content {
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::getTitle()
	 */
	public function getTitle() {
		return ("Joueurs");
	}
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::getPHTML()
	 */
	public function getPHTML() {
		return ('/joueurs.phtml');
	}
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::requireConnection()
	 */
	public function requiteAuthentification() {
		return (true);
	}
}

?>