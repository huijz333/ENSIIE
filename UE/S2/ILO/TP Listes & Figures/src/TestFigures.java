import java.util.ArrayList;
import java.util.Collection;

import listes.CollectionListe;
import points.Point2D;
import figures.AbstractFigure;
import figures.Cercle;
import figures.Figure;
import figures.Polygone;
import figures.Rectangle;
import figures.Triangle;

/**
 * Class de test des Figures
 * 
 * @author davidroussel
 */
class TestFigures {
	/**
	 * Programme de test des figures
	 * 
	 * @param args
	 *            arguments (non utilisés)
	 */
	public static void main(String args[]) {
		Cercle cer, cer2;
		Rectangle rec, rec2;
		Triangle tri, tri2;

		// création d'un cercle
		Point2D centre = new Point2D(0, 0);
		cer = new Cercle(centre, 2);
		cer2 = new Cercle(cer);
		System.out.println(cer + " == " + cer2 + " ?  : " + cer.equals(cer2));

		// création d'un carré centré en 2.5, 1.5
		Point2D pmin = new Point2D(2, 1);
		Point2D pmax = new Point2D(3, 2);
		rec = new Rectangle(pmin, pmax);
		rec2 = new Rectangle(rec);
		System.out.println(rec + " == " + rec2 + " ?  : " + rec.equals(rec2));

		// création d'un triangle
		tri = new Triangle();
		tri2 = new Triangle(tri);
		System.out.println(tri + " == " + tri2 + " ?  : " + tri.equals(tri2));

		// création d'un polygone
		Point2D p0 = new Point2D(4, 1);
		Point2D p1 = new Point2D(4, 1);
		Point2D p2 = new Point2D(5, 3);
		Point2D p3 = new Point2D(4, 5);
		Point2D p4 = new Point2D(2, 5);
		Polygone poly = new Polygone(p0, p1);
		poly.ajouter(p2);
		poly.ajouter(p3);
		poly.ajouter(p4);
		Polygone poly2 = new Polygone(poly);
		System.out.println(poly + " == " + poly2 + " : " + (poly.equals(poly2) ? "Ok" : "Ko"));

		ArrayList<Point2D> vp = new ArrayList<Point2D>();
		vp.add(p0);
		vp.add(p1);
		vp.add(p2);
		vp.add(p3);
		vp.add(p4);
		Polygone poly3 = new Polygone(vp);
		System.out.println(poly + " == " + poly3 + " : " + (poly.equals(poly3) ? "Ok" : "Ko"));

		// création d'une ligne
		Point2D pl0 = new Point2D(0, 0);
		Point2D pl1 = new Point2D(1, 0);
		Polygone ligne = new Polygone(pl0, pl1);
		Polygone ligne2 = new Polygone(ligne);
		System.out.println(ligne + " == " + ligne2 + " ?  : " + ligne.equals(ligne2));

		// test des différentes méthodes communes au figures
		Collection<Figure> figures = new CollectionListe<Figure>();
		figures.add(cer);
		figures.add(rec);
		figures.add(tri);
		figures.add(poly);
		System.out.println("Ma " + figures);

		// affichage
		for (Figure f : figures) {
			System.out.println(f);
		}

		// déplacement
		for (Figure f : figures) {
			f.deplace(1, 1);
		}

		// nouvel affichage après déplacement
		for (Figure f : figures) {
			System.out.println(f);
		}

		// test de contenu
		Point2D pcont = new Point2D(2, 2);
		pcont.deplace(-0.5, -0.75);
		System.out.println("Test de contenance du point " + pcont);
		System.out.println("Le point " + pcont + " est : ");
		for (Figure f : figures) {
			afficheContenance(f, pcont);
		}

		Point2D pcont2 = new Point2D(3, 3);
		System.out.println("Test de contenance du point " + pcont2);
		System.out.println("Le point " + pcont2 + " est : ");
		for (Figure f : figures) {
			afficheContenance(f, pcont2);
		}

		Point2D pcont3 = new Point2D(0.5, 0);
		System.out.println("Test de contenance du point " + pcont3);
		System.out.println("Le point " + pcont3 + " est : ");
		for (Figure f : figures) {
			afficheContenance(f, pcont3);
		}

		// distance aux centres
		Collection<Figure> figures2 = new CollectionListe<Figure>(figures);
		for (Figure f1 : figures) {
			for (Figure f2 : figures2) {
				afficheDistanceCentres(f1, f2);
			}
		}

		// aires des figures
		figures.add(ligne);
		for (Figure f : figures) {
			afficheAire(f);
		}

		// ajout d'une deuxième occurrence de polygone dans la collection
		figures.add(poly);
		System.out.println("Ma " + figures + " avant retrait de " + poly);

		// retrait de toutes les occurences de poly de la collection
		int count = 0;
		while (figures.contains(poly)) {
			if (figures.remove(poly)) {
				count++;
			}
		}

		// affichage de la collection après retrait des poly
		System.out.print("Ma " + figures + "après retrait : ");
		System.out.println(" " + count + " occurrences supprimées");

	}

	public static void afficheContenance(Figure f, Point2D p) {
		if (f.contient(p)) {
			System.out.println("	dans le " + f.getNom());
		} else {
			System.out.println("	en dehors du " + f.getNom());
		}
	}

	public static void afficheDistanceCentres(Figure f1, Figure f2) {
		System.out.println("Distance " + f1.getNom() + " -> " + f2.getNom() + " : " + Figure.distanceToCentre(f1, f2));
	}

	public static void afficheAire(Figure f) {
		System.out.println("Aire de " + f.getNom() + " : " + f.aire());
	}

}