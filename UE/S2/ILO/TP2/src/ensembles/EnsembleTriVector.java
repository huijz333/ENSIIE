package ensembles;

import java.util.Collection;
import java.util.Iterator;

import tableaux.Tableau;

/**
 * Ensemble trié utilisant un {@link Tableau}
 * @author davidroussel
 */
public class EnsembleTriVector<E extends Comparable<E>> extends
		EnsembleVector<E> implements EnsembleTri<E>
{

	/**
	 * Constructeur par défaut d'un ensemble trié utilisant un {@link Tableau}
	 */
	public EnsembleTriVector()
	{
		super();
	}

	/**
	 * Constructeur de copie à partir d'un autre iterable
	 * @param elements l'itérable dont on veut copier les éléments
	 */
	public EnsembleTriVector(Iterable<E> elements)
	{
		super(elements);
	}

	/**
	 * Ajout d'un élément de manière triée dans l'ensemble utilisant un
	 * {@link Tableau}
	 * @param element l'élément à ajouter de manière triée (on considèrera que
	 * l'on ne peut pas ajouter d'élément null)
	 * @return true si l'élément n'était pas déjà présent dans l'ensemble, false
	 * sinon ou si l'on a tenté d'insérer un élément null (auquel cas il
	 * n'est pas inséré).
	 * @see ensembles.EnsembleTableau#ajout(java.lang.Object)
	 * @see tableaux.Tableau#insertElement(E, int)
	 */
	@Override
	public boolean ajout(E element)
	{
		if (element == null || this.contient(element)) {
			return (false);
		}
		super.vector.add(this.rang(element), element);
		return (true);
	}

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
		 * 		- si obj et this ont exactement les mêmes éléments dans le
		 * 		  même ordre ==> true
		 * 		- sinon ==> false;
		 * 	  - sinon (obj n'est pas un Ensembble<?>) ==> false
		 */
		if (obj == null) {
			return (false);
		}
		if (obj == this) {
			return (true);
		}
		if (obj instanceof Ensemble<?>) {
			Ensemble<?> ens = (Ensemble<?>)obj;
			if (this.cardinal() != ens.cardinal()) {
				return (false);
			}
			Iterator<?> it1 = ens.iterator();
			Iterator<?> it2 = this.iterator();
			while (it1.hasNext()) {
				if (!it1.next().equals(it2.next())) {
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
	 * @see tableaux.Tableau#hashCode() pour un exemple de hashage utilisant
	 * l'ordre des éléments
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
}
