/**
 *  This file is part of https://github.com/rpereira-dev/C_data_structures
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  PEREIRA Romain
 */

# include "hmap.h"

/**
 *	@require : 	capacity : capacity of the hashmap (number of lists boxes in memory)
 *  			hashf    : hash function to use on inserted elements
 *  			keycmpf  : comparison function to use when searching a data
 *	@ensure : renvoie une nouvelle table de hachage alloué sur le tas
 *	@assign : table initialement vide
 */
t_hmap * hmap_new(size_t capacity, t_hashf hashf, t_cmpf keycmpf) {
	t_hmap * hmap = (t_hmap *) malloc(sizeof(t_hmap));
	if (hmap == NULL) {
		return (NULL);
	}
	/* on s'assure que la capacité de départ sois une puissance de 2 */
	size_t c = 1;
	while (c < capacity) {
		c =  c << 1;
	}

	/* on crée la map, et on met les pointeurs sur NULL */
	hmap->entries = (t_array **)malloc(sizeof(t_array *) * c);
	if (hmap->entries == NULL) {
		free(hmap);
		return (NULL);
	}
	memset(hmap->entries, 0, sizeof(t_array *) * c);

	hmap->capacity = c;
	hmap->hashf = hashf;
	hmap->keycmpf = keycmpf;
	return (hmap);
}

/**
 *	@require	: hmap: table de hachage préalablement alloué via 'hmap_new()'
 *	@ensure		: supprime la table du tas
 *	@assign 	: ----------------
 *
 *	ATTENTION	: les clefs ou valeurs ne sont pas désalloués
 *			car elles n'ont pas été alloué par le hmap directement
 */
void hmap_delete(t_hmap * hmap) {
	free(hmap->entries);
	free(hmap);
}

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
void * hmap_put(t_hmap * hmap, void const * key, void const * value) {
	 /* d'où la puissance de 2 */
	size_t hash = hmap->hashf(key);
	size_t addr = hash & (hmap->capacity - 1);

	t_hmap_entry entry = {hash, (void*)key, (void*)value};
	t_array * sameHashes = hmap->entries[addr];
	
	/* si aucunes valeurs avec ce hash n'était enregistré avant... */
	if (sameHashes == NULL) {
		sameHashes = array_new(1, sizeof(t_hmap_entry));
		if (sameHashes == NULL) {
			return (NULL);
		}
		hmap->entries[addr] = sameHashes;
		array_add(sameHashes, &entry);
		return (NULL);
	}

	/* sinon, on regarde si une valeur avec la meme clef existe, si oui, on l'écrase */
	ARRAY_ITERATE_START(sameHashes, t_hmap_entry *, entry, i) {
		if (hmap->keycmpf(key, entry->key) == 0) {
			void * prevValue = entry->value;
			entry->value = (void*)value;
			return (prevValue);
		}
	}
	ARRAY_ITERATE_STOP(sameHashes, t_hmap_entry *, entry, i);
	return (NULL);
}

/**
 *	@require : 	hmap 	: la table de hachage
 *  			key	: la clef
 *	@ensure : renvoie la valeur correspond à la clef, ou NULL si non trouvé.
 *	@assign : ---------------------
 */
void * hmap_get(t_hmap * hmap, void const * key) {
	size_t hash = hmap->hashf(key);
	size_t addr = hash & (hmap->capacity - 1);

	t_array * sameHashes = hmap->entries[addr];
	
	/* si aucunes valeurs avec ce hash n'était enregistré avant... */
	if (sameHashes == NULL) {
		return (NULL);
	}

	/* s'il n'y a qu'une valeur => qu'une clef */
	if (sameHashes->size == 1) {
		t_hmap_entry * entry = (t_hmap_entry *)array_get(sameHashes, 0);
		return (entry->value);
	}

	/* sinon, on regarde si une valeur avec la meme clef existe */
	ARRAY_ITERATE_START(sameHashes, t_hmap_entry *, entry, i) {
		if (hmap->keycmpf(key, entry->key) == 0) {
			return (entry->value);
		}
	}
	ARRAY_ITERATE_STOP(sameHashes, t_hmap_entry *, entry, i);
	return (NULL);
}

/**
 *	@require : 	hmap 	: la table de hachage
 *  			key	: la clef
 *	@ensure : supprime l'entrée correspond à la clef, et renvoie l'entrée supprimée.
 *	@assign : ---------------------
 */
t_hmap_entry hmap_remove(t_hmap * hmap, void const * key) {
	size_t hash = hmap->hashf(key);
	size_t addr = hash & (hmap->capacity - 1);
	t_array * sameHashes = hmap->entries[addr];
	
	/* si aucunes valeurs avec ce hash n'était enregistré avant... */
	if (sameHashes == NULL) {
		t_hmap_entry nullEntry = {0, NULL, NULL};
		return (nullEntry);
	}

	/* s'il n'y a qu'une valeur => qu'une clef */
	if (sameHashes->size == 1) {
		t_hmap_entry * entryAddr = (t_hmap_entry *)array_get(sameHashes, 0);
		t_hmap_entry entry = *entryAddr;
		array_delete(sameHashes);
		hmap->entries[addr] = NULL;
		return (entry);
	}

	/* sinon, on cherche la valeur */
	ARRAY_ITERATE_START(sameHashes, t_hmap_entry *, entryAddr, i) {
		if (hmap->keycmpf(entryAddr->key, key) == 0) {
			/* trouvé */
			t_hmap_entry entry = *entryAddr;
			array_remove(sameHashes, i);
			return (entry);
		}
	}
	ARRAY_ITERATE_STOP(sameHashes, t_hmap_entry *, entry, i);

	/* pas trouvé */
	t_hmap_entry nullEntry = {0, NULL, NULL};
	return (nullEntry);
}

/**
 *	@require : 	hmap 	: la table de hachage
 *  			value	: une valeur
 *	@ensure : renvoie la 1ère clef trouvé correspondant à la valeur, ou NULL si non trouvée
 *	@assign : ---------------------
 */
void * hmap_key_of(t_hmap * hmap, void const * value) {
	size_t i;
	for (i = 0 ; i < hmap->capacity ; i++) {
		t_array * values = hmap->entries[i];
		if (values == NULL) {
			continue ;
		}
		ARRAY_ITERATE_START(values, t_hmap_entry *, entry, i) {
			if (entry->value == value) {
				return (entry->key);
			}
		}
		ARRAY_ITERATE_STOP(values, t_hmap_entry *, entry, i);
	}
	return (NULL);
}

/**
 *	quelques fonctions de hachage pré-implémentées
 *
 *	(voir http://www.cse.yorku.ca/~oz/hash.html)
 */
size_t strhash(char const * str) {
	if (str == NULL) {
		return (0);
	}
	size_t hash = 5381;
	while (*str) {
		hash = ((hash << 5) + hash) + *str++;
	}
	return (hash);
}

size_t inthash(int const value) {
	return ((size_t)value);
}

