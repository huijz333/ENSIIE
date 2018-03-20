import java.util.ArrayList;

import listes.CollectionListe;
import listes.Liste;

/**
 * Classe de test de la Liste et de la CollectionListe
 *
 * @author davidroussel
 */
public class TestListe {

	/**
	 * Programme principal de test des {@link Liste} et {@link CollectionListe}
	 *
	 * @param args
	 *            arguments (non utilisés ici)
	 */
	@SuppressWarnings("unlikely-arg-type")
	public static void main(String[] args) {
		String[] mots = { "mot1", "mot2", "mot3", "mot4", "mot5", "mot6", "mot7" };

		// --------------------------------------------------------------------
		// Liste<String>
		// --------------------------------------------------------------------
		Liste<String> liste1 = new Liste<String>();
		int count = 1;
		for (String mot : mots) {
			System.out.print("Ajout de " + mot);
			if ((count % 2) == 0) {
				liste1.add(mot);
				System.out.println(" à la fin");
			} else {
				liste1.insert(mot);
				System.out.println(" au début");
			}
			count++;
		}

		System.out.println("Liste = " + liste1);

		Liste<String> liste2 = new Liste<String>(liste1);

		System.out.print("Comparaison de " + liste1 + " et " + liste2 + " : ");
		System.out.println(liste1.equals(liste2) ? "Ok" : "Ko");

		System.out.print("Comparaison de " + liste1 + " et " + mots + " : ");
		System.out.println(liste1.equals(mots) ? "Ok" : "Ko");

		for (int i = 0; i < mots.length; i++) {
			liste1.remove(mots[i]);
			System.out.println("Liste - " + mots[i] + " = " + liste1);
		}

		liste2.clear();
		System.out.println("Liste2 après effacement : " + liste2);

		liste1.insert(mots[0], 0);
		liste1.insert(mots[6], 1);
		liste1.insert(mots[0], -1);
		liste1.insert(mots[0], 3);
		liste1.insert(mots[1], 1);
		liste1.insert(mots[4], 2);
		liste1.insert(mots[2], 2);
		liste1.insert(mots[5], 4);
		System.out.println("Liste1 après insertion indexée : " + liste1);

		// --------------------------------------------------------------------
		// CollectionListe<String>
		// --------------------------------------------------------------------

		CollectionListe<String> colListe1 = new CollectionListe<String>();
		ArrayList<String> vector1 = new ArrayList<String>();
		for (String mot : mots) {
			colListe1.add(mot);
			vector1.add(mot);
		}

		System.out.println("Collection Liste    : " + colListe1 + ", hash = " + colListe1.hashCode());
		System.out.println("Collection standard : " + vector1 + ", hash = " + vector1.hashCode());

		System.out.print("La Collection Liste est ");
		if (colListe1.equals(vector1)) {
			System.out.print("égale au");
		} else {
			System.out.print("différente du");
		}
		System.out.println(" ArrayList en terme de contenu");

		vector1.remove("mot7");

		System.out.println("Collection Liste    : " + colListe1 + ", hash = " + colListe1.hashCode());
		System.out.println("Collection standard : " + vector1 + ", hash = " + vector1.hashCode());

		System.out.print("La Collection Liste est ");
		if (colListe1.equals(vector1)) {
			System.out.print("égale au");
		} else {
			System.out.print("différente du");
		}
		System.out.println(" ArrayList en terme de contenu");

		CollectionListe<String> coListe2 = new CollectionListe<String>(colListe1);

		System.out.println("Collection Liste 1 : " + colListe1 + ", hash = " + colListe1.hashCode());
		System.out.println("Collection Liste 2 : " + coListe2 + ", hash = " + coListe2.hashCode());

		System.out.print("La Collection Liste est ");
		if (colListe1.equals(coListe2)) {
			System.out.print("égale à");
		} else {
			System.out.print("différente de");
		}
		System.out.println(" l'autre Collection Liste en terme de contenu");
	}

}
