package tests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import tableaux.Tableau;

/**
 * Classe de teste de la classe {@link tableaux.Iterable}
 * @author davidroussel
 */
public class TableauTest
{

	/**
	 * Le tableau à tester
	 */
	private Tableau<String> tableau;

	/**
	 * Des éléments pour remplir le tableau.
	 * Le nombre d'éléments doit être supérieur à {@link Iterable#INCREMENT}
	 */
	private final static String[] elementsArray = new String[] {
		"Hello",
		"Brave",
		"New",
		"World",
		"of",
		"Pain"
	};

	/**
	 * Une collection standard pour comparer avec le tableau
	 */
	private ArrayList<String> elementsCollection;

	/**
	 * Mise en place avant l'ensemble des tests
	 * @throws java.lang.Exception
	 */
	@BeforeClass
	public static void setUpBeforeClass() throws Exception
	{
		System.out.println("-------------------------------------------------");
		System.out.println("Test du Tableau");
		System.out.println("-------------------------------------------------");
	}

	/**
	 * Netoyage après l'ensmble des tests
	 * @throws java.lang.Exception
	 */
	@AfterClass
	public static void tearDownAfterClass() throws Exception
	{
		System.out.println("-------------------------------------------------");
		System.out.println("Fin Test du Tableau");
		System.out.println("-------------------------------------------------");
	}

	/**
	 * Mise en place avant chaque test
	 * @throws java.lang.Exception
	 */
	@Before
	public void setUp() throws Exception
	{
		tableau = new Tableau<String>();
		elementsCollection = new ArrayList<String>();
		for (String elt : elementsArray)
		{
			elementsCollection.add(elt);
		}
	}

	/**
	 * Nettoyage après chaque test
	 * @throws java.lang.Exception
	 */
	@After
	public void tearDown() throws Exception
	{
		tableau.efface();
		tableau = null;
		elementsCollection.clear();
	}

	/**
	 * Comparaison des élements de deux Iterables
	 * @param testName le nom du test dans lequel est appelée cette méthode
	 * @param i1 le premier iterable à tester
	 * @param i2 le second iterable avec lequel comparer
	 * @return true si les deux iterables possèdent le même nombre
	 * d'éléments et que tous les éléments sont identiques et dans le même ordre
	 */
	private boolean compareElements(String testName,
			Iterable<String> i1,
			Iterable<String> i2)
	{
		Iterator<String> it1 = i1.iterator();
		Iterator<String> it2 = i2.iterator();

		for (; it1.hasNext() && it2.hasNext();)
		{
			String s1 = it1.next();
			String s2 = it2.next();

			assertEquals(testName + "compare " + s1 + " with " + s2, s1, s2);

			if (!s1.equals(s2))
			{
				return false;
			}
		}

		return !it1.hasNext() && !it2.hasNext();

	}

	/**
	 * Liste mélangée d'index compris entre 0 et nbElements - 1;
	 *
	 * @param nbElements le nombre d'indexs
	 * @return un tableau contenant nbElements élements compris entre
	 *         [0..nbElements-1] et mélangés dans un ordre aléaloire
	 */
	private int[] shuffledIndexs(int nbElements)
	{
		int[] shuffled = new int[nbElements];

		ArrayList<Integer> list = new ArrayList<Integer>();
		for (int i = 0; i < nbElements; i++)
		{
			list.add(Integer.valueOf(i));
		}

		Collections.shuffle(list);

		Iterator<Integer> il = list.iterator();
		for (int i = 0; (i < nbElements) && il.hasNext(); i++)
		{
			shuffled[i] = il.next().intValue();
		}

		return shuffled;

	}

	/**
	 * Test method for {@link tableaux.Iterable#Iterable()}.
	 */
	@Test
	public final void testTableau()
	{
		String testName = new String("Tableau()");
		System.out.println(testName);

		assertNotNull(testName + " instance", tableau);
		assertEquals(testName + " tableau vide", tableau.taille(), 0);
	}

	/**
	 * Test method for {@link tableaux.Iterable#Iterable(java.lang.Iterable)}.
	 */
	@Test
	public final void testTableauIterableOfE()
	{
		String testName = new String("Tableau(Iterable<E>)");
		System.out.println(testName);

		tableau = new Tableau<String>(elementsCollection);

		assertNotNull(testName + " instance", tableau);
		assertEquals(testName + " tableau non vide", tableau.taille(),
				elementsCollection.size());

		boolean compare = compareElements(testName, tableau, elementsCollection);

		assertTrue(testName + " elements comparison result", compare);
	}

	/**
	 * Test method for {@link tableaux.Iterable#taille()}.
	 */
	@Test
	public final void testTaille()
	{
		String testName = new String("Tableau.taille()");
		System.out.println(testName);

		assertEquals(testName + " tableau vide", tableau.taille(), 0);
		int taille = 0;
		for (String elt : elementsArray)
		{
			tableau.ajouter(elt);
			taille++;
			assertEquals(testName + " tableau[" + taille + "]",
					tableau.taille(), taille);
		}

		tableau.efface();
		assertEquals(testName + " tableau nettoyé", tableau.taille(), 0);
	}

	/**
	 * Test method for {@link tableaux.Iterable#capacite()}.
	 */
	@Test
	public final void testCapacite()
	{
		String testName = new String("Tableau.capacite()");
		System.out.println(testName);
		int predictedCapacity = 0;

		assertEquals(testName + "capacite tableau vide", tableau.capacite(),
				predictedCapacity);

		int nb = 0;
		for (String elt : elementsArray)
		{
			nb++;
			if (nb > tableau.capacite())
			{
				predictedCapacity += Tableau.INCREMENT;
			}
			tableau.ajouter(elt);
			assertEquals(testName + " tableau[" + nb + "]",
					tableau.capacite(), predictedCapacity);
		}
	}

	/**
	 * Test method for {@link tableaux.Iterable#ajouter(java.lang.Object)}.
	 */
	@Test
	public final void testAjouter()
	{
		String testName = new String("Tableau.ajouter(E)");
		System.out.println(testName);
		int predictedSize = 0;

		for (String elt : elementsArray)
		{
			tableau.ajouter(elt);

			predictedSize++;

			String lastElement = null;
			for (Iterator<String> itt = tableau.iterator(); itt.hasNext();)
			{
				lastElement = itt.next();
			}

			assertEquals(testName + " size", predictedSize, tableau.taille());
			assertEquals(testName + "last elt comparison", elt, lastElement);
		}
	}

	/**
	 * Test method for {@link tableaux.Iterable#retrait(java.lang.Object)}.
	 */
	@Test
	public final void testRetrait()
	{
		String testName = new String("Tableau.retrait(E)");
		System.out.println(testName);

		tableau = new Tableau<String>(elementsCollection);
		int nbElements = elementsArray.length;
		int nbElementsLeft = nbElements;

		boolean result = compareElements(testName, tableau, elementsCollection);
		assertTrue(testName + " no more elts to compare", result);
		// on va retirer des elts de tableau et elementsCollection dans un
		// ordre aléatoire
		int[] indexs = shuffledIndexs(nbElements);

		for (int i = 0; i < nbElements; i++)
		{
			tableau.retrait(elementsArray[indexs[i]]);
			elementsCollection.remove(elementsArray[indexs[i]]);
			nbElementsLeft = elementsCollection.size();

			result = compareElements(testName, tableau, elementsCollection);
			assertTrue(testName + nbElementsLeft + "elts compared", result);
		}
	}

	/**
	 * Test method for {@link tableaux.Iterable#efface()}.
	 */
	@Test
	public final void testEfface()
	{
		String testName = new String("Tableau.efface()");
		System.out.println(testName);

		tableau = new Tableau<String>(elementsCollection);

		assertTrue(testName + "tableau initial non vide", tableau.taille() > 0);

		tableau.efface();

		assertEquals(testName + "tableau final vide", tableau.taille(), 0);
		Iterator<String> it = tableau.iterator();
		assertFalse(testName + " pas d'elts à itérer", it.hasNext());
	}

	/**
	 * Test method for {@link tableaux.Iterable#insertElement(java.lang.Object)}.
	 */
	@Test
	public final void testInsertElementE()
	{
		String testName = new String("Tableau.insertElement(E)");
		System.out.println(testName);

		for (String elt : elementsArray)
		{
			tableau.insertElement(elt);

			Iterator<String> it = tableau.iterator();
			assertEquals(testName + " first elt compare", elt, it.next());
		}
	}

	/**
	 * Test method for {@link tableaux.Iterable#insertElement(java.lang.Object, int)}.
	 * Ajout à un index invalide dans une collection vide
	 */
	@Test(expected = IndexOutOfBoundsException.class)
	public final void testInsertElementEIntInvalidEmpty()
	{
		String testName = new String("Tableau.insertElement(E, int)");
		System.out.println(testName);

		tableau.insertElement("Bonjour", 1);

		fail(testName + " Ajout ds tableau vide à index invalide réussi !");
	}

	/**
	 * Test method for {@link tableaux.Iterable#insertElement(java.lang.Object, int)}.
	 * Ajout à un index invalide dans une collection pleine
	 */
	@Test(expected = IndexOutOfBoundsException.class)
	public final void testInsertElementEIntInvalidFull()
	{
		String testName = new String("Tableau.insertElement(E, int)");
		System.out.println(testName);

		tableau = new Tableau<String>(elementsCollection);

		tableau.insertElement("Bonjour", tableau.taille() + 1);

		fail(testName + " Ajout ds tableau plein à index invalide réussi !");
	}

	/**
	 * Test method for {@link tableaux.Iterable#insertElement(java.lang.Object, int)}.
	 */
	@Test
	public final void testInsertElementEInt()
	{
		String testName = new String("Tableau.insertElement(E, int)");
		System.out.println(testName);
		int nbElements = elementsArray.length;
		elementsCollection.clear();
		int currentSize = 0;
		boolean result = false;

		// Ajouts en début et fin
		for (int i = 0; i < (nbElements / 2); i++)
		{
			// Ajout au début
			tableau.insertElement(elementsArray[i], 0);
			elementsCollection.add(0, elementsArray[i]);

			currentSize = elementsCollection.size();

			result = compareElements(testName, tableau, elementsCollection);
			assertTrue(testName + " after push front", result);

			// Ajout à la fin
			int sourceIdx = nbElements-1-i;
			tableau.insertElement(elementsArray[sourceIdx], currentSize);
			elementsCollection.add(currentSize, elementsArray[sourceIdx]);

			result = compareElements(testName, tableau, elementsCollection);
			assertTrue(testName + " after push back", result);
		}

		currentSize = elementsCollection.size();

		// Ajout au milieu
		String extraElement = "Bonjour";
		tableau.insertElement(extraElement, currentSize/2);
		elementsCollection.add(currentSize/2, extraElement);

		result = compareElements(testName, tableau, elementsCollection);
		assertTrue(testName + " after push middle", result);
	}

	/**
	 * Test method for {@link tableaux.Iterable#iterator()}.
	 */
	@Test
	public final void testIterator()
	{
		String testName = new String("Tableau.iterator()");
		System.out.println(testName);

		// itérateur sur tableau vide
		Iterator<String> itt = tableau.iterator();
		assertFalse(testName + " iterateur sur tableau vide", itt.hasNext());

		// itérateur su tableau rempli
		tableau = new Tableau<String>(elementsCollection);
		boolean result = compareElements(testName, tableau, elementsCollection);
		assertTrue(testName, result);

		// utilisation du remove sans next
		for (itt = tableau.iterator(); itt.hasNext(); )
		{
			try
			{
				itt.remove();
				fail(testName + " remove utilisé avec succès sans next dans boucle");
			}
			catch (IllegalStateException ise)
			{
				// rien, c'est normal
			}
			itt.next();
			itt.remove();
		}

		assertFalse(testName + " iterator terminé fin boucle", itt.hasNext());
		assertEquals(testName + " tableau vide avec suite remove", 0,
				tableau.taille());
	}

	/**
	 * Test method for {@link tableaux.Iterable#equals(java.lang.Object)}.
	 */
	@SuppressWarnings("unlikely-arg-type")
	@Test
	public final void testEqualsObject()
	{
		String testName = new String("Tableau.equals(Object)");
		System.out.println(testName);

		// Inegalite avec null
		boolean result = tableau.equals(null);
		assertFalse(testName + " inequality with null", result);

		// Egalite avec this
		assertTrue(testName + " self equality", tableau.equals(tableau));

		// Egalite avec une copie de soi même (vide)
		Tableau<String> other = new Tableau<String>(tableau);
		assertTrue(testName + " equality with copy", tableau.equals(other));

		// Inegalite avec tableau de contenu différent
		for (String elt : elementsArray)
		{
			tableau.ajouter(elt);
		}
		assertFalse(testName + " content inequality", tableau.equals(other));

		// Egalite sur contenus identiques
		for (String elt : elementsArray)
		{
			other.ajouter(elt);
		}
		assertTrue(testName + " content equality", tableau.equals(other));

		// Inegalite avec un objet quelconque
		assertFalse(testName + " type inequality", tableau.equals(new Object()));

		// Inegalite avec un autre Iterable
		assertFalse(testName + " inequality with Iterable",
				tableau.equals(elementsCollection));
	}

	/**
	 * Test method for {@link tableaux.Iterable#hashCode()}.
	 */
	@Test
	public final void testHashCode()
	{
		String testName = new String("Tableau.hashCode()");
		System.out.println(testName);

		// Hash code sur tableau vide
		assertEquals(testName + " empty tableau", 1, tableau.hashCode());

		tableau = new Tableau<String>(elementsCollection);

		// Hash code sur tableau rempli égal au hascode des collections standard
		assertEquals(testName + " full tableau", tableau.hashCode(),
				elementsCollection.hashCode());
	}

	/**
	 * Test method for {@link tableaux.Iterable#toString()}.
	 */
	@Test
	public final void testToString()
	{
		String testName = new String("Tableau.toString()");
		System.out.println(testName);

		tableau = new Tableau<String>(elementsCollection);

		StringBuilder sb = new StringBuilder();
		sb.append("[");
		for (Iterator<String> it = tableau.iterator(); it.hasNext();)
		{
			sb.append(it.next().toString());
			if (it.hasNext())
			{
				sb.append(", ");
			}
		}
		sb.append("](");
		sb.append(Integer.toString(tableau.taille()));
		sb.append(", ");
		sb.append(Integer.toString(tableau.capacite()));
		sb.append(")");
		String expected = sb.toString();

		assertEquals(testName, expected, tableau.toString());

	}
}
