package listes;

import java.util.Iterator;

/**
 * Interface d'une liste générique d'éléments.
 * 
 * @note On considèrera que la liste ne peut pas contenir d'elt null
 * @author David Roussel
 * @param <E>
 *            le type des éléments de la liste.
 */
public interface IListe<E> extends Iterable<E> {
	/**
	 * Ajout d'un élément en fin de liste
	 * 
	 * @param elt
	 *            l'élément à ajouter en fin de liste
	 * @throws NullPointerException
	 *             si l'on tente d'ajouter un élément null
	 */
	public abstract void add(E elt) throws NullPointerException;

	/**
	 * Insertion d'un élément en tête de liste
	 * 
	 * @param elt
	 *            l'élément à ajouter en tête de liste
	 * @throws NullPointerException
	 *             si l'on tente d'insérer un élement null
	 */
	public abstract void insert(E elt) throws NullPointerException;

	/**
	 * Insertion d'un élément à la (index+1)ième place
	 * 
	 * @param elt
	 *            l'élément à insérer
	 * @param index
	 *            l'index de l'élément à insérer
	 * @return true si l'élément a pu être inséré à l'index voulu, false sinon ou si
	 *         l'élément à insérer était null
	 */
	public abstract boolean insert(E elt, int index);

	/**
	 * Suppression de la première occurrence de l'élémént e
	 * 
	 * @param elt
	 *            l'élement à rechercher et à supprimer.
	 * @return true si l'élément a été trouvé et supprimé de la liste
	 * @note doit fonctionner même si e est null
	 */
	public default boolean remove(E elt) {
		Iterator<E> it = this.iterator();
		while (it.hasNext()) {
			E e = it.next();
			if (e == elt) {
				it.remove();
				return (true);
			}
		}
		return (false);
	}

	/**
	 * Suppression de toutes les instances de elt dans la liste
	 * 
	 * @param elt
	 *            l'élément à supprimer
	 * @return true si au moins un élément a été supprimé
	 * @note doit fonctionner même si e est null
	 */
	public default boolean removeAll(E elt) {
		boolean removed = false;

		Iterator<E> it = this.iterator();
		while (it.hasNext()) {
			E e = it.next();
			if (e.equals(elt)) {
				it.remove();
				removed = true;
			}
		}

		return removed;
	}

	/**
	 * Nombre d'éléments dans la liste
	 * 
	 * @return le nombre d'éléments actuellement dans la liste
	 */
	public default int size() {
		int count = 0;
		Iterator<E> it = this.iterator();
		while (it.hasNext()) {
			it.next();
			++count;
		}
		return count;
	}

	/**
	 * Effacement de la liste;
	 */
	public default void clear() {
		Iterator<E> it = this.iterator();
		while (it.hasNext()) {
			it.next();
			it.remove();
		}
	}

	/**
	 * Test de liste vide
	 * 
	 * @return true si la liste est vide, false sinon
	 */
	public default boolean empty() {
		return (this.size() == 0);
	}

	/**
	 * Test d'égalité au sens du contenu de la liste
	 * 
	 * @param o
	 *            la liste dont on doit tester le contenu
	 * @return true si o est une liste, que tous les maillons des deux listes sont
	 *         identiques (au sens du equals de chacun des maillons), dans le même
	 *         ordre, et que les deux listes ont la même longueur. false sinon
	 * @note On serait tenté d'en faire une "default method" dans la mesure où l'on
	 *       peut n'utiliser que l'itérateur pour parcourir les éléments de la liste
	 *       MAIS les méthodes par défaut n'ont pas le droit de surcharger les
	 *       méthodes de la superclasse Object.
	 */
	@Override
	public abstract boolean equals(Object o);

	/**
	 * HashCode d'une liste
	 * 
	 * @return le hashCode de la liste
	 * @note On serait tenté d'en faire une "default method" dans la mesure où l'on
	 *       peut n'utiliser que l'itérateur pour parcourir les éléments de la liste
	 *       MAIS les méthodes par défaut n'ont pas le droit de surcharger les
	 *       méthodes de la superclasse Object.
	 */
	@Override
	public abstract int hashCode();

	/**
	 * Représentation de la chaine sous forme de chaine de caractère.
	 * 
	 * @return une chaine de caractère représentant la liste chainée
	 * @note On serait tenté d'en faire une "default method" dans la mesure où l'on
	 *       peut n'utiliser que l'itérateur pour parcourir les éléments de la liste
	 *       MAIS les méthodes par défaut n'ont pas le droit de surcharger les
	 *       méthodes de la superclasse Object.
	 */
	@Override
	public abstract String toString();

	/**
	 * Obtention d'un itérateur pour parcourir la liste : <code>
	 * Liste<Type> l = new Liste<Type>();
	 * ...
	 * for (Iterator<Type> it = l.iterator(); it.hasNext(); )
	 * {
	 * 		... it.next() ...
	 * }
	 * ou bien
	 * for (Type elt : l)
	 * {
	 * 		... elt ...
	 * }
	 * </code>
	 * 
	 * @return un nouvel itérateur sur la liste
	 * @see {@link Iterable#iterator()}
	 */
	@Override
	public abstract Iterator<E> iterator();
}
