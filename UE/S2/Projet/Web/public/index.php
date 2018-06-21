<?php

/* include */
use Controller\Aside;
use Controller\Content;
use Controller\Footer;
use Controller\Header;
use Controller\Navbar;
use Controller\Sidebar;
use Controller\Toastbar;
use Model\ULC\BDD\BDD;
use Model\ULC\Permissions\Permission;
use Model\ULC\Utilisateur\Utilisateur;

require ('../vendor/autoload.php');
define ( 'PROJECT_ROOT', realpath ( dirname ( __FILE__ ) . "/../" ) );
define ( 'VIEW_FOLDER', PROJECT_ROOT . "/src/View" );

/* recupere la session */
session_start ();

/**
 * DEBUT : AFFICHAGE DE LA PAGE
 */
$pageElementsClass = [ 
		Header::class,
		Toastbar::class,
		Navbar::class,
		Sidebar::class,
		Content::getContent (),
		Aside::class,
		Footer::class 
];

/* recupere l'utilisateur */
$bdd = BDD::instance ();

/* recupere l'utilisateur */
$user = Utilisateur::instance ();

foreach ( $pageElementsClass as $pageElementClass ) {
	$pageElement = new $pageElementClass ( $bdd, $user );
	$pageElement->afficher ();
}


/** FIN : AFFICHAGE DE LA PAGE */
