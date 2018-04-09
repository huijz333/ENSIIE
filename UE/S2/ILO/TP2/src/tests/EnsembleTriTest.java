package tests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

import ensembles.EnsembleTri;
import ensembles.EnsembleTriFactory;
import ensembles.EnsembleTriTableau;

/**
 * Classe de test complémentaire pour tous les types d'ensembles triés :
 * {@link ensembles.EnsembleTriVector}, {@link ensembles.EnsembleTriVector2},
 * {@link ensembles.EnsembleTriListe}, {@link ensembles.EnsembleTriListe2},
 * {@link ensembles.EnsembleTriTableau}, {@link ensembles.EnsembleTriTableau2}
 * @author davidroussel
 */
@RunWith(value = Parameterized.class)
public class EnsembleTriTest
{
	/**
	 * l'ensemble à tester
	 */
	private EnsembleTri<String> ensemble;

	/**
	 * Le type d'ensemble à tester.
	 */
	private Class<? extends EnsembleTri<String>> typeEnsemble;

	/**
	 * Nom du type d'ensemble à tester
	 */
	private String typeName;

	/**
	 * Les différentes natures d'ensembles à tester
	 */
	@SuppressWarnings("unchecked")
	private static final Class<? extends EnsembleTri<String>>[] typesEnsemble =
	(Class<? extends EnsembleTri<String>>[]) new Class<?>[]
	{
		/*
		 * TODO Commenter / décommenter les lignes ci-dessous en focntion
		 * de votre avancement (Attention la dernière ligne non commentée
		 * ne doit pas avoir de virgule)
		 */
//		EnsembleTriVector.class,
//		EnsembleTriVector2.class,
		EnsembleTriTableau.class
//		EnsembleTriTableau2.class,
//		EnsembleTriListe.class,
//		EnsembleTriListe2.class
	};

	/**
	 * Elements pour remplir l'ensemble
	 */
	private static final String[] elements = new String[] {
		"Lorem",		// 0
		"ipsum",		// 6
		"sit",			// 7
		"dolor",		// 4
		"amet",			// 2
		"dolor",		// 4
		"amet",			// 2
		"consectetur",	// 3
		"adipisicing",	// 1
		"elit"			// 5
	};

	/**
	 * Rang d'insertion des éléments successifs
	 */
	private static final int[] insertionRank = new int[] {
		0,	// Lorem
		1, 	// ipsum
		2, 	// sit
		1, 	// dolor
		1, 	// amet
		2,	// dolor
		1,	// amet
		2,	// consectetur
		1,	// adipisicing
		5,	// elit
	};
	/**
	 * Elements triés pour contrôler le remplissage de l'ensemble
	 */
	private static final String[] singleSortedElements = new String[] {
		"Lorem",		// 0
		"adipisicing",	// 1
		"amet",			// 2
		"consectetur",	// 3
		"dolor",		// 4
		"elit",			// 5
		"ipsum",		// 6
		"sit"			// 7
	};

	/**
	 * Elements triés pour contrôler le remplissage de l'ensemble
	 */
	private static final String[][] insertSortedElements = new String[][] {
		{"Lorem"},
		{"Lorem", "ipsum"},
		{"Lorem", "ipsum", "sit"},
		{"Lorem", "dolor", "ipsum", "sit"},
		{"Lorem", "amet", "dolor", "ipsum", "sit"},
		{"Lorem", "amet", "dolor", "ipsum", "sit"},
		{"Lorem", "amet", "dolor", "ipsum", "sit"},
		{"Lorem", "amet", "consectetur", "dolor", "ipsum", "sit"},
		{"Lorem", "adipisicing", "amet", "consectetur", "dolor", "ipsum", "sit"},
		singleSortedElements
	};

	/**
	 * Collection pour contenir les élements de remplissage
	 */
	private ArrayList<String> listElements;

	/**
	 * Construit une instance de EnsembleTri<String> en fonction d'un type
	 * d'ensemble à créer et éventuellement d'un contenu l'ensemble à mettre en
	 * place
	 * @param testName le message à répéter dans les assertions en fonction du
	 * test dans lequel est employé cette méthode
	 * @param type le type d'ensemble à créer
	 * @param content le contenu à mettre en place dans le nouvel ensemble, ou
	 * bien null si aucun contenu n'est requis.
	 * @return un nouvel ensemble du type demandé evt rempli avec le contenu
	 * fournit s'il est non null.
	 */
	private static EnsembleTri<String>
	constructEnsemble(String testName,
	                  Class<? extends EnsembleTri<String>> type,
	                  Iterable<String> content)
	{
		EnsembleTri<String> ensemble = null;

		try
		{
			ensemble = EnsembleTriFactory.<String>getEnsemble(type, content);
		}
		catch (SecurityException e)
		{
			fail(testName +  " constructor security exception");
		}
		catch (NoSuchMethodException e)
		{
			fail(testName +  " constructor not found");
		}
		catch (IllegalArgumentException e)
		{
			fail(testName + " wrong constructor arguments");
		}
		catch (InstantiationException e)
		{
			fail(testName + " instanciation exception");
		}
		catch (IllegalAccessException e)
		{
			fail(testName + " illegal access");
		}
		catch (InvocationTargetException e)
		{
			fail(testName + " invocation exception");
		}

		return ensemble;
	}

	/**
	 * Compare les éléments d'un ensemble pour vérifier qu'ils sont tous dans
	 * un tableau donné et dans le même ordre
	 * @param testName le nom du test dans lequel est utilisé cette méthode
	 * @param ensemble l'ensemble dont on doit comparer les éléments
	 * @param array le tableau utilisé pour vérifier la présence des éléments
	 * de l'ensemble
	 * @return true si tous les éléments du tableau sont présents dans l'ensemble
	 * et dans le même ordre
	 */
	private static boolean compareElts2Array(String testName,
			EnsembleTri<String> ensemble, String[] array)
	{
		Iterator<String> ite = ensemble.iterator();

		if (ite != null)
		{
			for (int i = 0; (i < array.length) && ite.hasNext(); i++)
			{
				String ensembleElt = ite.next();
				String arrayElt = array[i];
				boolean check = ensembleElt.equals(arrayElt);
				assertTrue(testName + " [" + i + "] = " + arrayElt + " == "
						+ ensembleElt + " failed", check);
				if (!check)
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * Vérifie qu'un ensemble ne contient qu'un seul exemplaire de chacun
	 * de ses éléments
	 * @param testName le nom du test dans lequel est employée cette méthode
	 * @param ensemble l'ensemble à tester
	 * @return true si chaque élément de l'ensemble n'existe qu'à un seul
	 * exemplaire.
	 */
	private static <E extends Comparable<E>>
	boolean checkCount(String testName, EnsembleTri<E> ensemble)
	{
		Map<E, Integer> wordCount = new HashMap<E, Integer>();
		for (E elt : ensemble)
		{
			if (!wordCount.containsKey(elt))
			{
				wordCount.put(elt, Integer.valueOf(1));
			}
			else
			{
				Integer count = wordCount.get(elt);
				count = Integer.valueOf(count.intValue() + 1);
				wordCount.put(elt, count);
			}
		}

		for (Integer i : wordCount.values())
		{
			int countValue = i.intValue();
			assertEquals(testName + " count check #" + countValue + " failed",
					1, countValue);
			if (countValue != 1)
			{
				return false;
			}
		}

		return true;
	}

	/**
	 * Paramètres à transmettre au constructeur de la classe de test.
	 * @return une collection de tableaux d'objet contenant les paramètres à
	 * transmettre au constructeur de la classe de test
	 */
	@Parameters(name = "{index}:{1}")
	public static Collection<Object[]> data()
	{
		Object[][] data = new Object[typesEnsemble.length][2];
		for (int i = 0; i < typesEnsemble.length; i++)
		{
			data[i][0] = typesEnsemble[i];
			data[i][1] = typesEnsemble[i].getSimpleName();
		}
		return Arrays.asList(data);
	}

	/**
	 * Constructeur paramêtré par le type d'ensemble à tester.
	 * Lancé pour chaque test
	 * @param typeEnsemble le type d'ensemble à générer
	 * @param le nom du type d'ensemble à tester (pour le faire apparaître
	 * dans le déroulement des tests).
	 */
	public EnsembleTriTest(Class<? extends EnsembleTri<String>> typeEnsemble,
	                       String typeEnsembleName)
	{
		this.typeEnsemble = typeEnsemble;
		typeName = typeEnsembleName;
	}

	/**
	 * Mise en place avant l'ensemble des tests
	 * @throws java.lang.Exception
	 */
	@BeforeClass
	public static void setUpBeforeClass() throws Exception
	{
		System.out.println("-------------------------------------------------");
		System.out.println("Test des ensembles triés");
		System.out.println("-------------------------------------------------");
	}

	/**
	 * Nettoyage après l'ensemble des tests
	 * @throws java.lang.Exception
	 */
	@AfterClass
	public static void tearDownAfterClass() throws Exception
	{
		System.out.println("-------------------------------------------------");
		System.out.println("Fin Test des ensembles triés");
		System.out.println("-------------------------------------------------");
	}

	/**
	 * Mise en place avant chaque test
	 * @throws java.lang.Exception
	 */
	@Before
	public void setUp() throws Exception
	{
		ensemble = constructEnsemble("setUp", typeEnsemble, null);
		assertNotNull("setUp non null instance failed", ensemble);

		listElements = new ArrayList<String>();

		for (String elt : elements)
		{
			listElements.add(elt);
		}
	}

	/**
	 * Nettoyage après chaque test
	 * @throws java.lang.Exception
	 */
	@After
	public void tearDown() throws Exception
	{
		ensemble.efface();
		ensemble = null;
		listElements.clear();
		listElements = null;
	}

	/**
	 * Test method for
	 * {@link ensembles.EnsembleTriVector#EnsembleTriVector()} or
	 * {@link ensembles.EnsembleTriVector2#EnsembleTriVector2()} or
	 * {@link ensembles.EnsembleTriListe#EnsembleTriListe()} or
	 * {@link ensembles.EnsembleTriListe2#EnsembleTriListe2()} or
	 * {@link ensembles.EnsembleTriTableau#EnsembleTriTableau()} or
	 * {@link ensembles.EnsembleTriTableau2#EnsembleTriTableau2()}
	 */
	@Test
	public final void testDefaultConstructor()
	{
		String testName = new String(typeName + "()");
		System.out.println(testName);

		ensemble = constructEnsemble(testName, typeEnsemble, null);
		assertNotNull(testName + " non null instance failed" , ensemble);

		assertEquals(testName + " instance type failed", typeEnsemble,
				ensemble.getClass());
		assertTrue(testName +  " empty instance failed", ensemble.estVide());
		assertEquals(testName + "instance size failed", 0, ensemble.cardinal());
	}

	/**
	 * Test method for
	 * {@link ensembles.EnsembleTriVector#EnsembleTriVector(Iterable)} or
	 * {@link ensembles.EnsembleTriVector2#EnsembleTriVector2(Iterable)} or
	 * {@link ensembles.EnsembleTriListe#EnsembleTriListe(Iterable)} or
	 * {@link ensembles.EnsembleTriListe2#EnsembleTriListe2(Iterable)} or
	 * {@link ensembles.EnsembleTriTableau#EnsembleTriTableau(Iterable)} or
	 * {@link ensembles.EnsembleTriTableau2#EnsembleTriTableau2(Iterable)} or
	 */
	@Test
	public final void testCopyConstructor()
	{
		String testName = new String(typeName + "(Iterable)");
		System.out.println(testName);

		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed" , ensemble);

		assertEquals(testName + " instance type failed", typeEnsemble,
				ensemble.getClass());
		assertFalse(testName + " not empty instance failed", ensemble.estVide());
		boolean compare = compareElts2Array(testName, ensemble,
				singleSortedElements);
		assertTrue(testName + " elts compare failed", compare);

		// Tous les éléments de ensemble doivent se retrouver dans list
		for (String elt : ensemble)
		{
			assertTrue(testName + "check content [" + elt + "] failed",
					listElements.contains(elt));
		}

		// Tous les éléments de l'ensemble n'existent qu'à un seul exemplaire
		boolean countCheck = EnsembleTriTest.<String>checkCount(testName,
				ensemble);
		assertTrue(testName + "after count check failed", countCheck);

	}

	/**
	 * Test method for {@link ensembles.EnsembleTri#ajout(java.lang.Comparable)}.
	 */
	@Test
	public final void testAjout()
	{
		String testName = new String(typeName + ".ajout(E)");
		System.out.println(testName);

		assertTrue(testName + " vide avant remplissage failed",
				ensemble.estVide());

		int size = 0;
		for (int i = 0; i < elements.length; i++)
		{
			if (!ensemble.contient(elements[i]))
			{
				size++;
			}
			ensemble.ajout(elements[i]);
			assertEquals(testName + " size failed", size, ensemble.cardinal());
			boolean checkElts = compareElts2Array(testName, ensemble,
					insertSortedElements[i]);
			assertTrue(testName + " check elts failed", checkElts);
		}
	}

	/**
	 * Test method for {@link ensembles.EnsembleTri#rang(java.lang.Comparable)}.
	 */
	@Test
	public final void testRang()
	{
		String testName = new String(typeName + ".rang(E)");
		System.out.println(testName);

		assertTrue(testName + " vide avant remplissage failed",
				ensemble.estVide());

		for (int i = 0; i < elements.length; i++)
		{
			assertEquals(testName + " rang de " + elements[i] + "[" + i
					+ "] failed", insertionRank[i], ensemble.rang(elements[i]));
			ensemble.ajout(elements[i]);
		}
	}
}
