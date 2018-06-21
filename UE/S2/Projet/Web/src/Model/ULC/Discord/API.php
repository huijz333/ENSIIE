<?php

/**
 *  Pour ouvrir la Gateway, lancer API::discord() à partir de la ligne de commande
 *
 *  exemple d'utilisation:
 *      require('../vendor/autoload.php'); 
 *
 *      use Model\Discord\API;
 *
 *      $client = API::client();
 *      var_dump($client->guild->getGuild(['guild.id' => API::guildID()]));
 *      var_dump($client->channel->createMessage(['channel.id' => 440662985997025280, 'content' => '<@172798956823248896> est une tâche']));
 */
namespace Model\ULC\Discord;

use Discord\Discord;
use RestCord\DiscordClient;

/**
 * Représente l'API de Discord.
 * Cet objet facilite les transactions avec le serveur discord
 */
class API {
	
	/**
	 * Discord
	 */
	private static $discord = NULL;
	
	/**
	 * DiscordClient
	 */
	private static $client = NULL;
	
	/**
	 *
	 * @return l'instance de discord
	 */
	public static function discord() {
		if (API::$discord == NULL) {
			API::$discord = new Discord ( [ 
					'token' => API::botToken () 
			] );
			API::$discord->on ( 'ready', function ($discord) {
				echo "Bot is ready!", PHP_EOL;
				
				// Listen for messages.
				API::$discord->on ( 'message', function ($message, $discord) {
					echo "{$message->author->username}: {$message->content}", PHP_EOL;
				} );
			} );
			
			API::$discord->run ();
		}
		return (API::$discord);
	}
	
	/**
	 *
	 * @return DiscordClient : l'instance du client discord
	 */
	public static function client() {
		if (API::$client == NULL) {
			API::$client = new DiscordClient ( [ 
					'token' => API::botToken () 
			] );
		}
		return (API::$client);
	}
	
	/**
	 *
	 * @return string : token du bot
	 */
	public static function botToken() {
		return ('NDMyNTM4MjI1MjgyNzc3MTA4.DcnpUg.DY0sPCmhcM9f7yglkny3ZKPzMUA');
	}
	
	/**
	 *
	 * @return int : id de la guilde Discord
	 */
	public static function guildID() {
		return (432533096609480704);
	}
}

