package ensembles;

import tableaux.Tableau;

/**
 * Ensemble trié utilisant un {@link Tableau}
 * 
 * @author davidroussel
 */
public class EnsembleTriTableau2<E extends Comparable<E>> extends EnsembleTriGenerique<E> {
	public EnsembleTriTableau2() {
		super.ensemble = new EnsembleTableau<E>();
	}
	
	public EnsembleTriTableau2(Iterable<E> elements) {
		this();
		for (E e : elements) {
			this.ajout(e);
		}
	}

	@Override
	protected boolean insererAuRang(E element, int rang) {
		try {
			Tableau<E> tableau = ((EnsembleTableau<E>) super.ensemble).tableau;
			tableau.insertElement(element, rang);
			return (true);
		} catch (Exception e) {
			return (false);
		}
	}
}
