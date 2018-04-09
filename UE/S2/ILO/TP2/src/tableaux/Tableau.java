package tableaux;

import java.util.Collection;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Tableau de données
 *
 * @author davidroussel
 * @param <E> le type des données stockées dans le tableau
 */
public class Tableau<E> implements Iterable<E>
{
	/**
	 * Le tableau de données
	 */
	protected E[] table;

	/**
	 * nombre d'éléments actuellement dans le tableau. Et index du prochain
	 * élément à insérer
	 */
	protected int size;

	/**
	 * Nombre de cases max du tableau
	 */
	protected int capacity;

	/**
	 * Nombres de cases initiales par défaut du tableau de données. Et nombre de
	 * cases à rajouter en cas de manque de cases
	 */
	public static final int INCREMENT = 5;

	/**
	 * constructeur par défaut d'un tableau de données
	 */
	@SuppressWarnings("unchecked")
	public Tableau()
	{
		table = (E[]) new Object[INCREMENT];
		size = 0;
	}

	/**
	 * constructeur de copie à partir d'un autre {@link Iterable}
	 * @param elements l'itérable dont on doit copier les éléments
	 */
	public Tableau(Iterable<E> elements)
	{
		this();
		for (E elt : elements)
		{
			ajouter(elt);
		}
	}

	/**
	 * Nombre d'éléments actuellement dans le tableau
	 * @return Le nombre d'éléments actuellement dans le tableau
	 */
	public int taille()
	{
		return size;
	}

	/**
	 * Nombre d'éléments maximum (actuellement) dans le tableau
	 * @return le nombre de l'éléments amx dans le tableau actuellement
	 */
	public int capacite()
	{
		return capacity;
	}

	/**
	 * Ajout d'un element à la fin du tableau
	 * @param element l'élément à insérer
	 */
	public void ajouter(E element)
	{
		if (size >= capacity)
		{
			// ajouterCapacite(Math.max(INCREMENT, (size - capacity) + 1));
			int sc1 = (size - capacity) + 1;
			ajouterCapacite((INCREMENT >= sc1 ? INCREMENT : sc1));
		}
		table[size] = element;
		size++;
	}

	/**
	 * Ajout de nbCases au tableau
	 * @param nbCases nombre de cases à ajouter.
	 */
	protected void ajouterCapacite(int nbCases)
	{
		if (nbCases > 0)
		{
			capacity += nbCases;
			@SuppressWarnings("unchecked")
			E[] newTable = (E[]) new Object[capacity];
			for (int i = 0; i < size; i++)
			{
				newTable[i] = table[i];
				table[i] = null; // avoid weak references
			}
			table = newTable;
		}
	}

	/**
	 * Retrait de la prmière occurrence d'un élément
	 * @param element l'élément à retirer du tableau
	 * @return true si l'élément a été trouvé et retiré
	 */
	public boolean retrait(E element)
	{
		for (Iterator<E> it = iterator(); it.hasNext();)
		{
			if (it.next().equals(element))
			{
				it.remove();
				return true;
			}
		}

		return false;
	}

	/**
	 * Effacement de tous les éléments tu tableau
	 */
	public void efface()
	{
		for (Iterator<E> it = iterator(); it.hasNext();)
		{
			it.next();
			it.remove();
		}
	}

	/**
	 * Insertion d'un élément en début de tableau
	 * @param element l'élément à insérer
	 */
	public void insertElement(E element)
	{
		try
		{
			insertElement(element, 0);
		}
		catch (IndexOutOfBoundsException ioobe)
		{
			System.err.println("Tableau::insertElement : " + ioobe);
		}
	}

	/**
	 * Insertion d'un élément à la place index
	 * @param element l'élément à insérer dans le tableau
	 * @param index l'index où insérer l'élement
	 * @throws IndexOutOfBoundsException si l'index où insérer l'élément est
	 * invalide
	 */
	public void insertElement(E element, int index)
			throws IndexOutOfBoundsException
	{
		if ((index <= size) && (index >= 0))
		{
			if (index == size)
			{
				ajouter(element);
			}
			else // index >=0 & < size
			{
				if ((size + 1) >= capacity)
				{
					ajouterCapacite(INCREMENT);
				}
				// décalage des elements
				for (int i = size; i > index; i--)
				{
					table[i] = table[i - 1];
				}

				table[index] = element;
				size++;
			}
		}
		else
		{
			throw new IndexOutOfBoundsException("Invalid Index :"
					+ Integer.toString(index));
		}

	}

	/**
	 * Factory method fournissant un itérateur sur le tableau
	 * @return un nouvel itérateur sur le tableau
	 */
	@Override
	public Iterator<E> iterator()
	{
		return new TabIterator<E>();
	}

	/**
	 * Test d'égalité avec un autre objet.
	 * @return true ssi l'objet est un {@link Tableau} et qu'il contient
	 * les mêmes éléménts dans le même ordre.
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj)
	{
		if (obj == null)
		{
			return false;
		}

		if (obj == this)
		{
			return true;
		}

		if (getClass().isInstance(obj))
		{
			Tableau<?> tab = (Tableau<?>) obj;

			Iterator<E> it1 = iterator();
			Iterator<?> it2 = tab.iterator();

			for (; it1.hasNext() && it2.hasNext();)
			{
				if (!it1.next().equals(it2.next()))
				{
					return false;
				}
			}

			return !it1.hasNext() && !it2.hasNext();
		}
		else
		{
			return false;
		}
	}

	/**
	 * Code de hashage d'un tableau.
	 * Le code he hashage est compatible avec celui fournit par toute {@link Collection}
	 * contenant les mêmes éléments dans le même ordre.
	 * @return le code de hashage résultants des éléments du Tableau
	 * @see java.lang.Object#hashCode()
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
	 * Chaine de caractère représentant les éléments du tableau ainsi que sa
	 * taille et sa capacité courante
	 * @return une nouvelle chaine de caractère représentant le Tableau
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append("[");
		for (Iterator<E> it = iterator(); it.hasNext();)
		{
			sb.append(it.next().toString());
			if (it.hasNext())
			{
				sb.append(", ");
			}
		}
		sb.append("](");
		sb.append(Integer.toString(size));
		sb.append(", ");
		sb.append(Integer.toString(capacity));
		sb.append(")");

		return new String(sb);
	}

	/**
	 * Itérateur sur un {@link Tableau}
	 * @author davidroussel
	 * @param <F> le type des éléments à itérer
	 */
	private class TabIterator<F> implements Iterator<F>
	{
		/**
		 * L'index courant de l'itérateur. index de l'élément courant dans le
		 * tableau
		 */
		private int index;

		/**
		 * Indique si next vient d'être appelé ce qui permet (éventuellement)
		 * d'appeler remove.
		 */
		private boolean nextCalled;

		/**
		 * Constructeur par défaut d'un itérateur sur un tableau
		 */
		public TabIterator()
		{
			index = 0;
			nextCalled = false;
		}

		/**
		 * Clause de continuation
		 * @return true si l'itérateur peut encore itérer (utiliser la méthode
		 * {@link #next()})
		 */
		@Override
		public boolean hasNext()
		{
			return index < size;
		}

		/**
		 * Incrémentation de l'itérateur
		 * @return la donnée correpondant à la position courante de l'itérateur
		 * @throws NoSuchElementException si l'itérateur ne peut plus itérer,
		 * lorsque celui ci a déjà atteint le dernier élément à itérer
		 */
		@Override
		public F next() throws NoSuchElementException
		{
			if (hasNext())
			{
				@SuppressWarnings("unchecked")
				F element = (F) table[index];
				index++;
				nextCalled = true;
				return element;
			}
			else
			{
				throw new NoSuchElementException();
			}
		}

		/**
		 * Suppression du dernier élément renvoyé par {@link #next()}.
		 * Attention, remove ne peut être appelé qu'après avoir appelé
		 * {@link #next()}.
		 * @post l'élément précédent l'élément courant de l'itérateur a été
		 * supprimé.
		 */
		@Override
		public void remove() throws IllegalStateException
		{
			if (nextCalled) // index >= 1
			{
				for (int i = index - 1; i < (size - 1); i++)
				{
					table[i] = table[i + 1];
				}
				size--;
				index--;
				nextCalled = false;
			}
			else
			{
				throw new IllegalStateException("Next not called yet");
			}
		}
	}
}
