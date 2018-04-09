package listes;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * 
 * @author rpereira
 *
 *         Implementation de Liste doublement chainé circulaire
 *
 * @param <E>
 *            : n'importe quel object
 */
public class Liste<E> implements IListe<E> {

	private static class Maillon<E> {
		private Maillon<E> next;
		private Maillon<E> prev;
		private E data;

		public Maillon(E data) {
			this.data = data;
			this.next = this;
			this.prev = this;
		}

		public void setNext(Maillon<E> m) {
			this.next = m;
		}

		public void setPrev(Maillon<E> m) {
			this.prev = m;
		}

		public Maillon<E> getNext() {
			return (this.next);
		}

		public Maillon<E> getPrev() {
			return (this.prev);
		}

		public E getData() {
			return (this.data);
		}
	}

	private static class ListeIterator<E> implements Iterator<E> {

		private Liste<E> lst;
		private Maillon<E> current;

		public ListeIterator(Liste<E> lst) {
			this.lst = lst;
			this.current = lst.main;
		}

		@Override
		public boolean hasNext() {
			return (this.current.getNext() != this.lst.main);
		}

		@Override
		public E next() {
			if (!this.hasNext()) {
				throw new NoSuchElementException();
			}
			this.current = this.current.getNext();
			return (this.current.getData());
		}

		@Override
		public void remove() {
			this.lst.remove(this.current);
		}
	}

	private Maillon<E> main;
	private int size;

	public Liste() {
		this.main = new Maillon<E>(null);
		this.size = 0;
	}

	public Liste(Liste<E> toCopy) {
		this();
		for (E e : toCopy) {
			this.add(e);
		}
	}

	@Override
	public void add(E elt) throws NullPointerException {
		if (elt == null) {
			throw new NullPointerException();
		}
		Maillon<E> oldTail = this.main.getPrev();
		Maillon<E> newTail = new Maillon<E>(elt);

		newTail.setPrev(oldTail);
		newTail.setNext(this.main);

		this.main.setPrev(newTail);
		oldTail.setNext(newTail);

		++this.size;
	}

	@Override
	public void insert(E elt) throws NullPointerException {
		if (elt == null) {
			throw new NullPointerException();
		}
		Maillon<E> oldHead = this.main.next;
		Maillon<E> newHead = new Maillon<E>(elt);

		newHead.setPrev(this.main);
		newHead.setNext(oldHead);

		oldHead.setPrev(newHead);
		this.main.setNext(newHead);

		++this.size;
	}

	@Override
	public boolean insert(E elt, int index) {
		if (elt == null || index < 0 || index > this.size) {
			return (false);
		}

		Maillon<E> prevNode = this.main;
		int i = 0;
		while (i < index) {
			++i;
			prevNode = prevNode.getNext();
		}

		Maillon<E> newNode = new Maillon<E>(elt);
		newNode.setNext(prevNode.getNext());
		prevNode.getNext().setPrev(newNode);
		prevNode.setNext(newNode);
		newNode.setPrev(prevNode);

		++this.size;

		return (true);
	}

	private void remove(Maillon<E> node) {
		node.getPrev().setNext(node.next);
		node.getNext().setPrev(node.prev);
		--this.size;
	}

	@Override
	public Iterator<E> iterator() {
		return (new ListeIterator<E>(this));
	}

	@Override
	public int size() {
		return (this.size);
	}

	@Override
	public String toString() {
		if (this.size == 0) {
			return ("[]");
		}

		StringBuilder sb = new StringBuilder();
		sb.append("[");
		Maillon<E> cur = this.main.getNext();
		while (cur != this.main) {
			sb.append(cur.getData().toString());
			if (cur.getNext() != this.main) {
				sb.append("->");
			}
			cur = cur.getNext();
		}
		sb.append("]");
		return (sb.toString());
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == this) {
			return (true);
		}
		if (!(obj instanceof Iterable)) {
			return (false);
		}

		Iterator<E> it = this.iterator();
		for (Object e : (Iterable<?>) obj) {
			if (!it.hasNext()) {
				return (false);
			}
			if (!e.equals(it.next())) {
				return (false);
			}
		}
		return (!it.hasNext());

	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		for (E elt : this) {
			result = (prime * result) + (elt == null ? 0 : elt.hashCode());
		}
		return result;
	}
}
