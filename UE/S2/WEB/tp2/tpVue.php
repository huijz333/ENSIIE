<?php

function enTete($titre)
{
    print "<!DOCTYPE html>\n";
    print "<html>\n";
    print "  <head>\n";
    print "    <meta charset=\"utf-8\" />\n";
    print "    <title>$titre</title>\n";
    print "    <link rel=\"stylesheet\" href=\"tpStyle.css\"/>\n";
    print "  </head>\n";
  
    print "  <body>\n";
    print "    <header><h1> $titre </h1></header>\n";
}

function pied(){
    print " </body>\n";
    print "</html>\n";
/* Compléter cette fonction afin qu'elle ferme les balises body et html */

}


function retour_menu() 
{ 
/* A compléter lorsque demandé, pour afficher: 
   - un lien de retour vers la page de menu (index.php) si l'on n'est pas dans cette page
   - un lien vers la page de déconnexion (tpQuitter.php) sur toutes les pages (sauf tpQuitter)
*/
   print "<a href=index.php>retour vers la page de menu</a>\n";
   print "<a href=tpQuitter.php>Deconnexion</a>\n";
}


function vue_connexion() {
    echo '<section>
        <p> Bonjour, bienvenue sur l\'application de gestion des débits clients.
        Commencez par vous authentifier </p>

        <br/>

        <form action="tpVerifMDP.php" method="post">
            <label>Entrez votre mot de passe :</label> <input type="password" name="mdp" size="8"/><br/>
            <input type="submit" value="Valider"/>
        </form>
        </section>';

}

function affiche($str) {
    echo $str;
}


function affiche_info($str) {
    echo '<p>'.$str.'</p>';
}

function affiche_erreur($str) {
    echo '<p class="erreur">'.$str.'</p>';
}

function affiche_erreur_client($errcode) {
    switch ($errcode) {
        case -1:
            affiche_erreur("Aucun client n'a été trouvé pour ce numéro client.");
            break ;
        case -2:
            affiche_erreur("La requete n'a pas pu être executé.");
            break;
        case -3:
            affiche_erreur("La connection à la base de donnée a échouée");
            break ;
        default:
            affiche_erreur("Erreur inconnue");
    } 
}

function affiche_menu(){
echo '<form action="tpGestionClient.php" method="post">'.
        ajout_champ('number', '', 'numCli', 'Numéro de client', 'numCli', 1).'<br/>'.
	"<fieldset>
	<legend>Choix:</legend>\n".
  ajout_champ("radio", "v", "choix", "Visualisation", "v", 1)."<br/>\n".  
  ajout_champ("radio", "m", "choix", "Modification", "m", 1)."<br/>\n".  
  ajout_champ("radio", "c", "choix", "Création", "c", 1)."<br/>\n".  
  ajout_champ("radio", "a", "choix", "Achat", "a", 1)."<br/>\n".  
/*

	Ajouter trois champs de menu:
	- un champ Modification
	- un champ Création
	- un champ Achat
*/
	"</fieldset>\n".
	ajout_champ('submit', 'Envoyer', 'soumission', '', '', 0)."\n".
	'</form>';
}	



function affiche_form_modif($numCli, $nomCli,$debitCli){
/*
  Fonction qui créé un formulaire envoyant vers tpModifClient et contient:
  - un champ caché permettant de conserver le numéro du client 
  - un champ texte comprenant le nom actuel du client et permettant de le modifider
  - un champ numérique comprenant le débit actuel du client et permettant de le modifier
  - un champ pour envoyer le formulaire
*/

   echo '<form action="tpModifClient.php" method="POST">'.
          ajout_champ('hidden', $numCli, 'numCli', '', '', 0).'<br/>'.
          ajout_champ("text", $nomCli, "nomMod", "Nom du client", "nomMod", 1)."<br/>\n". 
          ajout_champ("number", $debitCli, "debitMod", "Débit du client", "debitMod", 1)."<br/>\n". 
          ajout_champ('submit', 'Envoyer', 'soumission', '', '', 0)."\n".'</form>';
}


function affiche_form_achat($numCli,$achat){
/*
  Fonction qui créé un formulaire envoyant vers tpNvelAchat et contient:
  - un champ caché permettant de conserver le numéro du client 
  - un champ numérique pour indiquer le montant de l'achat
  - un champ pour envoyer le formulaire
*/


   echo '<form action="tpNvelAchat.php" method="POST">'.
          ajout_champ('hidden', $numCli, 'numCli', '', '', 0).'<br/>'.
          ajout_champ("number", $achat, "montant", "Montant de l'achat.", "montant", 1)."<br/>\n". 
          ajout_champ('submit', 'Envoyer', 'soumission', '', '', 0)."\n".'</form>';
           
}


function affiche_form_creation($numCli){
  echo '<form action="tpCreationClient.php" method="POST">'.
          ajout_champ('hidden', $numCli, 'numCli', '', '', 0).'<br/>'.
          ajout_champ("text", '', "nomCli", "Nom client", "nomCli", 1)."<br/>\n".	
          ajout_champ("text", '', "debitInit", "Débit initial", "debitInit")."<br/>\n".	
          ajout_champ('submit', 'Envoyer', 'soumission', '', '', 0)."\n".'</form>';
}


function ajout_champ(){
/* fonction qui prend comme arguments dans l'ordre: type, value, name, label, id, (required), (step) 
    (les arguments entre parenthèses ne sont pas obligatoires, mais doivent être à l'index prévu:
    par exemple, si on veut indiquer un argument step, il faut mettre un argument required)
*/

	$tmp='';
	//label
	if(! empty(func_get_arg(3))){
		$tmp.= '<label for="'.func_get_arg(4) .'">'.func_get_arg(3).':</label>';
	}
	$tmp .= '<input type="'.func_get_arg(0).'" ';
	if(! empty(func_get_arg(4))){
		$tmp.= 'id="'.func_get_arg(4).'" ';
	}
	if(! empty(func_get_arg(1))){
		$tmp.= 'value="'.func_get_arg(1).'" ';
	}
	if(! empty(func_get_arg(2))){
		$tmp.= 'name="'.func_get_arg(2).'" ';
	}
	if(func_num_args()>5 && func_get_arg(5)){
		$tmp.= 'required="required" ';
	}
	if(func_num_args() > 6 && func_get_arg(0) == "number" && func_get_arg(6) == -1){
		$tmp .= 'step="any" ';
	}

	$tmp.='>';
	return $tmp;
}


function number_fr($num){
    return number_format($num, 2, ',', ' ');
}


?>
