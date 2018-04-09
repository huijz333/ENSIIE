package ensembles;

import listes.Liste;
import tableaux.Tableau;

/**
 * Ensemble trié utilisant un {@link Tableau}
 * 
 * @author davidroussel
 */
public class EnsembleTriListe2<E extends Comparable<E>> extends EnsembleTriGenerique<E> {
	public EnsembleTriListe2() {
		super.ensemble = new EnsembleListe<E>();
	}

	public EnsembleTriListe2(Iterable<E> elements) {
		this();
		for (E e : elements) {
			this.ajout(e);
		}
	}

	@Override
	protected boolean insererAuRang(E element, int rang) {
		try {
			Liste<E> liste = ((EnsembleListe<E>) super.ensemble).container;
			liste.insert(element, rang);
			return (true);
		} catch (Exception e) {
			return (false);
		}
	}
}
