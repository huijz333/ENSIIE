package tests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNotSame;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.NoSuchElementException;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import listes.Liste;

/**
 * Classe de test de la liste Chainée
 * @author davidroussel
 */
public class ListeTest
{

	/**
	 * La liste à tester.
	 * La nature du contenu de la liste importe peu du moment qu'il est
	 * homogène : donc n'importe quel type ferait l'affaire.
	 */
	private Liste<String> liste = null;

	/**
	 * Liste des éléments à insérer dans la liste
	 */
	private static String[] elements;

	/**
	 * Mise en place avant l'ensemble des tests
	 * @throws java.lang.Exception [not used]
	 */
	@BeforeClass
	public static void setUpBeforeClass() throws Exception
	{
		System.out.println("-------------------------------------------------");
		System.out.println("Test de la Liste");
		System.out.println("-------------------------------------------------");
	}

	/**
	 * Nettoyage après l'ensemble des tests
	 * @throws java.lang.Exception [not used]
	 */
	@AfterClass
	public static void tearDownAfterClass() throws Exception
	{
		System.out.println("-------------------------------------------------");
		System.out.println("Fin Test de la Liste");
		System.out.println("-------------------------------------------------");
	}

	/**
	 * Mise en place avant chaque test
	 * @throws java.lang.Exception [not used]
	 */
	@Before
	public void setUp() throws Exception
	{
		elements = new String[] {
					"Hello",
					"Brave",
					"New",
					"World"
				};
		liste = new Liste<String>();
	}

	/**
	 * Nettoyage après chaque test
	 * @throws java.lang.Exception [not used]
	 */
	@After
	public void tearDown() throws Exception
	{
		liste.clear();
		liste = null;
	}

	/**
	 * Méthode utilitaire de remplissage de la liste avec les éléments
	 * du tableau #elements
	 */
	private final void remplissage()
	{
		if (liste != null)
		{
			for (String elt : elements)
			{
				liste.add(elt);
			}
		}
	}

	/**
	 * Test method for {@link listes.Liste#Liste()}.
	 */
	@Test
	public final void testListe()
	{
		String testName = new String("Liste<String>()");
		System.out.println(testName);

		assertNotNull(testName + " instance non null failed", liste);
		assertTrue(testName + " liste vide failed", liste.empty());
	}

	/**
	 * Test method for {@link listes.Liste#Liste(listes.Liste)}.
	 */
	@Test
	public final void testListeListeOfT()
	{
		String testName = new String("Liste<String>(Liste<String>)");
		System.out.println(testName);

		Liste<String> liste2 = new Liste<String>();
		liste = new Liste<String>(liste2);

		assertNotNull(testName + " instance non null failed", liste);
		assertTrue(testName + " liste vide failed", liste.empty());

		remplissage();
		assertFalse(testName + " liste remplie failed", liste.empty());
		liste2 = new Liste<String>(liste);
		assertNotNull(testName + " copie liste remplie failed", liste2);
		assertEquals(testName + " contenus égaux failed", liste, liste2);
	}

	/**
	 * Test method for {@link listes.Liste#add(java.lang.Object)}.
	 */
	@Test
	public final void testAdd()
	{
		String testName = new String("Liste<String>.add(E)");
		System.out.println(testName);

		// Ajout dans une liste vide
		liste.add(elements[0]);
		assertFalse(testName + "liste non vide failed", liste.empty());
		Iterator<String> it = liste.iterator();
		String insertedElt = it.next();
		assertSame(testName + " contrôle ref element[0] failed", insertedElt, elements[0]);
		// Si assertSame réussit asserEquals n'est plus nécessaire

		// Ajout dans une liste non vide
		for (int i=1; i < elements.length; i++)
		{
			liste.add(elements[i]);
			/*
			 * Attention le précédent "it" a été invalidé par l'ajout
			 * Lors du dernier next le current de l'itérateur est passé à null
			 * puisqu'il n'y avait pas (encore) de suivant, donc retenter un
			 * next sur le même itérateur génèrera un NoSuchelementException.
			 * Il faut donc réobtenir un itérateur pour parcourir la liste
			 * après un ajout
			 */
			it = liste.iterator();
			for (int j = 0; j <= i; j++)
			{
				insertedElt = it.next();
			}
			assertSame(testName + " contrôle ref element[" + i + "] failed",
					insertedElt, elements[i]);
		}
	}

	/**
	 * Test method for {@link listes.Liste#add(java.lang.Object)}.
	 */
	@Test(expected = NullPointerException.class)
	public final void testAddNull()
	{
		String testName = new String("Liste<String>.add(null)");
		System.out.println(testName);

		liste.add(elements[0]);

		assertFalse(testName + " ajout 1 elt failed", liste.empty());

		// Ajout null dans une liste non vide (sinon on fait un insere(null))
		// Doit lever une NullPointerException
		liste.add(null);

		fail(testName + " ajout null sans exception");
	}

	/**
	 * Test method for {@link listes.Liste#insert(java.lang.Object)}.
	 */
	@Test
	public final void testInsert()
	{
		String testName = new String("Liste<String>.insert(E)");
		System.out.println(testName);

		// Insertion elt null
		try
		{
			liste.insert(null);

			fail(testName +" insertion elt null");
		}
		catch (NullPointerException e)
		{
			assertTrue(testName + " insertion elt null, liste vide failed",
					liste.empty());
		}

		// Insertion dans une liste vide
		int lastIndex = elements.length - 1;
		liste.insert(elements[lastIndex]);
		assertFalse(testName + "liste non vide failed", liste.empty());
		Iterator<String> it = liste.iterator();
		String insertedElt = it.next();
		assertSame(testName + " contrôle ref element[" + lastIndex + "]  failed",
				insertedElt, elements[lastIndex]);
		// Si assertSame réussit asserEquals n'est plus nécessaire

		// Ajout dans une liste non vide
		for (int i=1; i < elements.length; i++)
		{
			liste.insert(elements[lastIndex - i]);

			insertedElt = liste.iterator().next();
			assertSame(testName + " contrôle ref element[" + (lastIndex - i)
					+ "] failed", insertedElt, elements[lastIndex - i]);
		}
	}

	/**
	 * Test method for {@link listes.Liste#insert(java.lang.Object)}.
	 */
	@Test(expected = NullPointerException.class)
	public final void testInsertNull()
	{
		String testName = new String("Liste<String>.insert(null)");
		System.out.println(testName);

		// Insertion dans une liste vide
		// Doit soulever une NullPointerException
		liste.insert(null);

		fail(testName + " insertion null sans exception");
	}

	/**
	 * Test method for {@link listes.Liste#insert(java.lang.Object, int)}.
	 */
	@Test
	public final void testInsertInt()
	{
		String testName = new String("Liste<String>.insert(E, int)");
		System.out.println(testName);

		int[] nextIndex = new int[] {1, 0, 3, 2};
		int index = 0;

		// - insertion d'un élément null
		boolean result = liste.insert(null, 0);
		assertFalse(testName + " insertion elt null ds liste vide failed",
				result);
		assertTrue(testName + " insertion elt null ds liste vide, liste vide failed",
				liste.empty());

		// - insertion dans une liste vide avec un index invalide
		result = liste.insert(elements[nextIndex[index]], 1);
		assertFalse(testName + " insertion ds liste vide, index invalide failed",
				result);
		assertTrue(testName + " insertion ds liste vide, index invalide, " +
				"liste vide failed",liste.empty());

		// + insertion dans une liste vide avec un index valide
		result = liste.insert(elements[nextIndex[index]], 0);
		// liste = Brave ->
		assertTrue(testName + " insertion ds liste vide, index valide failed",
				result);
		assertFalse(testName + " insertion ds liste vide, index valide, " +
				"liste non vide failed",liste.empty());
		index++;

		// - insertion dans une liste non vide avec un index invalide
		result = liste.insert(elements[nextIndex[index]], 5);
		assertFalse(testName + " insertion ds liste non vide, index invalide failed",
				result);

		// + insertion en début de liste non vide avec un index valide
		result = liste.insert(elements[nextIndex[index]], 0);
		// liste = Hello -> Brave ->
		assertTrue(testName + " insertion début liste non vide, index valide failed",
				result);
		index++;

		// + insertion en fin de liste non vide avec un index valide
		result = liste.insert(elements[nextIndex[index]], 2);
		// liste = Hello -> Brave -> World
		assertTrue(testName + " insertion fin liste non vide, index valide failed",
				result);
		index++;

		// + insertion en milieu de liste non vide avec un index valide
		result = liste.insert(elements[nextIndex[index]], 2);
		// liste = Hello -> Brave -> New -> World
		assertTrue(testName + " insertion milieu liste non vide, index valide failed",
				result);
	}

	/**
	 * Test method for {@link listes.Liste#remove(java.lang.Object)}.
	 */
	@Test
	public final void testRemove()
	{
		String testName = new String("Liste<String>.remove(E)");
		System.out.println(testName);

		// suppression d'un élément non null d'une liste vide
		boolean result = liste.remove(elements[0]);
		assertTrue(testName + " elt liste vide failed", liste.empty());
		assertFalse(testName + " elt liste vide failed", result);

		// suppresion d'un élement null d'une liste vide
		result = liste.remove(null);
		assertTrue(testName + " null liste vide failed", liste.empty());
		assertFalse(testName + " null liste vide failed", result);

		remplissage();
		liste.add("Hello"); // "Hello" not same as elements[0]
		// liste = Hello -> Brave -> New -> World -> Hello

		// suppression d'un élément null d'une liste non vide
		result = liste.remove(null);
		assertFalse(testName + " null failed", result);

		// suppression d'un élément inexistant d'une liste non vide
		result = liste.remove("Coucou");
		assertFalse(testName + " Coucou failed", result);

		// suppression d'un élement existant en début de liste
		result = liste.remove("Hello");
		// liste = Brave -> New -> World -> Hello
		assertTrue(testName + " suppr Hello debut failed", result);
		String nextElt = liste.iterator().next();
		assertSame(testName + " suppr Hello debut failed", nextElt, elements[1]);

		// suppresion d'un élement existant en fin de liste
		result = liste.remove("Hello");
		// liste = Brave -> New -> World
		assertTrue(testName + " Hello fin failed", result);
		Iterator<String> it = liste.iterator();
		it.next(); // Brave
		it.next(); // New
		String lastElt = it.next(); // World
		assertSame(testName + " Hello fin failed", lastElt, elements[3]);

		// suppression d'un élement existant en milieu de liste
		result = liste.remove(elements[2]);
		// liste = Brave -> World
		assertTrue(testName + " New milieu failed", result);
		it = liste.iterator();
		String firstElt = it.next(); // Brave
		lastElt = it.next(); // World
		assertSame(testName + " first elt left failed", firstElt, elements[1]);
		assertSame(testName + " last elt left failed", lastElt, elements[3]);
	}

	/**
	 * Test method for {@link listes.Liste#removeAll(java.lang.Object)}.
	 */
	@Test
	public final void testRemoveAll()
	{
		String testName = new String("Liste<String>.removeAll(E)");
		System.out.println(testName);

		// suppression d'un élément non null d'une liste vide
		boolean result = liste.removeAll(elements[0]);
		assertTrue(testName + " supprTous elt liste vide failed", liste.empty());
		assertFalse(testName + " supprTous elt liste vide failed", result);

		// suppresion d'un élement null d'une liste vide
		result = liste.removeAll(null);
		assertTrue(testName + " supprTous elt null liste vide failed", liste.empty());
		assertFalse(testName + " supprTous elt null liste vide failed", result);

		elements[2] = new String("Hello");
		remplissage();
		liste.add("Hello"); // "Hello" not same as elements[0]
		// liste = Hello -> Brave -> Hello -> World -> Hello

		// suppression d'un élément null d'une liste non vide
		result = liste.removeAll(null);
		assertFalse(testName + " supprTous elt null liste failed", result);

		// suppression d'un element existant au début, au milieu et à la fin
		result = liste.removeAll("Hello");
		// liste = Brave -> World
		assertTrue(testName + " supprimeTous Hello", result);
		Iterator<String> it = liste.iterator();
		String firstElt = it.next();
		String lastElt = it.next();
		assertFalse(testName + " 2 elts left failed", it.hasNext());
		assertSame(testName + " first elt left failed", firstElt, elements[1]);
		assertSame(testName + " last elt left failed", lastElt, elements[3]);
	}

	/**
	 * Test method for {@link listes.Liste#size()}.
	 */
	@Test
	public final void testSize()
	{
		String testName = new String("Liste<String>.size()");
		System.out.println(testName);

		// taille d'une liste vide
		assertTrue(testName + " taille liste vide failed", liste.size() == 0);

		remplissage();
		assertFalse(testName + " remplissage failed", liste.empty());

		// taille d'une liste non vide
		assertTrue(testName + " taille liste pleine failed",
		           liste.size() == elements.length);
	}

//	/**
//	 * Test method for {@link listes.Liste#get(int)}.
//	 */
//	@Test
//	public final void testGet()
//	{
//		String testName = new String("Liste<String>.get(int)");
//		System.out.println(testName);
//
//		// get sur une liste vide
//		assertTrue(testName + " get liste vide failed", liste.get(0) == null);
//		assertTrue(testName + " get liste vide failed", liste.get(-1) == null);
//
//		remplissage();
//		assertFalse(testName + " remplissage failed", liste.empty());
//
//		// get dans une liste non vide
//		for (int i = -1; i <= liste.size(); i++)
//		{
//			if ((i >= 0) && (i < liste.size()))
//			{
//				assertNotNull(testName + " get(" + i + ") liste pleine failed",
//						liste.get(i));
//				assertTrue(testName + " get(" + i + ") liste pleine failed",
//						liste.get(i).equals(elements[i]));
//			}
//			else
//			{
//				assertTrue(testName + " get(" + i + ") liste pleine failed",
//						liste.get(i) == null);
//			}
//		}
//	}

	/**
	 * Test method for {@link listes.Liste#clear()}.
	 */
	@Test
	public final void testClear()
	{
		String testName = new String("Liste<String>.clear()");
		System.out.println(testName);

		// effacement d'une liste vide
		liste.clear();
		assertTrue(testName + " effacement liste vide failed", liste.empty());

		remplissage();
		assertFalse(testName + " remplissage failed", liste.empty());

		// effacement d'une liste non vide
		liste.clear();
		assertTrue(testName + " effacement failed", liste.empty());
	}

	/**
	 * Test method for {@link listes.Liste#empty()}.
	 */
	@Test
	public final void testEmpty()
	{
		String testName = new String("Liste<String>.empty()");
		System.out.println(testName);

		assertTrue(testName + " vide failed", liste.empty());

		remplissage();

		assertFalse(testName + " non vide failed", liste.empty());
	}

	/**
	 * Test method for {@link listes.Liste#equals(java.lang.Object)}.
	 */
	@SuppressWarnings("unlikely-arg-type")
	@Test
	public final void testEqualsObject()
	{
		String testName = new String("Liste<String>.equals(Object)");
		System.out.println(testName);

		remplissage();

		// Inegalite sur objet null
		boolean result  = liste.equals(null);
		assertFalse(testName + " null object failed", result);

		// Egalite sur soi-même
		result = liste.equals(liste);
		assertTrue(testName + " self failed", result);

		// Egalite sur liste copiée
		Liste<String> liste2 = new Liste<String>(liste);
		result = liste.equals(liste2);
		assertTrue(testName + " copy failed", result);

		// Inegalité sur listes de tailles différentes
		liste2.add("of Pain");
		result = liste.equals(liste2);
		assertFalse(testName + " copy + of Pain failed", result);

		// Inegalite sur liste à contenu dans une autre ordre
		liste2.clear();
		for (String elt : elements)
		{
			liste2.insert(elt);
		}
		result = liste.equals(liste2);
		assertFalse(testName + " reversed copy failed", result);

		// Egalite avec une collection standard de même contenu
		// SSI equals compare un Iterable plutôt qu'une Liste
		ArrayList<String> alist = new ArrayList<>();
		for (String elt : elements)
		{
			alist.add(elt);
		}
		assertTrue(testName + " equality with std Iterable failed",
				liste.equals(alist));
	}

	/**
	 * Test method for {@link listes.Liste#toString()}.
	 */
	@Test
	public final void testToString()
	{
		String testName = new String("Liste<String>.toString()");
		System.out.println(testName);

		remplissage();

		assertEquals(testName, "[Hello->Brave->New->World]", liste.toString());
	}

	/**
	 * Test method for {@link listes.Liste#iterator()}.
	 */
	@Test(expected = NoSuchElementException.class)
	public final void testIterator()
	{
		String testName = new String("Liste<String>.iterator()");
		System.out.println(testName);

		Iterator<String> it = liste.iterator();
		assertFalse(testName + " liste vide", it.hasNext());

		remplissage();

		it = liste.iterator();
		assertTrue(testName + " liste non vide", it.hasNext());

		int i = 0;
		while (it.hasNext())
		{
			String nextElt = it.next();
			assertNotNull(testName + "next elt not null", nextElt);
			assertSame(testName + "next elt", elements[i++], nextElt);
			it.remove(); // ne doit pas invalider l'itérateur
		}

		assertFalse(testName + " finished", it.hasNext());

		// Un appel supplémentaire à next sur un itérateur terminé
		// doit soulever une NoSuchElementException
		it.next();

		fail(testName + " next sur itérateur terminé");
	}

	/**
	 * Test method for {@link listes.Liste#hashCode()}.
	 */
	@Test
	public final void testHashCode()
	{
		String testName = new String("Liste<String>.hashCode()");
		System.out.println(testName);

		// hashcode d'une liste vide = 1
		int listeHash = liste.hashCode();
		assertEquals(testName + " liste vide failed", 1, listeHash, 0);

		remplissage();

		// hashcode de la liste standard
		listeHash = liste.hashCode();
		assertEquals(testName + " liste standard failed", 1161611233, listeHash);

		/*
		 * Contrat hashCode : Si a.equals(b) alors a.hashcode() == b.hashcode()
		 */
		Liste<String> liste2 = new Liste<String>(liste);
		assertNotSame(testName + " egalite liste distinctes failed", liste, liste2);
		assertEquals(testName + " egalite liste equals failed", liste, liste2);
		assertEquals(testName + " egalite liste hashCode failed", liste.hashCode(),
				liste2.hashCode(), 0);

		liste2.add("Hourra");
		assertFalse(testName + " inegalite liste equals failed", liste.equals(liste2));
		assertFalse(testName + " inegalite liste hashCode failed",
				liste.hashCode() == liste2.hashCode());

		// HashCode similaire à celui d'une collection standard
		ArrayList<String> collection = new ArrayList<String>();
		for (String elt : elements)
		{
			collection.add(elt);
		}
		int collectionHash = collection.hashCode();
		assertEquals(testName + " hashCode standard failed", listeHash, collectionHash);
	}
}
