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
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

import ensembles.Ensemble;
import ensembles.EnsembleFactory;
import ensembles.EnsembleListe;
import ensembles.EnsembleTableau;
import ensembles.EnsembleTri;
import ensembles.EnsembleTriListe;
import ensembles.EnsembleTriListe2;
import ensembles.EnsembleTriTableau;
import ensembles.EnsembleTriTableau2;
import ensembles.EnsembleTriVector;
import ensembles.EnsembleTriVector2;
import ensembles.EnsembleVector;

/**
 * Classe de test pour tous les types d'ensembles :
 * {@link ensembles.EnsembleVector}, {@link ensembles.EnsembleListe},
 * {@link ensembles.EnsembleTableau}.
 * Mais aussi pour les méthodes communes avec les ensemble triés tels que
 * {@link ensembles.EnsembleTriVector}, {@link ensembles.EnsembleTriVector2},
 * {@link ensembles.EnsembleTriListe}, {@link ensembles.EnsembleTriListe2},
 * {@link ensembles.EnsembleTriTableau}, {@link ensembles.EnsembleTriTableau2}
 * @author davidroussel
 */
@RunWith(value = Parameterized.class)
public class AllEnsembleTest
{
	/**
	 * l'ensemble à tester
	 */
	private Ensemble<String> ensemble;

	/**
	 * Le type d'ensemble à tester.
	 */
	private Class<? extends Ensemble<String>> typeEnsemble;

	/**
	 * Nom du type d'ensemble à tester
	 */
	private String typeName;

	/**
	 * Les différentes natures d'ensembles à tester
	 */
	@SuppressWarnings("unchecked")
	private static final Class<? extends Ensemble<String>>[] typesEnsemble =
	(Class<? extends Ensemble<String>>[]) new Class<?>[]
	{
		/*
		 * TODO Commenter / décommenter les lignes ci-dessous en fonction
		 * de votre avancement (Attention la dernière ligne non commentée
		 * ne doit pas avoir de virgule)
		 */
		EnsembleTableau.class,
		EnsembleVector.class,
		EnsembleListe.class,
		EnsembleTriVector.class,
		EnsembleTriVector2.class,
		EnsembleTriTableau.class,
		EnsembleTriTableau2.class,
		EnsembleTriListe.class,
		EnsembleTriListe2.class
	};

	/**
	 * Elements pour remplir l'ensemble : "Lorem ipsum dolor sit amet"
	 */
	private static final String[] elements1 = new String[] {
		"Lorem",
		"ipsum",
		"sit",
		"dolor",
		"amet"
	};

	/**
	 * Autres Elements pour remplir un ensemble :
	 * "dolor amet consectetur adipisicing elit"
	 */
	private static final String[] elements2 = new String[] {
		"dolor",
		"amet",
		"consectetur",
		"adipisicing",
		"elit"
	};

	/**
	 * Elements union de {@value #elements1} et {@link #elements2}
	 */
	private static final String[] allSingleElements = new String[] {
		"Lorem",
		"ipsum",
		"sit",
		"dolor",
		"amet",
		"consectetur",
		"adipisicing",
		"elit"
	};

	/**
	 * Elements union triée de {@value #elements1} et
	 * {@link #elements2}
	 */
	private static final String[] allSingleElementsSorted = new String[] {
		"Lorem",
		"adipisicing",
		"amet",
		"consectetur",
		"dolor",
		"elit",
		"ipsum",
		"sit"
	};

	/**
	 * Elements communs à {@value #elements1} et {@link #elements2}
	 */
	private static final String[] commonSingleElements = new String[] {
		"dolor",
		"amet"
	};

	/**
	 * Elements du complement de {@value #elements1} et
	 * {@link #elements2}
	 */
	private static final String[] complementElements1 = new String[] {
		"Lorem",
		"ipsum",
		"sit"
	};

	/**
	 * Elements du complement de {@value #elements2} et
	 * {@link #elements1}
	 */
	private static final String[] complementElements2 = new String[] {
		"consectetur",
		"adipisicing",
		"elit"
	};

	/**
	 * Elements non communs à {@value #elements1} et
	 * {@link #elements2}
	 */
	private static final String[] diffSingleElements = new String[] {
		"Lorem",
		"ipsum",
		"sit",
		"consectetur",
		"adipisicing",
		"elit"
	};

	/**
	 * Elements pour remplir l'ensemble avec des doublons pour vérifier que ceux
	 * ci ne seront pas ajoutés dans les ensembles
	 */
	private static final String[] elements = new String[elements1.length
			+ elements2.length];

	/**
	 * Collection pour contenir les élements de remplissage
	 */
	private ArrayList<String> listElements;

	/**
	 * Construit une instance de Ensemble<String> en fonction d'un type
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
	private static Ensemble<String>
	constructEnsemble(String testName,
	                  Class<? extends Ensemble<String>> type,
	                  Iterable<String> content)
	{
		Ensemble<String> ensemble = null;

		try
		{
			ensemble = EnsembleFactory.<String>getEnsemble(type, content);
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
	 * un tableau donné
	 * @param testName le nom du test dans lequel est utilisé cette méthode
	 * @param ensemble l'ensemble dont on doit comparer les éléments
	 * @param array le tableau utilisé pour vérifier la présence des éléments
	 * de l'ensemble
	 * @return true si tous les éléments du tableau sont présents dans l'ensemble
	 */
	private static boolean compareElts2Array(String testName,
			Ensemble<String> ensemble, String[] array)
	{
		for (String elt : array)
		{
			boolean contenu = ensemble.contient(elt);
			assertTrue(testName + " contient(" + elt + ") failed", contenu);
			if (!contenu)
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * Vérifie qu'un ensemble ne contient qu'un seul exemplaire de chacun
	 * de ses éléments
	 * @param testName le nom du test dans lequel est employée cette méthode
	 * @param ensemble l'ensemble à tester
	 * @return true si chaque élément de l'ensemble n'existe qu'à un seul
	 * exemplaire.
	 */
	private static <E> boolean checkCount(String testName, Ensemble<E> ensemble)
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
	 * Mélange les éléments d'un tableau
	 * @param elements les élements à mélanger
	 * @return un tableau de même dimension avec les éléments dans un autre
	 * ordre
	 */
	private static String[] shuffleElements(String[] elements)
	{
		List<String> listElements = Arrays.asList(elements);

		Collections.shuffle(listElements);

		String[] result = new String[elements.length];
		int i = 0;
		for (String elt : listElements)
		{
			result[i++] = elt;
		}

		return result;
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
	public AllEnsembleTest(Class<? extends Ensemble<String>> typeEnsemble,
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
		int j = 0;
		for (int i = 0; i < elements1.length; i++)
		{
			elements[j++] = elements1[i];
		}
		for (int i = 0; i < elements2.length; i++)
		{
			elements[j++] = elements2[i];
		}
		System.out.println("-------------------------------------------------");
		System.out.println("Test des ensembles");
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
		System.out.println("Fin Test des ensembles");
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
		assertNotNull("setUp non null ensemble failed", ensemble);

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
	 * Test method for {@link ensembles.EnsembleVector#EnsembleVector()} or
	 * {@link ensembles.EnsembleListe#EnsembleListe()} or
	 * {@link ensembles.EnsembleTableau#EnsembleTableau()}
	 */
	@Test
	public final void testDefaultConstructor()
	{
		String testName = new String(typeName + "()");
		System.out.println(testName);

		ensemble = constructEnsemble(testName, typeEnsemble, null);
		assertNotNull(testName + " non null instance failed", ensemble);

		assertEquals(testName + " instance type failed", typeEnsemble,
				ensemble.getClass());
		assertTrue(testName +  " empty instance failed", ensemble.estVide());
		assertEquals(testName + " instance size failed", 0, ensemble.cardinal());
	}

	/**
	 * Test method for {@link ensembles.EnsembleVector#EnsembleVector(Iterable)}
	 * or {@link ensembles.EnsembleListe#EnsembleListe(Iterable)} or
	 * {@link ensembles.EnsembleTableau#EnsembleTableau(Iterable)}
	 */
	@Test
	public final void testCopyConstructor()
	{
		String testName = new String(typeName + "(Iterable)");
		System.out.println(testName);

		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed", ensemble);

		assertEquals(testName + " instance type failed", typeEnsemble,
				ensemble.getClass());
		assertFalse(testName +  " not empty instance failed", ensemble.estVide());
		boolean compare = compareElts2Array(testName, ensemble, allSingleElements);
		assertTrue(testName + "elts compare failed", compare);

		// Tous les éléments de ensemble doivent se retrouver dans list
		for (String elt : ensemble)
		{
			assertTrue(testName + "check content [" + elt + "] failed",
					listElements.contains(elt));
		}

		// Tous les éléments de l'ensemble n'existent qu'à un seul exemplaire
		boolean countCheck = AllEnsembleTest.<String>checkCount(testName, ensemble);

		assertTrue(testName + "after count check failed", countCheck);
	}

	/**
	 * Test method for {@link ensembles.Ensemble#ajout(java.lang.Object)}.
	 */
	@Test
	public final void testAjout()
	{
		String testName = new String(typeName + ".ajout(E)");
		System.out.println(testName);

		// Ensemble vide avant remplissage
		assertEquals(testName + " ensemble vide failed", 0, ensemble.cardinal());
		int count = 0;
		for (String elt : elements)
		{
			if (!ensemble.contient(elt))
			{
				count++;
			}
			ensemble.ajout(elt);
		}
		// Ensemble non vide après remplissage
		assertEquals(testName + " ensemble rempli failed", count,
				ensemble.cardinal());

		// Verif taille ensemble
		boolean countCheck = AllEnsembleTest.<String>checkCount(testName, ensemble);
		assertTrue(testName + "after count check failed", countCheck);

		// Comparaison des elts avec allSingleElements
		boolean compare = compareElts2Array(testName, ensemble, allSingleElements);
		assertTrue(testName + "elts compare failed", compare);

		// Ajout d'un elt null
		boolean ajoutNull = ensemble.ajout(null);
		assertFalse(testName + " ajout null is true", ajoutNull);
	}

	/**
	 * Test method for {@link ensembles.Ensemble#retrait(java.lang.Object)}.
	 */
	@Test
	public final void testRetrait()
	{
		String testName = new String(typeName + ".retrait(E)");
		System.out.println(testName);

		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed", ensemble);

		String[] elementsToRemove = shuffleElements(allSingleElements);

		for (String elt : elementsToRemove)
		{
			ensemble.retrait(elt);

			assertFalse(testName + "no more contains " + elt + " failed",
					ensemble.contient(elt));
		}

		assertTrue(testName + " ensemble vide après retraits failed",
				ensemble.estVide());
	}

	/**
	 * Test method for {@link ensembles.Ensemble#estVide()}.
	 */
	@Test
	public final void testEstVide()
	{
		String testName = new String(typeName + ".estVide()");
		System.out.println(testName);

		assertTrue(testName + " ensemble vide failed", ensemble.estVide());
		assertFalse(testName + " ens vide rien à itérer failed",
				ensemble.iterator().hasNext());

		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed", ensemble);
		assertFalse(testName + " ensemble vide failed", ensemble.estVide());
		assertTrue(testName + " ens non vide iterable failed",
				ensemble.iterator().hasNext());
	}

	/**
	 * Test method for {@link ensembles.Ensemble#contient(java.lang.Object)}.
	 */
	@Test
	public final void testContientENull()
	{
		String testName = new String(typeName + ".contient((E)null)");
		System.out.println(testName);
		String mot = null;

		// Contient null sur ensemble vide
		assertFalse(testName + " ens vide !contient(null) failed",
				ensemble.contient(mot));

		// remplissage ensemble
		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed", ensemble);
		assertEquals(testName + " instance remplie failed",
				allSingleElements.length, ensemble.cardinal());

		// Contient null sur ensemble non vide
		assertFalse(testName + " ens plein !contient(null) failed",
				ensemble.contient((String) null));
	}

	/**
	 * Test method for {@link ensembles.Ensemble#contient(java.lang.Object)}.
	 */
	@Test
	public final void testContientE()
	{
		String testName = new String(typeName + ".contient(E)");
		System.out.println(testName);
		String mot = new String("Bonjour");

		// Contient mot quelconque sur ensemble vide
		assertFalse(testName + " ens vide !contient(" + mot + ") failed",
				ensemble.contient(mot));

		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed", ensemble);

		// Contient mot quelconque sur ensemble non vide
		assertFalse(testName + " ens vide contient(" + mot + ") failed",
				ensemble.contient(mot));

		// Contient mots contenus
		boolean compare = compareElts2Array(testName, ensemble, allSingleElements);
		assertTrue(testName + " elts compare failed", compare);
	}

	/**
	 * Test method for {@link ensembles.Ensemble#contient(ensembles.Ensemble)}.
	 */
	@Test
	public final void testContientEnsembleNull()
	{
		String testName = new String(typeName + ".contient((Ensemble<E>)null)");
		System.out.println(testName);

		// !Contient ensemble null dans ensemble vide
		assertFalse(testName + "ens vide !contient(null) failed",
				ensemble.contient((Ensemble<String>) null));

		// !Contient ensemble null dans ensemble plein
		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed", ensemble);
		assertEquals(testName + " instance remplie taille failed",
				allSingleElements.length, ensemble.cardinal());

		assertFalse(testName + "ens plein non !contient(null) failed",
				ensemble.contient((Ensemble<String>) null));
	}

	/**
	 * Test method for {@link ensembles.Ensemble#contient(ensembles.Ensemble)}.
	 */
	@Test
	public final void testContientEnsembleOfE()
	{
		for (int i = 0; i < typesEnsemble.length; i++)
		{
			Class<? extends Ensemble<String>> otherType = typesEnsemble[i];
			String otherTypeName = otherType.getSimpleName();

			String testName = new String(typeName + ".contient("
					+ otherTypeName + "<E>)");
			System.out.println(testName);

			// sous ensemble vide
			Ensemble<String> sousEnsemble = constructEnsemble(testName,
					typesEnsemble[i], null);
			assertNotNull(testName + " sousEnsemble non null instance failed",
					sousEnsemble);

			// Contient sous ensemble vide dans ensemble vide
			assertTrue(testName + " ens vide contient sous ens["
					+ typesEnsemble[i].getSimpleName() + "] vide failed",
					ensemble.contient(sousEnsemble));

			// remplissage ensemble
			for (String elt : elements1)
			{
				ensemble.ajout(elt);
			}

			// Contient sous ensemble vide dans ensemble non vide
			assertTrue(testName + " ens plein contient sous ens["
					+ typesEnsemble[i].getSimpleName() + "] vide failed",
					ensemble.contient(sousEnsemble));

			// remplissage sous ensemble
			for (int j = 0; j < (elements1.length / 2); j++)
			{
				sousEnsemble.ajout(elements1[j]);
			}

			// Contient sous ensemble non vide ds ens non vide
			assertTrue(testName + " ens plein contient sous ens["
					+ typesEnsemble[i].getSimpleName() + "] failed",
					ensemble.contient(sousEnsemble));

			// !Contient sous ensemble non vide non contenu ds ens non vide
			sousEnsemble.ajout("consectetur");
			assertFalse(testName + " ens plein !contient sous ens["
					+ typesEnsemble[i].getSimpleName() + "] failed",
					ensemble.contient(sousEnsemble));

			ensemble.efface();
		}
	}

	/**
	 * Test method for {@link ensembles.Ensemble#efface()}.
	 */
	@Test
	public final void testEfface()
	{
		String testName = new String(typeName + ".efface()");
		System.out.println(testName);

		assertTrue(testName + "ens vide avant effacement failed",
				ensemble.estVide());

		// Effacement ensemble vide
		ensemble.efface();
		assertTrue(testName + "ens vide après effacement failed", ensemble.estVide());

		// Effacement ensemble non vide
		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed", ensemble);
		assertFalse(testName + "ens non vide après remplissage failed",
				ensemble.estVide());
		ensemble.efface();
		assertTrue(testName + "ens vide après remplissage & effacement failed",
				ensemble.estVide());
	}

	/**
	 * Test method for {@link ensembles.Ensemble#cardinal()}.
	 */
	@Test
	public final void testCardinal()
	{
		String testName = new String(typeName + ".cardinal()");
		System.out.println(testName);

		assertTrue(testName + " ensemble vide failed", ensemble.estVide());
		assertEquals(testName + " cardinal 0 sur ensemble vide failed", 0,
				ensemble.cardinal());

		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed", ensemble);

		assertFalse(testName + " ensemble non vide failed", ensemble.estVide());
		assertEquals(testName + " cardinal " + allSingleElements.length
				+ " sur ensemble rempli failed", allSingleElements.length,
				ensemble.cardinal());
	}

	/**
	 * Test method for {@link ensembles.Ensemble#union(ensembles.Ensemble)}.
	 */
	@Test
	public final void testUnion()
	{
		for (int i = 0; i < typesEnsemble.length; i++)
		{
			Class<? extends Ensemble<String>> otherType = typesEnsemble[i];
			String otherTypeName = otherType.getSimpleName();

			String testName = new String(typeName + ".union(" + otherTypeName
					+ "<E>)");
			System.out.println(testName);

			// remplissage ensemble avec singleElements
			for (String elt : elements1)
			{
				ensemble.ajout(elt);
			}

			// remplissage other avec singleElements2
			Ensemble<String> other = constructEnsemble(testName,
					typesEnsemble[i], null);
			assertNotNull(testName + " other instance non null failed", other);
			for (String elt : elements2)
			{
				other.ajout(elt);
			}

			Ensemble<String> union = ensemble.union(other);

			assertNotNull(testName + " non null union instance failed", union);
			assertFalse(testName + " self union", ensemble == union);
			assertFalse(testName + " self union", other == union);
			assertEquals(testName + " taille failed",
					allSingleElements.length, union.cardinal());
			boolean compare = compareElts2Array(testName, union,
					allSingleElements);
			assertTrue(testName + " elts compare failed", compare);
		}
	}

	/**
	 * Test method for {@link ensembles.Ensemble#intersection(ensembles.Ensemble)}.
	 */
	@Test
	public final void testIntersection()
	{
		for (int i = 0; i < typesEnsemble.length; i++)
		{
			Class<? extends Ensemble<String>> otherType = typesEnsemble[i];
			String otherTypeName = otherType.getSimpleName();

			String testName = new String(typeName + ".intersection("
					+ otherTypeName + "<E>)");
			System.out.println(testName);

			// remplissage ensemble avec singleElements
			for (String elt : elements1)
			{
				ensemble.ajout(elt);
			}

			// remplissage other avec singleElements2
			Ensemble<String> other = constructEnsemble(testName,
					typesEnsemble[i], null);
			assertNotNull(testName + " other non null instance failed", other);
			for (String elt : elements2)
			{
				other.ajout(elt);
			}

			Ensemble<String> intersection = ensemble.intersection(other);

			assertNotNull(testName + " non null intersection instance failed",
					intersection);
			assertFalse(testName + " self intersection", ensemble == intersection);
			assertFalse(testName + " self intersection", other == intersection);
			assertEquals(testName + " taille failed",
					commonSingleElements.length, intersection.cardinal());
			boolean compare = compareElts2Array(testName, intersection,
					commonSingleElements);
			assertTrue(testName + " elts compare failed", compare);
		}
	}

	/**
	 * Test method for {@link ensembles.Ensemble#complement(ensembles.Ensemble)}.
	 */
	@Test
	public final void testComplement()
	{
		for (int i = 0; i < typesEnsemble.length; i++)
		{
			Class<? extends Ensemble<String>> otherType = typesEnsemble[i];
			String otherTypeName = otherType.getSimpleName();

			String testName = new String(typeName + ".complement("
					+ otherTypeName + "<E>)");
			System.out.println(testName);

			// remplissage ensemble avec singleElements
			for (String elt : elements1)
			{
				ensemble.ajout(elt);
			}

			// remplissage other avec singleElements2
			Ensemble<String> other = constructEnsemble(testName,
					typesEnsemble[i], null);
			assertNotNull(testName + " other non null instance failed", other);
			for (String elt : elements2)
			{
				other.ajout(elt);
			}

			Ensemble<String> complement1 = ensemble.complement(other);

			assertNotNull(testName + " non null complement instance 1 failed",
					complement1);
			assertFalse(testName + " self complement1", ensemble == complement1);
			assertFalse(testName + " self complement1", other == complement1);
			assertEquals(testName + " taille 1 failed",
					complementElements1.length, complement1.cardinal());
			boolean compare = compareElts2Array(testName, complement1,
					complementElements1);
			assertTrue(testName + " elts compare 1 failed", compare);

			Ensemble<String> complement2 = other.complement(ensemble);

			assertNotNull(testName + " non null complement instance 2 failed",
					complement2);
			assertFalse(testName + " self complement2", ensemble == complement2);
			assertFalse(testName + " self complement2", other == complement2);
			assertEquals(testName + " taille 2 failed",
					complementElements2.length, complement2.cardinal());
			compare = compareElts2Array(testName, complement2,
					complementElements2);
			assertTrue(testName + "elts compare 2 failed", compare);
		}
	}

	/**
	 * Test method for {@link ensembles.Ensemble#difference(ensembles.Ensemble)}.
	 */
	@Test
	public final void testDifference()
	{
		for (int i = 0; i < typesEnsemble.length; i++)
		{
			Class<? extends Ensemble<String>> otherType = typesEnsemble[i];
			String otherTypeName = otherType.getSimpleName();

			String testName = new String(typeName + ".difference("
					+ otherTypeName + "<E>)");
			System.out.println(testName);

			// remplissage ensemble avec singleElements
			for (String elt : elements1)
			{
				ensemble.ajout(elt);
			}

			// remplissage other avec singleElements2
			Ensemble<String> other = constructEnsemble(testName,
					typesEnsemble[i], null);
			assertNotNull(testName + " other non null instance failed", other);

			for (String elt : elements2)
			{
				other.ajout(elt);
			}

			Ensemble<String> difference = ensemble.difference(other);

			assertNotNull(testName + " difference non null instance failed",
					difference);
			assertFalse(testName + " self difference", ensemble == difference);
			assertFalse(testName + " self difference", other == difference);
			assertEquals(testName + " taille failed", diffSingleElements.length,
					difference.cardinal());
			boolean compare = compareElts2Array(testName, difference,
					diffSingleElements);
			assertTrue(testName + " elts compare failed", compare);
		}
	}

	/**
	 * Test method for {@link ensembles.Ensemble#typeElements()}.
	 */
	@Test
	public final void testTypeElements()
	{
		String testName = new String(typeName + ".typeElements()");
		System.out.println(testName);

		assertNotNull(testName + " non null instance failed", ensemble);

		// type elt sur ensemble vide == null
		assertEquals(testName + " sur ens vide failed", null,
				ensemble.typeElements());

		// type elt sur ensemble non vide == String
		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed", ensemble);
		assertEquals(testName + " sur ens non vide failed", String.class,
				ensemble.typeElements());
	}

	/**
	 * Test method for {@link ensembles.Ensemble#equals(java.lang.Object)}.
	 */
	@Test
	public final void testEquals()
	{
		String testName = new String(typeName + ".equals(Object)");
		System.out.println(testName);

		// Equals sur null
		assertFalse(testName + " sur null failed", ensemble.equals(null));

		// Equals sur this
		assertTrue(testName + " sur this failed", ensemble.equals(ensemble));

		// Equals sur autre objet
		assertFalse(testName + " sur Object failed",
				ensemble.equals(new Object()));

		// remplissage ensemble
		for (String elt : allSingleElementsSorted)
		{
			ensemble.ajout(elt);
		}

		String[] allsingleElementsShuffle = shuffleElements(allSingleElements);

		for (int i = 0; i < typesEnsemble.length; i++)
		{
			Class<? extends Ensemble<String>> otherType = typesEnsemble[i];
			String otherTypeName = otherType.getSimpleName();

			Ensemble<String> other = constructEnsemble(testName,
					typesEnsemble[i], null);

			// Equals sur Ensemble même contenu même ordre
			assertNotNull(testName + " other non null instance failed", other);
			for(String elt : allSingleElementsSorted)
			{
				other.ajout(elt);
			}
			assertEquals(testName + " ens identique, ordre identique["
					+ otherTypeName + "] failed", ensemble, other);

			// Equals sur Ensemble même contenu ordre différent
			other.efface();
			for(String elt : allsingleElementsShuffle)
			{
				other.ajout(elt);
			}

			// ensemble est toujours sorted car construit avec
			// allSingleElementsSorted
			if ((ensemble instanceof EnsembleTri<?>) &&
			   !(other instanceof EnsembleTri<?>))
			{
				assertFalse(testName + " ens identique, ordre différent["
						+ otherTypeName + "] failed", ensemble.equals(other));
			}
			else
			{
				assertEquals(testName + " ens identique, ordre différent["
						+ otherTypeName + "] failed", ensemble, other);
			}

			// Equals sur Ensemble contenu différent
			other.ajout("bonjour");
			assertFalse(testName + " ens différent failed",
					ensemble.equals(other));
		}
	}

	/**
	 * Test method for {@link ensembles.Ensemble#hashCode()}.
	 */
	@Test
	public final void testHashCode()
	{
		String testName = new String(typeName + ".hashCode()");
		System.out.println(testName);
		int hash;
		boolean trie = ensemble instanceof EnsembleTri<?>;
		if (trie)
		{
			hash = 1;
		}
		else
		{
			hash = 0;
		}

		// hash code ensemble vide ==
		//	0 pour les Ensemble
		//	1 pour les EnsembleTri
		assertEquals(testName + " hashcode ens vide failed", hash,
				ensemble.hashCode());

		// hash code ensemble non vide ==
		//	somme des hashcode des elts pour les Ensemble
		//	comme les collections pour les EnsembleTri
		for (String elt : allSingleElements)
		{
			ensemble.ajout(elt);
		}
		if (trie)
		{
			final int prime = 31;
			for (String elt : allSingleElementsSorted)
			{
				hash = (prime * hash) + (elt == null ? 0 : elt.hashCode());
			}
		}
		else
		{
			for (String elt : allSingleElements)
			{
				hash += elt.hashCode();
			}
		}

		assertEquals(testName + " hashcode ens non vide failed", hash,
				ensemble.hashCode());
	}

	/**
	 * Test method for {@link ensembles.Ensemble#toString()}.
	 */
	@Test
	public final void testToString()
	{
		String testName = new String(typeName + ".toString()");
		System.out.println(testName);

		ensemble = constructEnsemble(testName, typeEnsemble, listElements);
		assertNotNull(testName + " non null instance failed", ensemble);

		StringBuilder sb = new StringBuilder();
		sb.append("[");
		Iterator<String> it = ensemble.iterator();
		if (it != null)
		{
			for (; it.hasNext();)
			{
				sb.append(it.next().toString());
				if (it.hasNext())
				{
					sb.append(", ");
				}
			}
			sb.append("]");

			String expected = sb.toString();

			assertEquals(testName, expected, ensemble.toString());
		}
		else
		{
			fail(testName + " null iterator");
		}
	}

	/**
	 * Test method for {@link ensembles.Ensemble#iterator()}.
	 */
	@Test
	public final void testIterator()
	{
		String testName = new String(typeName + ".iterator()");
		System.out.println(testName);

		Iterator<String> it = null;

		// iterator existe
		it = ensemble.iterator();
		assertNotNull(testName +  " non null instance failed", it);

		// iterator sur ens vide n'a pas d'elts à itérer
		assertFalse(testName + " !hasNext() sur ens vide failed", it.hasNext());

		// remplissage
		for (String elt : allSingleElements)
		{
			ensemble.ajout(elt);
		}

		it = ensemble.iterator();

		// iterator sur ens rempli
		assertTrue(testName + " hashNext() sur ens rempli failed", it.hasNext());

		String[] array;
		if (ensemble instanceof EnsembleTri<?>)
		{
			array = allSingleElementsSorted;
		}
		else
		{
			array = allSingleElements;
		}

		// comparaison des elts
		for(int i = 0; (i < array.length) && it.hasNext(); i++)
		{
			assertEquals(testName + "check elt:" + array[i] + " failed",
					array[i], it.next());
		}

		// plus l'elts à itérer
		assertFalse(testName + " !hasNext() fin comparaison failed",
				it.hasNext());

		// retrait des elts avec l'itérateur
		it = ensemble.iterator();
		for (int i = 0; (i < array.length) && it.hasNext(); i++)
		{
			it.next();
			it.remove();
			assertFalse(testName + " retrait elt:" + array[i] + " failed",
					ensemble.contient(array[i]));
		}

		// plus l'elts à itérer
		assertFalse(testName + " !hasNext() fin retrait failed", it.hasNext());
		assertTrue(testName + " ens vide après retraits failed",
				ensemble.estVide());
	}
}
