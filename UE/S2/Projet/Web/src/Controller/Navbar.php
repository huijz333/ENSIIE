<?php

namespace Controller;

/**
 *  Représente la barre en haut du site
 */
class Navbar extends PageElement {

    /**
	 *	affiches le contenu dans la page
     */
    public function afficher() {
        include VIEW_FOLDER . "/navbar.phtml";
    }
}

?>