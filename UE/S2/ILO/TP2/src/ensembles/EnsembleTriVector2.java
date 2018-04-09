package ensembles;

import java.util.Vector;

/**
 * Ensemble trié utilisant un {@link Vector}
 * 
 * @author davidroussel
 */
public class EnsembleTriVector2<E extends Comparable<E>> extends EnsembleTriGenerique<E> {
	public EnsembleTriVector2() {
		super.ensemble = new EnsembleVector<E>();
	}
	
	public EnsembleTriVector2(Iterable<E> elements) {
		this();
		for (E e : elements) {
			this.ajout(e);
		}
	}

	@Override
	protected boolean insererAuRang(E element, int rang) {
		try {
			Vector<E> vector = ((EnsembleVector<E>) super.ensemble).vector;
			vector.add(rang, element);
			return (true);
		} catch (Exception e) {
			return (false);
		}
	}
}
