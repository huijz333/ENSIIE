<?php

namespace Controller\Content;

/**
 * Représente la page listant les tournois
 */
class Tournois extends \Controller\Content {
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::getTitle()
	 */
	public function getTitle() {
		return ("Tournois");
	}
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::getPHTML()
	 */
	public function getPHTML() {
		return ('/tournois.phtml');
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