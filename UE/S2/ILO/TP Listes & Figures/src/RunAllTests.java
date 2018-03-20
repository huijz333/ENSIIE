import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

import tests.AllTests;

/**
 * Exécution de tous les tests du TP1
 *
 * @author davidroussel
 */
public class RunAllTests {

	/**
	 * Programme principal de lancement des tests
	 * 
	 * @param args
	 *            non utilisés
	 */
	public static void main(String[] args) {
		System.out.println("Tests du TP1");

		Result result = JUnitCore.runClasses(AllTests.class);

		int failureCount = result.getFailureCount();

		if (failureCount == 0) {
			System.out.println("Every thing went fine");
		} else {
			for (Failure failure : result.getFailures()) {
				System.err.println("Failure : " + failure.toString());
			}
		}
	}
}
