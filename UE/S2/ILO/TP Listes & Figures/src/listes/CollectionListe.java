package listes;

import java.util.AbstractCollection;
import java.util.Collection;
import java.util.Iterator;

public class CollectionListe<E> extends AbstractCollection<E> {

	private Liste<E> liste;

	public CollectionListe() {
		this.liste = new Liste<E>();
	}

	public CollectionListe(Collection<E> collection) {
		this.liste = new Liste<E>();
		for (E e : collection) {
			this.liste.add(e);
		}
	}

	@Override
	public Iterator<E> iterator() {
		return (this.liste.iterator());
	}

	@Override
	public boolean add(E elt) {
		try {
			this.liste.add(elt);
		} catch (Exception e) {
			return (false);
		}
		return (true);
	}

	@Override
	public int size() {
		return (this.liste.size());
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == this) {
			return (true);
		}
		if (!(obj instanceof Collection)) {
			return (false);
		}
		if (((Collection<?>) obj).size() != this.size()) {
			return (false);
		}
		Iterator<E> it = this.iterator();
		for (Object e : (Collection<?>) obj) {
			if (!e.equals(it.next())) {
				return (false);
			}
		}
		return (true);
	}

	@Override
	public int hashCode() {
		return (this.isEmpty() ? 0 : super.hashCode());
	}
}
