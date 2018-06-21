<?php
/**
 * File              : Web/src/Model/ULC/LOL/API.php
 * @author           : rpereira <romain.pereira@ensiie.fr>
 * Date              : 21.06.2018
 * Last Modified Date: 21.06.2018
 * Last Modified By  : rpereira <romain.pereira@ensiie.fr>
 */

namespace Model\ULC\LOL;

use RiotAPI\RiotAPI;
use RiotAPI\Definitions\Region;

/**
 * Représente l'API de Riot.
 * Cet objet facilite les transactions avec les serveurs de Riot.
 *
 * exemple d'utilisation:
 *
 * ```
 * require('../vendor/autoload.php');
 *
 * use Model\API;
 *
 * $riot = API::riot();
 *
 * $summoner = $riot->getSummonerByName('Lamoukk');
 * var_dump($summoner);
 * ```
 */
class API {
	
	/**
	 * RiotAPI
	 */
	private static $riot = NULL;
	
	/**
	 *
	 * @return l'instance de RiotAPI
	 */
	public static function riot() {
		if (API::$riot == NULL) {
			API::$riot = new RiotAPI ( [ 
					RiotAPI::SET_KEY => 'RGAPI-7525589e-1c31-489b-9145-e2b89663db9a',
					RiotAPI::SET_REGION => Region::EUROPE_WEST 
			] );
		}
		return (API::$riot);
	}
}

?>
