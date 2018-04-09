package ensembles;

import java.util.Collection;
import java.util.Iterator;

/**
 * Implémentation générique partielle d'un ensemble trié sous forme de
 * décorateur d'un ensemble ordinaire.
 * @author davidroussel
 */
public abstract class EnsembleTriGenerique<E extends Comparable<E>>
extends EnsembleGenerique<E> implements EnsembleTri<E>
{
	/**
	 * Ensemble de base sous-jacent décoré par les ensembles triés.
	 */
	protected Ensemble<E> ensemble;

	/**
	 * Ajout d'un nouvel élément de manière à maintenir l'ensemble trié en
	 * utilisant la méthode {@link #insererAuRang(E element, int rang)} ssi
	 * l'élément peut être inséré dans cet ensemble trié
	 * @param element l'élément à ajouter de manière triée (on considèrera que
	 * l'on ne peut pas ajouter d'élément null)
	 * @return true si l'élément n'était pas déjà présent dans l'ensemble, false
	 * sinon ou si l'on a tenté d'insérer un élément null (auquel cas il
	 * n'est pas inséré).
	 * @see ensembles.EnsembleListe#ajout(java.lang.Object)
	 */
	@Override
	public boolean ajout(E element)
	{
		if (element == null || this.contient(element)) {
			return (false);
		}
		return (this.insererAuRang(element, this.rang(element)));
	}

	/**
	 * Insertion d'un nouvel élément au rang choisi en utilisant
	 * {@link #rang(E element)} pour calculer le rang d'insertion de l'élément
	 * @param element l'élement à insérer
	 * @param rang le rang où insérer cet élément
	 * @return true si l'élément a été inséré au rang choisi, false si l'élement
	 * n'a pas pu être inséré à cause d'un rang invalide
	 * @note On remarquera que la méthode ne teste pas au préalable l'existence
	 * de l'élément à insérer dans l'ensemble car c'est la méthode
	 * {@link #ajout(E)} qui s'en chargera
	 */
	protected abstract boolean insererAuRang(E element, int rang);

	/**
	 * Test d'égalité d'un ensemble trié. Il est nécessaire de réimplémenter la
	 * comparaison avec un autre ensemble car l'ordre des élément aura son
	 * importance dans la comparaison ce qui n'était pas le cas avec les
	 * ensembles non triés.
	 * @return true si l'objet obj est aussi un ensemble (pas forcément trié) et
	 * qu'il contient exactement les mêmes éléments dans le même ordre.
	 * @see ensembles.EnsembleGenerique#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj)
	{
		/*
		 *  Remplacer par ...
		 * 	1 - obj == null ? ==> false
		 * 	2 - obj == this ? ==> true
		 * 	3 - obj est une instance de Ensemble<?>
		 * 		- caster obj en Ensemble<?>
		 * 			- si obj et this ont exactement les mêmes éléments dans le même ordre ==> true
		 * 			- sinon ==> false;
		 * 	- sinon (obj n'est pas un Ensemble<?>) ==> false
		 */
		if (obj == null) {
			return (false);
		}
		if (obj == this) {
			return (true);
		}
		if (obj instanceof Ensemble<?>) {
			Ensemble<?> ens = (Ensemble<?>) obj;
			if (this.cardinal() != ens.cardinal()) {
				return (false);
			}
			Iterator<?> it1 = ens.iterator();
			Iterator<?> it2 = this.iterator();
			while (it1.hasNext() && it2.hasNext()) {
				Object e1 = it1.next();
				Object e2 = it2.next();
				if (!e1.equals(e2)) {
					return (false);
				}
			}
			return (true);
		}
		return (false);
	}

	/**
	 * Code de hachage d'un ensemble trié. Il est nécessaire de réimplémenter le
	 * code de hachage pour les ensembles triés car on considèrera que deux
	 * ensembles contenant les même éléments mais dans des ordres différents
	 * seront eux-même différents. Il faut donc que la méthode hashCode prenne
	 * en compte l'ordre des éléments (Comme dans les autres {@link Collection}
	 * d'ailleurs).
	 * @return le code de hachage de cet ensemble trié.
	 * @see listes.Liste#hashCode() ou tableaux.Tableau#hashCode() pour un
	 * exemple de hashage utilisant l'ordre des éléments
	 * @see ensembles.EnsembleGenerique#hashCode()
	 */
	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		for (E elt : this)
		{
			result = (prime * result) + (elt == null ? 0 : elt.hashCode());
		}
		return result;
	}

	/**
	 * Union avec un autre ensemble : reste semblable à l'union avec avec un
	 * ensemble non trié mais s'applique sur l'ensemble décoré {@link #ensemble}
	 * @param ensemble l'autre ensemble avec lequel on veut créer une union
	 * @return un nouvel ensemble contenant l'union de l'ensemble courant et de
	 * l'ensemble passé en argument
	 * @see ensembles.EnsembleGenerique#union(ensembles.Ensemble)
	 */
	@Override
	public Ensemble<E> union(Ensemble<E> autre)
	{
		return (this.ensemble.union(autre));
	}

	/**
	 * Intersection avec un autre ensemble : reste semblable à l'intersection
	 * avec avec un ensemble non trié mais s'applique sur l'ensemble décoré
	 * {@link #ensemble}
	 * @param ensemble l'autre ensemble avec lequel on veut créer une
	 * intersection
	 * @return un nouvel ensemble contenant l'intersection de l'ensemble courant
	 * et de l'ensemble passé en argument
	 * @see ensembles.EnsembleGenerique#intersection(ensembles.Ensemble)
	 */
	@Override
	public Ensemble<E> intersection(Ensemble<E> autre)
	{
		return (this.ensemble.intersection(autre));
	}

	/**
	 * Complement avec un autre ensemble : reste semblable au complement avec
	 * avec un ensemble non trié mais s'applique sur l'ensemble décoré
	 * {@link #ensemble}
	 * @param ensemble l'autre ensemble avec lequel on veut créer un complement
	 * @return un nouvel ensemble contenant le complement de l'ensemble courant
	 * et de l'ensemble passé en argument
	 * @see ensembles.EnsembleGenerique#complement(ensembles.Ensemble)
	 */
	@Override
	public Ensemble<E> complement(Ensemble<E> autre)
	{
		return (this.ensemble.complement(autre));
	}
	
	/**
	 * Factory method fournissant un itérateur sur l'ensemble en réutilisant
	 * l'itérateur de l'ensemble ordinaire sous-jacent.
	 * @return un nouvel itérateur sur cet ensemble
	 * @see ensembles.EnsembleGenerique#iterator()
	 */
	@Override
	public Iterator<E> iterator()
	{
		return (this.ensemble.iterator());
	}
}
