/**
 *  This file is part of https://github.com/rpereira-dev/C_data_structures
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  PEREIRA Romain
 */

#ifndef HMAP_H
# define HMAP_H

# include "array.h"
# include <string.h>	/* size_t */
# include <stdlib.h>	/* malloc(), free() */

/** une entry de la table de hachage. */
typedef struct  s_hmap_entry {
	size_t	hash;	/* le hash de la clef */
	void 	* key;	/* la clef */    
	void	* value;/* la valeur */
}               t_hmap_entry;

typedef int	(*t_cmpf)(void const * left, void const * right);
typedef size_t	(*t_hashf)(void const * value);

/**
 *	structure de donnée: table de hachage.
 *	C'est un tableau de 't_array' qui contiennent des 't_hmap_entry'
 */
typedef struct  s_hmap {
	t_array	** entries;	/**
				 *  tableau de tableau d'entrées,
				 *  indexé par le hachage des clefs,
				 *  ce qu permet de gérer les collisions
				 */
	size_t	capacity;	/* taille de 'entries' */
	t_hashf	hashf;		/* fonction de hachage a appliqué aux clefs */
	t_cmpf	keycmpf;	/* fonction de comparaison pour les clefs ('strcmp()') */
}               t_hmap;

/**
 *	exemple d'utilisation:
 *
 *		t_hmap * hmap = hmap_new(500, strhash, strcmp);
 *		
 *		hmap_put(hmap, strdup("key"), strdup("value"));
 *		hmap_put(hmap, strdup("key2"), strdup("value2"));
 *		hmap_put(hmap, NULL, strdup("null string"));
 *
 *		t_hmap_entry * entry = hmap_remove(hmap, "key");
 *		free(entry->value);
 *		free(entry->key);
 *
 *		char * null_str = (char *) hmap_get(hmap, NULL);
 *		printf("%s\n", null_str);
 *
 *		hmap_delete(hmap);
 */

/**
 *	@require : 	capacity : capacity of the hashmap (number of lists boxes in memory)
 *  			hashf    : hash function to use on inserted elements
 *  			keycmpf  : comparison function to use when searching a data
 *	@ensure : renvoie une nouvelle table de hachage alloué sur le tas
 *	@assign : table initialement vide
 */
t_hmap * hmap_new(size_t capacity, t_hashf hashf, t_cmpf keycmpf);

/**
 *	@require	: hmap: table de hachage préalablement alloué via 'hmap_new()'
 *	@ensure		: supprime la table du tas
 *	@assign 	: ----------------
 *
 *	ATTENTION:	les clefs ou valeurs ne sont pas désalloués
 *			car elles n'ont pas été alloué par la hmap directement
 */
void hmap_delete(t_hmap * hmap);

/**
 *	@require : 	hmap 	: la table de hachage
 *  			key	: la clef
 *  			value	: la valeur
 *
 *	@ensure : insere une valeur dans la table.
 *			Si une valeur avec la meme clef était déjà inséré, la renvoie
 *			sinon, renvoie NULL
 *	@assign : ---------------------
 */
void * hmap_put(t_hmap * hmap, void const * key, void const * value);

/**
 *	@require : 	hmap 	: la table de hachage
 *  			key	: la clef
 *	@ensure : renvoie la valeur correspond à la clef, ou NULL si non trouvé.
 *	@assign : ---------------------
 */
void * hmap_get(t_hmap * hmap, void const * key);

/**
 *	@require : 	hmap 	: la table de hachage
 *  			key	: la clef
 *	@ensure : supprime l'entrée correspond à la clef, et renvoie l'entrée supprimée.
 *	@assign : ---------------------
 */
t_hmap_entry hmap_remove(t_hmap * hmap, void const * key);

/**
 *	@require : 	hmap 	: la table de hachage
 *  			value	: une valeur
 *	@ensure : renvoie la 1ère clef trouvé correspondant à la valeur, ou NULL si non trouvée
 *	@assign : ---------------------
 */
void * hmap_key_of(t_hmap * hmap, void const * value);

/**
 *	quelques fonctions de hachage pré-implémentées
 *
 *	(voir http://www.cse.yorku.ca/~oz/hash.html)
 */
size_t strhash(char const * str);
size_t inthash(int const value);

#endif
