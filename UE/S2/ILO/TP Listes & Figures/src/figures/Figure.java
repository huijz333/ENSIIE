package figures;

import points.Point2D;

/**
 * Interface des figures
 * 
 * @author davidroussel
 */
public interface Figure extends Cloneable {
	/**
	 * Accesseur en lecture pour le nom de la figure
	 * 
	 * @return une chaine contenant le nom de la figure
	 */
	public abstract String getNom();

	/**
	 * Méthode abstraite Déplacement de la figure
	 * 
	 * @param dx
	 *            déplacement selon l'axe des X
	 * @param dy
	 *            déplacement selon l'axe des Y
	 * @return renvoie une référence vers la figure afin que l'on puisse déplacer
	 *         une figure en cascade : <code>f.deplace(dx,dy).deplace(dx,dy)</code>
	 * @see Point2D#deplace(double, double)
	 */
	public abstract Figure deplace(double dx, double dy);

	/**
	 * Affichage contenu
	 * 
	 * @return une chaine de caractère représentant la figure
	 */
	@Override
	public abstract String toString();

	/**
	 * Test de contenu d'un point dans la figure teste si le point passé en argument
	 * est contenu à l'intérieur de la figure
	 * 
	 * @param p
	 *            : point candidat à la contenance
	 * @return la contenance du point à l'intérieur de la figure
	 */
	public abstract boolean contient(Point2D p);

	/**
	 * Centre de la figure. renvoie le centre de la figure
	 * 
	 * @return renvoie le point2D central de la figure
	 */
	public abstract Point2D getCentre();

	/**
	 * Aire couverte par la figure
	 * 
	 * @return renvoie l'aire couverte par la figure
	 */
	public abstract double aire();

	/**
	 * Distance entre les centres de la figure courante et d'une figure passée en
	 * argument
	 * 
	 * @param f
	 *            figure avec laquelle on calcule la distance entre les centres
	 * @return la distance entre les points centraux des deux figures
	 * @see #getCentre()
	 * @see Point2D#distance(Point2D, Point2D)
	 */
	public default double distanceToCentreOf(Figure f) {
		// getCentre est une méthode abstraite mais rien ne nous empêche
		// de l'utiliser dans une autre méthode. Grâce au lien dynamique
		// TODO Remplacer par l'implémentation
		return this.getCentre().distance(f.getCentre());
	}

	/**
	 * Distance entre les centres de deux figures
	 * 
	 * @param f1
	 *            première figure
	 * @param f2
	 *            seconde figure
	 * @return la distance entre les points centraux des deux figures
	 * @see #getCentre()
	 * @see Point2D#distance(Point2D, Point2D)
	 */
	public static double distanceToCentre(Figure f1, Figure f2) {
		return (f1.getCentre().distance(f2.getCentre()));
	}

	/**
	 * Test d'égalité de la figure courante avec une autre figure. Cette méthode
	 * n'implémente que le test sur la nature des figures, le test sur le contenu
	 * doit être réimplémenté dans chaque sous classe, en utilisant cette méthode
	 * pour tester la nature des figures.
	 * 
	 * @param o
	 *            la figure dont il faut comparer le contenu.
	 * @return true si les deux figures sont de nature identique et qu'elles ont le
	 *         même contenu.
	 */
	@Override
	public abstract boolean equals(Object o);
}
