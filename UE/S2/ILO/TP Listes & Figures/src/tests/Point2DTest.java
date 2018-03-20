package tests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import points.Point2D;

/**
 * Class de test de la classe {@link Point2D}
 *
 * @author David
 */
public class Point2DTest
{
	/**
	 * Le point2D à tester
	 */
	private Point2D point;

	/**
	 * Liste de points
	 */
	private ArrayList<Point2D> points;

	/**
	 * Etendue max pour les random
	 */
	private static final double maxRandom = 1e9;

	/**
	 * Nombre d'essais pour les tests
	 */
	private static final long nbTrials = 1000;

	/**
	 * Nombre de subdivisions pour les étendues lors des tests
	 */
	private static final int nbSteps = 100;

	/**
	 * Constructeur de la classe de test. Initialise les attributs utilisés dans
	 * les tests
	 */
	public Point2DTest()
	{
		point = null;
		points = new ArrayList<Point2D>();
	}

	/**
	 * Mise en place avant tous les tests
	 *
	 * @throws java.lang.Exception
	 */
	@BeforeClass
	public static void setUpBeforeClass() throws Exception
	{
		// Rien
	}

	/**
	 * Nettoyage après tous les tests
	 *
	 * @throws java.lang.Exception
	 */
	@AfterClass
	public static void tearDownAfterClass() throws Exception
	{
		// Rien
	}

	/**
	 * Mise en place avant chaque test
	 *
	 * @throws java.lang.Exception
	 */
	@Before
	public void setUp() throws Exception
	{
		// rien
	}

	/**
	 * Nettoyage après chaque test
	 *
	 * @throws java.lang.Exception
	 */
	@After
	public void tearDown() throws Exception
	{
		point = null;
		System.gc();
	}

	/**
	 * Assertion de la valeur de x du point "point"
	 *
	 * @param message le message associé à l'assertion
	 * @param value la valeur attendue
	 * @param tolerance la tolérance de la valeur
	 */
	private void assertX(String message, double value, double tolerance)
	{
		assertEquals(message, value, point.getX(), tolerance);
	}

	/**
	 * Assertion de la valeur de y du point "point"
	 *
	 * @param message le message associé à l'assertion
	 * @param value la valeur attendue
	 * @param tolerance la tolérance de la valeur
	 */
	private void assertY(String message, double value, double tolerance)
	{
		assertEquals(message, value, point.getY(), tolerance);
	}

	/**
	 * Génère un nombre aléatoire compris entre [0...maxValue[
	 *
	 * @param maxValue la valeur max du nombre aléatoire
	 * @return un nombre aléatoire compris entre [0...maxValue[
	 */
	private double randomNumber(double maxValue)
	{
		return Math.random() * maxValue;
	}

	/**
	 * Génère un nombre aléatoire compris entre [-range...range[
	 *
	 * @param range l'étendue du nombre aléatoire généré
	 * @return un nombre aléatoire compris entre [-range...range[
	 */
	private double randomRange(double range)
	{
		return (Math.random() - 0.5) * 2.0 * range;
	}

	/**
	 * Test method for {@link points.Point2D#Point2D()}.
	 */
	@Test
	public void testPoint2D()
	{
		String testName = new String("Point2D()");
		System.out.println(testName);

		point = new Point2D();

		assertNotNull(testName + " instance", point);
		assertX(testName + ".getX() == 0.0", 0.0, 0.0);
		assertY(testName + ".getY() == 0.0", 0.0, 0.0);
		assertTrue(testName + ".getNbPoints()", Point2D.getNbPoints() > 0);
	}

	/**
	 * Test method for {@link points.Point2D#Point2D(double, double)}.
	 */
	@Test
	public void testPoint2DDoubleDouble()
	{
		String testName = new String("Point2D(double, double)");
		System.out.println(testName);

		double valueX = 1.0;
		double valueY = Double.NaN;
		point = new Point2D(valueX, valueY);
		assertNotNull(testName + " instance", point);
		assertX(testName + ".getX() == 1.0", valueX, 0.0);
		assertY(testName + ".getY() == NaN", valueY, 0.0);
	}

	/**
	 * Test method for {@link points.Point2D#Point2D(points.Point2D)}.
	 */
	@Test
	public void testPoint2DPoint2D()
	{
		String testName = new String("Point2D(Point2D)");
		System.out.println(testName);

		Point2D specimen = new Point2D(randomNumber(maxRandom),
				randomNumber(maxRandom));
		assertNotNull(testName + " instance specimen", specimen);

		point = new Point2D(specimen);
		assertNotNull(testName + " instance copie", point);
		assertX(testName + ".getX() == " + specimen.getX(), specimen.getX(),
				0.0);
		assertY(testName + ".getY() == " + specimen.getY(), specimen.getY(),
				0.0);
	}

	/**
	 * Test method for {@link points.Point2D#getX()}.
	 */
	@Test
	public void testGetX()
	{
		String testName = new String("Point2D.getX()");
		System.out.println(testName);

		point = new Point2D(1.0, 0.0);
		assertNotNull(testName + "instance", point);
		assertEquals(testName + ".getX() == 1.0", 1.0, point.getX(), 0.0);
	}

	/**
	 * Test method for {@link points.Point2D#getY()}.
	 */
	@Test
	public void testGetY()
	{
		String testName = new String("Point2D.getY()");
		System.out.println(testName);

		point = new Point2D(0.0, 1.0);
		assertNotNull(testName + "instance", point);
		assertEquals(testName + ".getY() == 1.0", 1.0, point.getY(), 0.0);
	}

	/**
	 * Test method for {@link points.Point2D#setX(double)}.
	 */
	@Test
	public void testSetX()
	{
		String testName = new String("Point2D.setX(double)");
		System.out.println(testName);

		point = new Point2D();
		assertNotNull(testName + " instance", point);
		assertEquals(testName + ".getX() == 0.0", 0.0, point.getX(), 0.0);
		point.setX(2.0);
		assertEquals(testName + ".getX() == 2.0", 2.0, point.getX(), 0.0);
	}

	/**
	 * Test method for {@link points.Point2D#setY(double)}.
	 */
	@Test
	public void testSetY()
	{
		String testName = new String("Point2D.setY(double)");
		System.out.println(testName);

		point = new Point2D();
		assertNotNull(testName + " instance", point);
		assertEquals(testName + ".getY() == 0.0", 0.0, point.getY(), 0.0);
		point.setY(2.0);
		assertEquals(testName + ".getY() == 2.0", 2.0, point.getY(), 0.0);
	}

	/**
	 * Test method for {@link points.Point2D#getEpsilon()}.
	 */
	@Test
	public void testGetEpsilon()
	{
		String testName = new String("Point2D.getEpsilon()");
		System.out.println(testName);

		double result = Point2D.getEpsilon();
		assertEquals(testName, 1e-6, result, 0.0);
	}

	/**
	 * Test method for {@link points.Point2D#getNbPoints()}.
	 */
	@Test
	public void testGetNbPoints()
	{
		String testName = new String("Point2D.getNbPoints()");
		System.out.println(testName);

		point = new Point2D();
		/*
		 * On ne sait pas combien de points sont encore en mémoire : cela dépend
		 * du Garbage Collector. On peut donc juste vérifier qu'il y en a au
		 * moins un
		 */
		assertTrue(testName, Point2D.getNbPoints() >= 1);
	}

	/**
	 * Test method for {@link points.Point2D#toString()}.
	 */
	@Test
	public void testToString()
	{
		String testName = new String("Point2D.toString()");
		System.out.println(testName);

		point = new Point2D(Math.PI, Math.E);
		String expectedString = new String(
				"x = 3.141592653589793 y = 2.718281828459045");
		String result = point.toString();
		assertEquals(testName, expectedString, result);
	}

	/**
	 * Test method for {@link points.Point2D#deplace(double, double)}.
	 */
	@Test
	public void testDeplace()
	{
		String testName = new String("Point2D.deplace(double, double)");
		System.out.println(testName);

		point = new Point2D();
		double origineX = point.getX();
		double origineY = point.getY();
		double deltaX = 5.0;
		double deltaY = 3.0;

		point.deplace(deltaX, deltaY);
		assertEquals(testName + ".getX() après +delta", origineX + deltaX,
				point.getX(), 0.0);
		assertEquals(testName + ".getY() après +delta", origineY + deltaY,
				point.getY(), 0.0);

		Point2D retour = point.deplace(-deltaX, -deltaY);
		double tolerance = Point2D.getEpsilon();
		assertSame(testName + " return == point déplacé", point, retour);
		assertEquals(testName + ".getX() après -delta", origineX, point.getX(),
				tolerance);
		assertEquals(testName + ".getY() après -delta", origineY, point.getY(),
				tolerance);
	}

	/**
	 * Test method for
	 * {@link points.Point2D#distance(points.Point2D, points.Point2D)}.
	 */
	@Test
	public void testDistancePoint2DPoint2D()
	{
		String testName = new String("Point2D.distance(Point2D, Point2D)");
		System.out.println(testName);

		double radius = randomNumber(maxRandom);
		double angleStep = Math.PI / nbSteps;

		// Distances entre deux points diamétralement opposés le long d'un
		// cercle
		for (double angle = 0.0; angle < (Math.PI * 2.0); angle += angleStep)
		{
			points.clear();
			double x = radius * Math.cos(angle);
			double y = radius * Math.sin(angle);
			points.add(new Point2D(x, y));
			points.add(new Point2D(-x, -y));

			assertEquals(testName + "p0p1[" + String.valueOf(angle) + "]",
					radius * 2.0,
					Point2D.distance(points.get(0), points.get(1)),
					Point2D.getEpsilon());
			assertEquals(testName + "p1p0[" + String.valueOf(angle) + "]",
					radius * 2.0,
					Point2D.distance(points.get(1), points.get(0)),
					Point2D.getEpsilon());
		}
	}

	/**
	 * Test method for {@link points.Point2D#distance(points.Point2D)}.
	 */
	@Test
	public void testDistancePoint2D()
	{
		String testName = new String("Point2D.distance(Point2D)");
		System.out.println(testName);

		double origineX = randomRange(maxRandom);
		double origineY = randomRange(maxRandom);
		point = new Point2D(origineX, origineY);
		double radius = randomNumber(maxRandom);
		double angleStep = Math.PI / nbSteps;

		// Distance entre un point fixe (point) et des points le long
		// d'un cercle l'entourant (p)
		for (double angle = 0.0; angle < (Math.PI * 2.0); angle += angleStep)
		{
			Point2D p = new Point2D(origineX + (radius * Math.cos(angle)),
					origineY + (radius * Math.sin(angle)));

			assertEquals(testName + "this,p[" + String.valueOf(angle) + "]",
					radius, point.distance(p), Point2D.getEpsilon());
			assertEquals(testName + "p,this[" + String.valueOf(angle) + "]",
					radius, p.distance(point), Point2D.getEpsilon());
		}
	}

	/**
	 * Test method for {@link points.Point2D#equals(java.lang.Object)}.
	 */
	@Test
	public void testEqualsObject()
	{
		String testName = new String("Point2D.equals(Object)");
		System.out.println(testName);

		point = new Point2D(randomRange(maxRandom), randomRange(maxRandom));
		Object o = new Object();

		// Inégalité avec un objet null
		assertFalse(testName + " sur null", point.equals(null));

		// Inégalité avec un objet de nature différente
		assertFalse(testName + " sur Object", point.equals(o));

		// Egalité avec soi même
		Object opoint = point;
		assertEquals(testName + " sur this", point, opoint);

		// Egalité avec une copie de soi même
		Point2D otherPoint = new Point2D(point);
		Object op = otherPoint;
		assertEquals(testName + " sur copie", point, op);
		double epsilon = Point2D.getEpsilon();

		// Egalité avec un point déplacé de epsilon au plus
		for (long i = 0; i < nbTrials; i++)
		{
			otherPoint.setX(point.getX());
			otherPoint.setY(point.getY());
			double radius = randomNumber(epsilon);
			double angle = randomNumber(Math.PI * 2.0);
			otherPoint.deplace(
					radius * Math.cos(angle),
					radius * Math.sin(angle));
			double distance = point.distance(otherPoint);

			/*
			 * Attention, à cause des approximations dues aux cos et sin
			 * le déplacement peut être légèrement supérieure à epsilon
			 */
			if (distance < epsilon)
			{
				assertEquals(testName + " point déplacé < epsilon [" + distance
						+ "]", point, otherPoint);
			}
			else
			{
				assertFalse(testName + " point déplacé >= epsilon [" + distance
						+ "]", point.equals(op));
			}
		}

		// Inégalité avec un point déplacé
		for (long i = 0; i < nbTrials; i++)
		{
			otherPoint.setX(point.getX());
			otherPoint.setY(point.getY());
			otherPoint.deplace(randomRange(maxRandom), randomRange(maxRandom));
			double distance = point.distance(otherPoint);

			if (distance < epsilon)
			{
				assertEquals(testName + " point déplacé proche [" + distance
						+ "]", point, otherPoint);
			}
			else
			{
				assertFalse(testName + " point déplacé loin [" + distance + "]",
						point.equals(otherPoint));
			}
		}
	}
}
