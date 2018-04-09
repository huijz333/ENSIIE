package ensembles;

import java.util.Collection;

/**
 * Ensemble d'élements triés. Les élements doivent donc être des
 * {@link Comparable} afin de pouvoir réaliser l'insertion triée de nouveaux
 * éléments dans {@link #ajout(Comparable)}. A titre d'information les
 * {@link Integer} et les {@link String} sont des {@link Comparable}.
 * @author davidroussel
 */
public interface EnsembleTri<E extends Comparable<E>> extends Ensemble<E>
{
	/*
	 * Note : les redéfinitions ci-dessous ne sont pas techniquement nécessaires
	 * (sauf rang()) mais permettent de documenter les changements nécessaires
	 * dans la réimplémentation de ces méthodes spécifiquement pour les
	 * ensembles triés.
	 */

	/**
	 * Ajout d'un nouvel élément de manière à maintenir l'ensemble trié
	 * @param element l'élément à ajouter de manière triée
	 * @return true si l'élément n'était pas déjà présent dans l'ensemble, false
	 * sinon.
	 */
	@Override
	public abstract boolean ajout(E element);

	/**
	 * Code de hachage d'un ensemble trié. Il est nécessaire de réimplémenter le
	 * code de hachage pour les ensembles triés car on considèrera que deux
	 * ensembles contenant les même éléments mais dans des ordres différents
	 * seront eux-même différents. Il faut donc que la méthode hashCode prenne
	 * en compte l'ordre des éléments (Comme dans les autres {@link Collection}
	 * d'ailleurs).
	 * @return le code de hachage de cet ensemble trié.
	 * @see listes.Liste#hashCode() tableaux.Tableau#hashCode() pour un exemple
	 * de hashage utilisant l'ordre des éléments
	 */
	@Override
	public abstract int hashCode();

	/**
	 * Test d'égalité d'un ensemble trié. Il est nécessaire de réimplémenter la
	 * comparaison avec un autre ensemble car l'ordre des élément aura son
	 * importance dans la comparaison ce qui n'était pas le cas avec les
	 * ensembles non triés.
	 * @return true si l'objet obj est aussi un ensemble (pas forcément trié) et
	 * qu'il contient exactement les mêmes éléments dans le même ordre.
	 */
	@Override
	public abstract boolean equals(Object obj);

	/**
	 * Calcule le rang où doit être inséré un élément de manière triée dans
	 * l'ensemble trié
	 * @param Element l'élément dont on veut calculer le rang dans l'ensemble
	 * trié
	 * @return le rang d'insertion de l'élément dans l'ensemble trié
	 */
	public default int rang(E element)
	{
		/*
		 * calcul du rang d'un nouvel élément : On parcours les éléments de this
		 * et si un elt de this est plus grand que l'element à inserer ((elt de
		 * this).compareTo(element) >= 0) on a trouvé le rang où insérer, on
		 * quitte alors la boucle sans passer au suivant et on renvoie le nombre
		 * d'itérations effectuées. Cas limites : - element < 1er elt de this on
		 * quitte la boucle immédiatement - element > dernier elt de this la
		 * boucle va jusqu'au bout
		 */
		int rang = 0;
		for (E e : this) {
			if (e.compareTo(element) >= 0) {
				return (rang);
			}
			++rang;
		}
		return (rang);
	}
}
