#ifndef LIST_H
# define LIST_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/**
 *	Structure de donnée: liste doublement chainée
 *	pouvant servir de file ou de liste
 */

typedef struct  s_list_node {
    struct s_list_node * next;
    struct s_list_node * prev;
}               t_list_node;

typedef struct  s_list {
    t_list_node         * head;
    unsigned long int   size;
}               t_list;

/**
 *	@require: ---------------------
 *	@ensure : renvoie une nouvelle liste doublement chainée
 *	@assign : ---------------------
 */
t_list * list_new(void);
 
/**
 *	@require: une liste, une donnée, et la taille de la donnée
 *	@ensure : ajoute une copie de la donnée en debut de liste
 *	@assign : ---------------------
 */
void * list_add(t_list * lst, void const * content, unsigned int content_size);

/**
 *	@require: une liste, une donnée, et la taille de la donnée
 *	@ensure : ajoute une copie de la donnée en fin de liste
 *	@assign : ---------------------
 */
void * list_push(t_list * lst, void const * content, unsigned int content_size);

/**
 *	@require: une liste
 *	@ensure : renvoie la donnée en tête de liste 
 *	@assign : ---------------------
 */
void * list_head(t_list * lst);

/**
 *	@require: une liste
 *	@ensure : renvoie la donnée en fin de liste 
 *	@assign : ---------------------
 */
void * list_tail(t_list * lst);

/**
 *	@require: une liste
 *	@ensure : supprime la donnée en tête de liste
 *	@assign : ---------------------
 */
int list_remove_head(t_list * lst);

/**
 *	@require: une liste
 *	@ensure : supprime la donnée en fin de liste
 *			renvoie 1 si une donnée a été supprimé, 0 sinon
 *	@assign : ---------------------
 */
int list_remove_tail(t_list * lst);

/**
 *	@require: une liste
 *	@ensure : supprime toutes les données de la liste
 *			renvoie 1 si une donnée a été supprimé, 0 sinon
 *	@assign : ---------------------
 */
void list_clear(t_list * lst);

/**
 *	@require: une liste
 *	@ensure : supprime la liste de la mémoire
 *	@assign : ---------------------
 */
void list_delete(t_list * lst);

#endif
