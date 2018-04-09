package ensembles;

import java.util.Iterator;

import listes.Liste;

public class EnsembleListe<E> extends EnsembleGenerique<E> {

	protected final Liste<E> container;

	public EnsembleListe() {
		this.container = new Liste<E>();
	}

	public EnsembleListe(Iterable<E> it) {
		this();
		for (E e : it) {
			this.ajout(e);
		}
	}

	@Override
	public boolean ajout(E element) {
		if (element == null || this.contient(element)) {
			return (false);
		}
		this.container.add(element);
		return (true);
	}

	@Override
	public Ensemble<E> union(Ensemble<E> ensemble) {
		Ensemble<E> res = new EnsembleListe<E>();
		Ensemble.union(this, ensemble, res);
		return (res);
	}

	@Override
	public Ensemble<E> intersection(Ensemble<E> ensemble) {
		Ensemble<E> res = new EnsembleListe<E>();
		Ensemble.intersection(this, ensemble, res);
		return (res);
	}

	@Override
	public Ensemble<E> complement(Ensemble<E> ensemble) {
		Ensemble<E> res = new EnsembleListe<E>();
		Ensemble.complement(this, ensemble, res);
		return (res);
	}

	@Override
	public Iterator<E> iterator() {
		return (this.container.iterator());
	}

}
