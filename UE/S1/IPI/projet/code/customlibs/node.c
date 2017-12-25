# include "node.h"

/**
 *	@require : un tableau renvoyé par "node_build_path()" et un flux
 *	@ensure  : affiche le chemin du tableau sur le flux
 *	@assign  : ---------------------------------
 */
void node_write_path(t_array * path, FILE * stream) {
	ARRAY_ITERATE_START(path, INDEX *, indexRef, i) {
		fprintf(stream, INDEX_IDENTIFIER "\n", *indexRef + 1);
	};
	ARRAY_ITERATE_STOP(path, INDEX *, index, i);
}

/**
 *	@require : les sommets résolus, l'indice source, et l'indice de destination
 *	@ensure  : renvoie un tableau qui contient le chemin allant de 's' à 't'
 *	@assign  : ---------------------------------
 */
t_array * node_build_path(t_array * nodes, INDEX s, INDEX t) {
	INDEX pathlen = ((t_node *)array_get(nodes, t))->pathlen;

	if (pathlen == MAX_NODES) {
		return (NULL);
	}

	/* le tableau contenant le chemin */
	t_array * array = array_new(pathlen + 1, sizeof(INDEX));
	if (array == NULL) {
		return (NULL);
	}
	
	/* on construit le chemin */
	INDEX i = t;
	while (i != s) {
		array_add(array, &i);
		i = ((t_node *)array_get(nodes, i))->prev;
	}
	array_add(array, &s);
	array_reverse(array);
	return (array);
}

/**
 *	@require : 	'nodes' : sommets resolu par un algorithme du plus court chemin
 *		   	's'	: sommet source de la resolution
 *			't'	: sommet vers lequel on construit le chemin
 *			'stream': le flux
 *	@ensure	 : génère et affiche le chemin sur le flux donnée
 *		   à partir des sommets resolus par un algorithme
 *	@assign	 : ----------------
 */
void node_solve_path(t_array * nodes, INDEX s, INDEX t, FILE * stream) {
	t_array * path = node_build_path(nodes, s, t);
	if (path == NULL) {
		printf("Not connected\n");
		return ;
	}
	node_write_path(path, stream);
	array_delete(path);
}

/**
 *	@require : 	'n'		: un nombre de sommets
 *		   	'nodeSize'	: taille d'un sommet en mémoire
 *	@ensure	 : génère et renvoie un tableau de 'n' sommet non initialisé
 *	@assign	 : ----------------
 */
t_array * nodes_new(INDEX n, size_t nodeSize) {
	t_array * nodes = array_new(n, nodeSize);
	if (nodes == NULL) {
		return (NULL);
	}
	array_addempty(nodes, n); /* on ajoute 'n' sommets vides */
	return (nodes);
}
