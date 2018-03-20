package tests;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

/**
 * Suite de tests
 * 
 * @author davidroussel
 */
@RunWith(Suite.class)
@SuiteClasses({ ListeTest.class, CollectionListeTest.class, Point2DTest.class, FigureTest.class })
public class AllTests {
	// Nothing
}
