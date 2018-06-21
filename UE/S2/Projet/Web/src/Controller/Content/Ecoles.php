<?php

namespace Controller\Content;

/**
 * Représente la page d'accueil
 */
class Ecoles extends \Controller\Content {
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::getTitle()
	 */
	public function getTitle() {
		return ("Ecoles");
	}
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::getPHTML()
	 */
	public function getPHTML() {
		return ('/ecoles.phtml');
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