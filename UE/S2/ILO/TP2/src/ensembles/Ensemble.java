package ensembles;

import java.util.Iterator;

/**
 * Interface définissant un ensemble comme une collection non triée d'éléments
 * sans doublons. Le fait que les éléments sont considérés comme non triés
 * impliquera que la comparaison de deux ensembles ne devra pas prendre en
 * compte l'ordre (apparent) des éléments.
 * 
 * @author davidroussel
 */
public interface Ensemble<E> extends Iterable<E> {
	/**
	 * Ajout d'un élément à un ensemble ssi celui ci n'est pas null et qu'il n'est
	 * pas déjà présent
	 * 
	 * @param element
	 *            l'élément à ajouter à l'ensemble (on considèrera que l'on ne peut
	 *            pas ajouter d'élément null)
	 * @return true si l'élément a pu être ajouté à l'ensemble, false sinon ou si
	 *         l'on a tenté d'insérer un élément null (auquel cas il n'est pas
	 *         inséré)
	 */
	public abstract boolean ajout(E element);

	/**
	 * Retrait d'un élément de l'ensemble en utilisant le remove de l'itérateur
	 * fournit par {@link #iterator()}
	 * 
	 * @param element
	 *            l'élément à supprimer de l'ensemble
	 * @return true si l'élément était présent dans l'ensemble (au sens de la
	 *         comparaison profonde) et qu'il a été retiré, false sinon
	 */
	public default boolean retrait(E element) {
		Iterator<E> it = this.iterator();
		while (it.hasNext()) {
			if (it.next().equals(element)) {
				it.remove();
				return (true);
			}
		}
		return (false);
	}

	/**
	 * Teste si l'ensemble est vide en utilisant l'itérateur ou bien le
	 * {@link #cardinal}
	 * 
	 * @return renvoie true si l'ensemble ne contient aucun élément, false sinon
	 * @see ensembles.Ensemble#estVide()
	 * @note Attention, si l'on utilise cardinal dans estVide, il ne faut pas
	 *       utiliser estVide dans cardinal et vice versa.
	 */
	public default boolean estVide() {
		return (this.cardinal() == 0);
	}

	/**
	 * Test d'appartenance d'un élément à l'ensemble en utilisant l'itérateur pour
	 * parcourir les éléments
	 * 
	 * @param element
	 *            l'élément dont on doit tester l'appartenance
	 * @return true si l'élément est présent dans l'ensemble (au sens de la
	 *         comparaison profonde), false sinon
	 */
	public default boolean contient(E element) {
		Iterator<E> it = this.iterator();
		while (it.hasNext()) {
			if (it.next().equals(element)) {
				return (true);
			}
		}
		return (false);
	}

	/**
	 * Test si ensemble est un sous-ensemble de l'ensemble courant. C'est à dire si
	 * l'ensemble courant contient tous les éléments de l'ensemble passé en argument
	 * 
	 * @note Si l'ensemble passé en argument est null il ne sera pas considéré comme
	 *       contenu.
	 * @param ensemble
	 *            l'ensemble dont on veut tester s'il est un sous ensemble de
	 *            l'ensemble courant
	 * @return true si ensemble est un sous-ensemble de l'ensemble courant, false
	 *         sinon. false si ensemble est null.
	 */
	public default boolean contient(Ensemble<E> ensemble) {
		if (ensemble == null) {
			return (false);
		}
		Iterator<E> it = ensemble.iterator();
		while (it.hasNext()) {
			if (!this.contient(it.next())) {
				return (false);
			}
		}
		return (true);
	}

	/**
	 * Efface tous les élements de l'ensemble en utilisant le remove de l'itérateur
	 * fournit par {@link #iterator()}
	 */
	public default void efface() {
		Iterator<E> it = this.iterator();
		while (it.hasNext()) {
			it.next();
			it.remove();
		}
	}

	/**
	 * Taille de l'ensemble en utilisant l'itérateur
	 * 
	 * @return le nombre d'éléments dans l'ensemble
	 * @see ensembles.Ensemble#cardinal() Attention : si l'on utilise estVide dans
	 *      cardinal, il ne faut pas utiliser cardinal dans estVide
	 * @note Cette méthode aura intérêt à être réimplémentée dans les classes filles
	 *       qui utilisent des conteneurs pouvant donner leur taille directement
	 */
	public default int cardinal() {
		int c = 0;
		Iterator<E> it = this.iterator();
		while (it.hasNext()) {
			it.next();
			c++;
		}
		return (c);
	}

	/**
	 * Union avec un autre ensemble : (this union ensemble).
	 * 
	 * @param ensemble
	 *            l'autre ensemble avec lequel on veut créer une union
	 * @return un nouvel ensemble contenant l'union de l'ensemble courant et de
	 *         l'ensemble passé en argument
	 */
	public abstract Ensemble<E> union(Ensemble<E> ensemble);

	/**
	 * Implémentation de classe de l'union de deux ensemble dans un autre ensemble
	 * 
	 * @param ens1
	 *            le premier ensemble
	 * @param ens2
	 *            le second ensemble
	 * @param res
	 *            l'ensemble contenant l'union de ens1 et ens2
	 */
	public static <E> void union(Ensemble<E> ens1, Ensemble<E> ens2, Ensemble<E> res) {
		Iterator<E> it = ens1.iterator();
		while (it.hasNext()) {
			res.ajout(it.next());
		}

		it = ens2.iterator();
		while (it.hasNext()) {
			res.ajout(it.next());
		}
	}

	/**
	 * Intersection avec un autre ensemble : (this inter ensemble).
	 * 
	 * @param ensemble
	 *            l'autre ensemble avec lequel on veut créer une intersection
	 * @return un nouvel ensemble contenant l'intersection de l'ensemble courant et
	 *         de l'ensemble passé en argument
	 */
	public abstract Ensemble<E> intersection(Ensemble<E> ensemble);

	/**
	 * Implémentation de classe de l'intersection de deux ensemble dans un autre
	 * ensemble
	 * 
	 * @param ens1
	 *            le premier ensemble
	 * @param ens2
	 *            le second ensemble
	 * @param res
	 *            l'ensemble contenant l'intersection de ens1 et ens2
	 */
	public static <E> void intersection(Ensemble<E> ens1, Ensemble<E> ens2, Ensemble<E> res) {
		Iterator<E> it = ens1.iterator();
		while (it.hasNext()) {
			E e = it.next();
			if (ens2.contient(e)) {
				res.ajout(e);
			}
		}
	}

	/**
	 * Complément avec un autre ensemble : (this - ensemble).
	 * 
	 * @param ensemble
	 *            l'autre ensemble avec lequel on veut créer le complément
	 * @return un nouvel ensemble contenant uniquement les éléments présents dans
	 *         l'ensemble courant mais PAS dans l'ensemble passé en argument
	 */
	public abstract Ensemble<E> complement(Ensemble<E> ensemble);

	/**
	 * Implémentation de classe du complément de deux ensembles dans un autre
	 * ensemble.
	 * 
	 * @param ens1
	 *            le premier ensemble
	 * @param ens2
	 *            le second ensemble
	 * @param res
	 *            l'ensemble contenant le complément de ens1 - ens2
	 */
	public static <E> void complement(Ensemble<E> ens1, Ensemble<E> ens2, Ensemble<E> res) {
		Iterator<E> it = ens1.iterator();
		while (it.hasNext()) {
			E e = it.next();
			if (!ens2.contient(e)) {
				res.ajout(e);
			}
		}
	}

	/**
	 * Différence symmétrique avec un autre ensemble : (this delta ensemble).
	 * L'ensemble correspondant à la différence symétrique contient les élément qui
	 * sont soit dans l'ensemble courant, soit dans l'autre ensemble mais pas dans
	 * les deux ensembles = (this - ensemble) union (ensemble - this)
	 * 
	 * @param ensemble
	 *            l'autre ensemble avec lequel on veut créer une différence
	 *            symétrique
	 * @return un nouvel ensemble contenant la différence symmétrique de l'ensemble
	 *         courant et de l'ensemble passé en argument
	 * @see ensembles.Ensemble#difference(ensembles.Ensemble)
	 */
	public default Ensemble<E> difference(Ensemble<E> ensemble) {
		/* Soit (A ∪ B) - (B ∩ A) */
		return (this.union(ensemble).complement(ensemble.intersection(this)));
		/* (A - B) ∪ (B - A) */
		// return (this.complement(ensemble).union(ensemble.complement(this)));
	}

	/**
	 * Type des élements de l'ensemble
	 * 
	 * @return une instance de la classe Class représentant le type des élements de
	 *         l'ensemble si celui ci n'est pas vide, ou bien null si l'ensemble est
	 *         vide.
	 * @note cette méthode sera utile dans l'implémentation de la méthode
	 *       {@link #equals(Object)} pour déterminer si deux ensembles ont le même
	 *       type d'éléments
	 * @see ensembles.Ensemble#typeElements()
	 */
	@SuppressWarnings("unchecked")
	public default Class<E> typeElements() {
		Iterator<E> it = iterator();
		if (it != null) {
			if (it.hasNext()) {
				return (Class<E>) it.next().getClass();
			}
		}

		return null;
	}

	// ------------------------------------------------------------------------
	// Méthodes à implémenter définies dans la classe Object
	// ------------------------------------------------------------------------

	/**
	 * Test d'égalité entre deux ensembles
	 * 
	 * @param o
	 *            l'objet à comparer
	 * @return true si l'objet à comparer est un ensemble et qu'il contient les
	 *         mêmes éléments (pas forcément dans le même ordre). Si les deux
	 *         ensembles sont vides on considère qu'ils seront égaux quel que soit
	 *         leur type de contenu (dans la mesure où l'on ne peut pas le
	 *         déterminer avec {@link ensembles.Ensemble#typeElements()}
	 * @note une interface ne peut pas implémenter par défaut des méthodes
	 *       surchargées de la classe object (celles ci dépendant de l'état interne
	 *       des objets, ce qui n'est pas le cas d'une interface)
	 */
	@Override
	public abstract boolean equals(Object o);

	/**
	 * Hashcode d'un ensemble. Le HashCode d'un ensemble doit être calculé comme
	 * étant la somme des hascodes de ses éléments afin de ne pas tenir compte de
	 * l'ordre des éléments dans la collection sous-jacente.
	 * 
	 * @return le hashage d'un ensemble
	 * @note une interface ne peut pas implémenter par défaut des méthodes
	 *       surchargées de la classe object (celles ci dépendant de l'état interne
	 *       des objets, ce qui n'est pas le cas d'une interface)
	 */
	@Override
	public abstract int hashCode();

	/**
	 * Affichage des élement de l'ensemble sous la forme : par exemple pour un
	 * ensemble de 3 elts : "[elt1, elt2, elt3]" où eltn représente le toString du
	 * nième elt.
	 * 
	 * @return une chaine de caractère représentant les éléments de l'ensemble
	 *         séparé par des virgules et encadré par des crochets
	 * @note une interface ne peut pas implémenter par défaut des méthodes
	 *       surchargées de la classe object (celles ci dépendant de l'état interne
	 *       des objets, ce qui n'est pas le cas d'une interface)
	 */
	@Override
	public abstract String toString();

	// ------------------------------------------------------------------------
	// Méthodes à implémenter définies dans l'interface Iterable<E>
	// ------------------------------------------------------------------------
	/**
	 * Factory method fournissant un itérateur sur l'ensemble
	 * 
	 * @return un nouvel itérateur sur cet ensemble
	 */
	@Override
	public abstract Iterator<E> iterator();
}
