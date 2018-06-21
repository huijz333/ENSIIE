<?php

namespace Controller;

/**
 * Représente le contenu au milieu de la page du site
 */
abstract class Content extends PageElement {
	
	/**
	 * Fonction principal qui renvoie le PageElement à afficher
	 * en fonction de la variable GET['page']
	 */
	public static function getContent() {
		/**
		 * les pages afficheables
		 */
		$pages = [ 
				"accueil" => Content\Accueil::class,
				"compte" => Content\Compte::class,
				"ecoles" => Content\Ecoles::class,
				"erreur" => Content\Erreur::class,
				"equipes" => Content\Equipes::class,
				"joueurs" => Content\Joueurs::class,
				"live" => Content\Live::class,
				"nous" => Content\Nous::class,
				"profil" => Content\Profil::class,
				"reset" => Content\Reset::class,
				"tournois" => Content\Tournois::class 
		];
		
		$pageID = isset ( $_GET ['page'] ) ? $_GET ['page'] : "accueil";
		return (isset ( $pages [$pageID] ) ? $pages [$pageID] : $pages ["erreur"]);
	}
	
	/**
	 * affiches le contenu dans la page
	 */
	public function afficher() {
		/* affiche le header de la page et le titre */
		include VIEW_FOLDER . "/Content/header.phtml";
		
		/* affiche le contenu */
		if ($this->requiteAuthentification () && ! $this->getUser ()->isConnected ()) {
			include VIEW_FOLDER . "/Content/non_connecte.phtml";
		} else {
			include VIEW_FOLDER . "/Content" . $this->getPHTML ();
		}
		
		/* affiche le footer du contenu */
		include VIEW_FOLDER . "/Content/footer.phtml";
	}
	
	/**
	 *
	 * @return boolean true si ce contenu requiet une authentification pour être affiché
	 */
	public abstract function requiteAuthentification();
	
	/**
	 *
	 * @return string : le titre de la page
	 */
	public abstract function getTitle();
	
	/**
	 * Renvoie le code html à être inséré dans le contenu de la page
	 * (entre 2 balises <div class="page"> </div>)
	 *
	 * @return string : chemin du fichier .phtml qui correspond au contenu de la page
	 */
	public abstract function getPHTML();
}