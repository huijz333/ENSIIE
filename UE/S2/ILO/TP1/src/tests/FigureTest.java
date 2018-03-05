package tests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

import figures.Cercle;
import figures.Figure;
import figures.Groupe;
import figures.Polygone;
import figures.Rectangle;
import figures.Triangle;
import points.Point2D;

/**
 * Classe de test de l'ensemble des figures
 * 
 * @author davidroussel
 */
@RunWith(value = Parameterized.class)
public class FigureTest<F extends Figure> {
	/**
	 * La figure courante à tester
	 */
	private F testFigure = null;

	/**
	 * La classe de la figure à tester (pour invoquer ses constructeurs)
	 */
	private Class<F> figureDefinition = null;

	/**
	 * Le nom/type de la figure courante à tester
	 */
	private String typeName;

	/**
	 * Tolérance pour les comparaisons numériques (aires, distances)
	 */
	private static final double tolerance = Point2D.getEpsilon();

	/**
	 * Les différentes natures de figures à tester
	 */
	@SuppressWarnings("unchecked")
	private static final Class<? extends Figure>[] figureTypes = (Class<? extends Figure>[]) new Class<?>[] {
			Cercle.class, Rectangle.class, Triangle.class, Polygone.class, Groupe.class };

	/**
	 * L'ensemble des figures à tester
	 */
	private static final Figure[] figures = new Figure[figureTypes.length];

	/**
	 * Autre ensemble (distinct) de figures à tester pour l'égalité;
	 */
	private static final Figure[] altFigures = new Figure[figureTypes.length];

	/**
	 * la map permettant d'obtenir la figure en fonction de son nom. Sera construite
	 * à partir de {@link #noms} et de {@link #figures}
	 */
	private static Map<String, Figure> figuresMap = new HashMap<String, Figure>();

	/**
	 * Les points à utiliser pour construire les figures
	 */
	private static final Point2D[][] points = new Point2D[][] { { new Point2D(7, 3) }, // Cercle
			{ new Point2D(4, 1), new Point2D(8, 4) }, // Rectangle
			{ new Point2D(3, 2), new Point2D(7, 3), new Point2D(4, 6) }, // Triangle
			{ new Point2D(5, 1), new Point2D(8, 2), new Point2D(7, 5), new Point2D(2, 4), new Point2D(2, 3) } // Polygone
	};

	/**
	 * Nom des différentes figures à tester
	 */
	private static final String[] noms = new String[figureTypes.length];

	/**
	 * Index dans le tableau de noms {@link #noms} à partir d'un nom.
	 */
	private static Map<String, Integer> nomsIndex = new HashMap<String, Integer>();

	/**
	 * Les différents centre des figures
	 */
	private static final Point2D[] centres = new Point2D[] { new Point2D(7, 3), // Cercle
			new Point2D(6, 2.5), // Rectangle
			new Point2D(4.666666666666667, 3.6666666666666665), // Triangle
			new Point2D(5.150537634408602, 3.053763440860215), // Polygone
			new Point2D() // Groupe : on le calculera plus tard
	};

	/**
	 * toString attendu des différentes figures
	 */
	private static String[] toStrings = new String[] { "Cercle : x = 7.0 y = 3.0, r = 2.0",
			"Rectangle : x = 4.0 y = 1.0, x = 8.0 y = 4.0",
			"Triangle : x = 3.0 y = 2.0, x = 7.0 y = 3.0, x = 4.0 y = 6.0",
			"Polygone : x = 5.0 y = 1.0, x = 8.0 y = 2.0, x = 7.0 y = 5.0, x = 2.0 y = 4.0, x = 2.0 y = 3.0", "" // Groupe
																													// =
																													// à
																													// recalculer
																													// d'après
																													// les
																													// précédents
	};

	/**
	 * aires attendues des différentes figures
	 */
	private static double[] aires = new double[] { 12.566371, // Cercle
			12.0, // Rectangle
			7.5, // Triangle
			15.5, // Polygone
			47.566371 // Groupe
	};

	/**
	 * Distances entre les centres des figures
	 */
	private static double[][] interDistances = new double[][] {
			// Cercle Rect. Tri. Poly. Grp.
			{ 0.0, 1.118034, 2.426703, 1.850244, 1.296870 }, // Cercle
			{ 1.118034, 0.0, 1.771691, 1.014022, 0.628953 }, // Rectangle
			{ 2.426703, 1.771691, 0.0, 0.780885, 1.204446 }, // Triangle
			{ 1.850244, 1.014022, 0.780885, 0.0, 0.553765 }, // Polygone
			{ 1.296870, 0.628953, 1.204446, 0.553765, 0.0 } // Groupe
	};

	// /**
	// * Enum interne décrivant les indices des différentes figures dans les
	// * tableaux #figureTypes, #points, #centres, #toStrings, #aires,
	// * #interDistances
	// * @author davidroussel
	// *
	// */
	// static private enum Indices
	// {
	// CIRCLE,
	// RECTANGLE,
	// TRIANGLE,
	// POLYGON,
	// GROUP;
	//
	// public int toInt() throws IllegalArgumentException
	// {
	// switch(this)
	// {
	// case CIRCLE:
	// return 0;
	// case RECTANGLE:
	// return 1;
	// case TRIANGLE:
	// return 2;
	// case POLYGON:
	// return 3;
	// case GROUP:
	// return 4;
	// default :
	// throw new IllegalArgumentException("Indices::toInt");
	// }
	// }
	//
	// @Override
	// public String toString() throws IllegalArgumentException
	// {
	// switch(this)
	// {
	// case CIRCLE:
	// return new String("Cercle");
	// case RECTANGLE:
	// return new String("Rectangle");
	// case TRIANGLE:
	// return new String("Triangle");
	// case POLYGON:
	// return new String("Polygone");
	// case GROUP:
	// return new String("Groupe");
	// default :
	// throw new IllegalArgumentException("Indices::toString");
	// }
	//
	// }
	// }

	/**
	 * la map permettant d'obtenir le centre précalculé d'une figure en fonction de
	 * son nom. Sera construite à partir de {@link #noms} et de {@link #centres}
	 */
	private static Map<String, Point2D> centresMap = new HashMap<String, Point2D>();

	/**
	 * Un point à l'intérieur de toutes les figures
	 */
	private static final Point2D insidePoint = new Point2D(6, 3);

	/**
	 * Un point à l'extérieur de toutes les figures
	 */
	private static final Point2D outsidePoint = new Point2D(6, 5);

	/**
	 * Mise en place avant l'ensemble des tests
	 * 
	 * @throws java.lang.Exception
	 */
	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
		// remplissage des noms
		for (int i = 0; i < figureTypes.length; i++) {
			noms[i] = figureTypes[i].getSimpleName();
		}

		/*
		 * Premier ensmble de figures
		 */
		// Première figure = cercle
		figures[0] = new Cercle(points[0][0], 2);
		// Seconde figure = rectangle
		figures[1] = new Rectangle(points[1][0], points[1][1]);
		// Troisième figure = triangle
		figures[2] = new Triangle(points[2][0], points[2][1], points[2][2]);
		// Quatrième figure = polygone
		ArrayList<Point2D> polyPoints = new ArrayList<Point2D>();
		for (Point2D p : points[3]) {
			polyPoints.add(p);
		}
		figures[3] = new Polygone(polyPoints);
		// Cinquième figure : groupe de l'ensemble des 4 premières
		ArrayList<Figure> figureGroup = new ArrayList<Figure>();
		for (int i = 0; i < (figures.length - 1); i++) {
			figureGroup.add(figures[i]);
		}
		figures[4] = new Groupe(figureGroup);

		/*
		 * Second ensemble de figures
		 */
		// Première figure = cercle
		altFigures[0] = new Cercle(points[0][0], 2);
		// Seconde figure = rectangle
		altFigures[1] = new Rectangle(points[1][1], points[1][0]);
		// Troisième figure = triangle
		altFigures[2] = new Triangle(points[2][1], points[2][0], points[2][2]);
		// Quatrième figure = polygone
		polyPoints.clear();
		for (int i = 1; i <= points[3].length; i++) {
			polyPoints.add(points[3][i % points[3].length]);
		}
		altFigures[3] = new Polygone(polyPoints);
		// Cinquième figure : groupe de l'ensemble des 4 premières
		figureGroup.clear();
		for (int i = figures.length - 2; i >= 0; i--) {
			figureGroup.add(altFigures[i]);
		}
		altFigures[4] = new Groupe(figureGroup);

		// calcul du barycentre des 4 premières figures pour initialiser
		// le centre du groupe de figures
		int j = 0;
		double centreX = 0.0;
		double centreY = 0.0;
		for (; j < (figures.length - 1); j++) {
			Point2D centre = figures[j].getCentre();
			centreX += centre.getX();
			centreY += centre.getY();
		}

		centres[4].setX(centreX / j);
		centres[4].setY(centreY / j);

		// calcul du toString des Groupes
		StringBuilder sb = new StringBuilder("Groupe : ");
		for (int i = 0; i < (toStrings.length - 1); i++) {
			sb.append("\n" + toStrings[i]);
		}
		toStrings[4] = sb.toString();

		// construction des maps de
		// - figures
		// - centres
		for (int i = 0; i < figureTypes.length; i++) {
			figuresMap.put(noms[i], figures[i]);
			centresMap.put(noms[i], centres[i]);
			nomsIndex.put(noms[i], Integer.valueOf(i));
		}
	}

	/**
	 * Nettoyage après l'ensemble des tests
	 * 
	 * @throws java.lang.Exception
	 */
	@AfterClass
	public static void tearDownAfterClass() throws Exception {
		// rien
	}

	/**
	 * Mise en place avant chaque test
	 * 
	 * @throws java.lang.Exception
	 */
	@Before
	public void setUp() throws Exception {
		// rien
	}

	/**
	 * Nettoyage après chaque test
	 * 
	 * @throws java.lang.Exception
	 */
	@After
	public void tearDown() throws Exception {
		// rien
	}

	/**
	 * Paramètres à transmettre au constructeur de la classe de test.
	 *
	 * @return une collection de tableaux d'objet contenant les paramètres à
	 *         transmettre au constructeur de la classe de test
	 */
	@Parameters(name = "{index}:{1}")
	public static Collection<Object[]> data() {
		Object[][] data = new Object[figureTypes.length][2];
		for (int i = 0; i < figureTypes.length; i++) {
			data[i][0] = figureTypes[i];
			data[i][1] = figureTypes[i].getSimpleName();
		}
		return Arrays.asList(data);
	}

	/**
	 * Constructeur parametré par le type de figure à tester
	 * 
	 * @param typeFigure
	 *            le type de figure à tester
	 * @param typeName
	 *            le nom du type à tester (pour affichage)
	 */
	@SuppressWarnings("unchecked") // à cause du cast en F
	public FigureTest(Class<F> typeFigure, String typeName) {
		figureDefinition = typeFigure;
		this.typeName = typeName;
		testFigure = (F) figuresMap.get(typeName);
	}

	/**
	 * Test method for one of {@link figures.Figure} default constructor
	 */
	@Test
	public final void testFigureConstructor() {
		String testName = new String(typeName + "()");
		System.out.println(testName);
		Constructor<F> defaultConstructor = null;
		Class<?>[] constructorsArgs = new Class<?>[0];

		try {
			defaultConstructor = figureDefinition.getConstructor(constructorsArgs);
		} catch (SecurityException e) {
			fail(testName + " constructor security exception");
		} catch (NoSuchMethodException e) {
			fail(testName + " constructor not found");
		}

		if (defaultConstructor != null) {
			Object instance = null;
			try {
				instance = defaultConstructor.newInstance(new Object[0]);
			} catch (IllegalArgumentException e) {
				fail(testName + " wrong constructor arguments");
			} catch (InstantiationException e) {
				fail(testName + " instanciation exception");
			} catch (IllegalAccessException e) {
				fail(testName + " illegal access");
			} catch (InvocationTargetException e) {
				fail(testName + " invocation target exception");
			}

			assertNotNull(testName, instance);
			assertEquals(testName + " self equality", instance, instance);
		}
	}

	/**
	 * Test method for one of {@link figures.Figure} copy constructor
	 */
	@Test
	public final void testFigureConstructorFigure() {
		String testName = new String(typeName + "(" + typeName + ")");
		System.out.println(testName);
		Constructor<F> copyConstructor = null;
		Class<?>[] constructorsArgs = new Class<?>[] { figureDefinition };

		try {
			copyConstructor = figureDefinition.getConstructor(constructorsArgs);
		} catch (SecurityException e) {
			fail(testName + " constructor security exception");
		} catch (NoSuchMethodException e) {
			fail(testName + " constructor not found");
		}

		if (copyConstructor != null) {
			Object instance = null;
			try {
				instance = copyConstructor.newInstance(testFigure);
			} catch (IllegalArgumentException e) {
				fail(testName + " wrong constructor arguments");
			} catch (InstantiationException e) {
				fail(testName + " instanciation exception");
			} catch (IllegalAccessException e) {
				fail(testName + " illegal access");
			} catch (InvocationTargetException e) {
				fail(testName + " invocation target exception");
			}

			assertNotNull(testName, instance);
			assertEquals(testName + " equality", testFigure, instance);
		}
	}

	/**
	 * Test method for {@link figures.Figure#getNom()}.
	 */
	@Test
	public final void testGetNom() {
		String testName = new String(typeName + ".getNom()");
		System.out.println(testName);

		assertEquals(testName, noms[nomsIndex.get(typeName).intValue()], testFigure.getNom());
	}

	/**
	 * Test method for {@link figures.Figure#deplace(double, double)}.
	 */
	@Test
	public final void testDeplace() {
		String testName = new String(typeName + ".deplace(double, double)");
		System.out.println(testName);

		Point2D centreBefore = new Point2D(testFigure.getCentre());

		double dx = 1.0;
		double dy = 1.0;

		testFigure.deplace(dx, dy);

		Point2D centreAfter = testFigure.getCentre();

		assertEquals(testName, centreBefore.deplace(dx, dy), centreAfter);

		testFigure.deplace(-dx, -dy);
	}

	/**
	 * Test method for {@link figures.Figure#toString()}.
	 */
	@Test
	public final void testToString() {
		String testName = new String(typeName + ".toString()");
		System.out.println(testName);

		assertEquals(testName, toStrings[nomsIndex.get(typeName).intValue()], testFigure.toString());
	}

	/**
	 * Test method for {@link figures.Figure#contient(points.Point2D)}.
	 */
	@Test
	public final void testContient() {
		String testName = new String(typeName + ".contient(Point2D)");
		System.out.println(testName);

		assertTrue(testName + " inner point", testFigure.contient(insidePoint));

		assertFalse(testName + " outer point", testFigure.contient(outsidePoint));
	}

	/**
	 * Test method for {@link figures.Figure#getCentre()}.
	 */
	@Test
	public final void testGetCentre() {
		String testName = new String(typeName + ".getCentre()");
		System.out.println(testName);

		assertEquals(testName, centres[nomsIndex.get(typeName).intValue()], testFigure.getCentre());
	}

	/**
	 * Test method for {@link figures.Figure#aire()}.
	 */
	@Test
	public final void testAire() {
		String testName = new String(typeName + ".aire()");
		System.out.println(testName);

		assertEquals(testName, aires[nomsIndex.get(typeName).intValue()], testFigure.aire(), tolerance);
	}

	/**
	 * Test method for {@link figures.Figure#distanceToCentreOf(figures.Figure)}.
	 */
	@Test
	public final void testDistanceToCentreOf() {
		String testName = new String(typeName + ".distanceToCentreOf(Figure)");
		System.out.println(testName);

		for (int i = 0; i < figures.length; i++) {
			assertEquals(testName + "->" + noms[i], interDistances[nomsIndex.get(typeName).intValue()][i],
					testFigure.distanceToCentreOf(figures[i]), tolerance);
		}
	}

	/**
	 * Test method for {@link figures.Figure#equals(java.lang.Object)}.
	 */
	@Test
	public final void testEquals() {
		String testName = new String(typeName + ".equals(Object)");
		System.out.println(testName);

		// Inégalité avec null
		assertFalse(testName + " != null", testFigure.equals(null));

		// Egalité avec soi même
		assertEquals(testName + " == this", testFigure, testFigure);

		// Egalité / Inégalité avec le même ensemble de figures
		for (int i = 0; i < figures.length; i++) {
			if (nomsIndex.get(typeName).intValue() == i) {
				assertEquals(testName + " == (1) " + noms[i], testFigure, figures[i]);
			} else {
				assertFalse(testName + " != (1) " + noms[i], testFigure.equals(figures[i]));
			}
		}

		// Egalité / Inégalité avec l'autre ensemble de figures
		for (int i = 0; i < figures.length; i++) {
			if (nomsIndex.get(typeName).intValue() == i) {
				assertEquals(testName + " == (2) " + noms[i], testFigure, altFigures[i]);
			} else {
				assertFalse(testName + " != (2) " + noms[i], testFigure.equals(altFigures[i]));
			}
		}
	}
}
