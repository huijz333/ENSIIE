<?php

namespace Controller;

/**
 * Représente la barre à droite du site
 */
class Aside extends PageElement {
	
	/**
	 * Fonction principal qui affiche le contenu
	 */
	public function afficher() {
		include VIEW_FOLDER . "/aside.phtml";
	}
}