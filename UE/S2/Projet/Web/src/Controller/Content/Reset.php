<?php

namespace Controller\Content;

/**
 * Représente la page de reset du mot de passe
 */
class Reset extends \Controller\Content {
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::getTitle()
	 */
	public function getTitle() {
		return ("Reset");
	}
	
	/**
	 *
	 * {@inheritdoc}
	 * @see \Controller\Content::getPHTML()
	 */
	public function getPHTML() {
		return ('/reset.phtml');
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