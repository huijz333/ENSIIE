/**
 * effectue une requete à l'api
 * 
 * @param service:
 *            la chaine de caractère correspondant au service. e.x:
 *            '/user/account/disconnect/'
 * @param callback :
 *            une map reliant les codes de réponses html (200, 404, ...) à une
 *            fonction qui sera appelé en fonction du code de réponse obtenus
 *            par la requête.
 * @param parameters:
 *            une map avec les paramètres de la requête
 */
 function requestAPI(service, callback, parameters={}) {
 	// on s'assure que le service est bien formatté
 	if ((!service.endsWith("/"))) {
 		service = service + "/";
 	}

 	// on s'assure que le service existe
 	if (!(service in API)) {
 		toast("Le service demandé est invalide: " + service, "error");
 		return (false);
 	}

 	// type de la requete
 	var method = API[service];

 	// création de la requete
	var xhr = new XMLHttpRequest();
	xhr.open(method.name, './api' + service + method.formatUrl(parameters), true);
	xhr.onreadystatechange = function() {
		if (xhr.readyState == XMLHttpRequest.DONE) {
			if (xhr.status in callback) {
				callback[xhr.status](xhr.response);
			} else if (xhr.status in DEFAULT_CALLBACK) {
				DEFAULT_CALLBACK[xhr.status](xhr.response);
			} else {
				// TODO remove this
				toast("Erreur " + xhr.status + " non gérée sur le service " + service + " : " + xhr.response, "error");
			}
		}
	}
	// envoie de la requete avec les arguments formattés
	xhr.send(method.sendArguments(parameters));
 }

/**
 * constantes pratiques pour distinguer les types de requêtes
 */
var REQUEST = {
	POST: {
		"name"			: 	"POST",
		"formatUrl" 	: 	function(parameters) {
								return ("");
							},
		"sendArguments"	: 	function(parameters) {
								var data = new FormData();
								for (var parameterName in parameters) {
									data.append(parameterName, parameters[parameterName]);
								}
								return (data);
							}
	},

	GET: {
		"name"			: 	"GET",
		"formatUrl"		: 	function(parameters) {
								var data = "?";
								for (var parameterName in parameters) {
									data = data + parameterName + "=" + parameters[parameterName] + "&";
								}
								return (data);
							},
		"sendArguments"	: 	function(parameters) {
								return (null);
							}

	}
}

/**
 * Les callback par défaut sur les retours serveurs
 */
var DEFAULT_CALLBACK = {
		400:	function(response) {
					toast("La requête est invalide.</br>Veuillez contacter un administrateur. " + response, "error");
				},
				
        401: function(response) {
                	toast('Vous êtes deconnecté. Veuillez vous reconnecter au site...', "error")
            	},
				
		503:	function(response) {
					toast("La base de données est innacessible pour le moment.", "error");
				}
};

/**
 * Relis un service au type de la requête à envoyer
 */
//TODO : method on parameters not on request
 var API = {
	"/permission/get/"							: REQUEST.GET,
 	"/role/add_permission/"						: REQUEST.POST,
 	"/role/get/"								: REQUEST.GET,
 	"/role/remove_permission/"					: REQUEST.POST,
 	"/school/list/" 							: REQUEST.GET,
 	"/user/account/connect/" 					: REQUEST.POST,
 	"/user/account/disconnect/" 				: REQUEST.POST,
 	"/user/account/register/" 					: REQUEST.POST,
 	"/user/account/image/set/" 					: REQUEST.POST,
 	"/user/account/password/modify/"			: REQUEST.POST,
 	"/user/account/password/reset/"				: REQUEST.POST,
 	"/user/account/permission/get/"				: REQUEST.GET,
 	"/user/account/role/add/"					: REQUEST.POST,
 	"/user/account/role/get/"					: REQUEST.GET,
 	"/user/game/lol/accounts/delete/"			: REQUEST.POST,
 	"/user/game/lol/accounts/get/"				: REQUEST.GET,
 	"/user/game/lol/third-party-code/generate/"	: REQUEST.GET,
 	"/user/game/lol/third-party-code/link/"		: REQUEST.POST,
 	"/user/list/"				                : REQUEST.GET,
 	"/user/notification/get/"					: REQUEST.GET,
 	"/user/notification/see/"					: REQUEST.POST,
 	"/user/notification/seeall/"				: REQUEST.POST,
 	"/team/list/"								: REQUEST.GET,
 	"/tournament/list/"							: REQUEST.GET
 };