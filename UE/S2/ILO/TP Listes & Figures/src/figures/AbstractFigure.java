package figures;

import points.Point2D;

/**
 * Classe abstraite Figure Contient une données concrête : le nom de la figure (
 * {@link #nom} )
 * <ul>
 * <li>des méthodes d'instance</li>
 * <ul>
 * <li>concrètes
 * <ul>
 * <li>un constructeur avec un nom : {@link #AbstractFigure(String)}</li>
 * <li>un accesseur pour ce nom : {@link #getNom()}</li>
 * <li>la méthode toString pour afficher ce nom {@link #toString()}</li>
 * <li>{@link #distanceToCentreOf(Figure)}</li>
 * </ul>
 * <li>abstraites
 * <ul>
 * <li>{@link #deplace(double,double)}</li>
 * <li>{@link #contient(Point2D)}</li>
 * <li>{@link #getCentre()}</li>
 * <li>{@link #aire()}</li>
 * </ul>
 * </ul>
 * <li>des méthodes de classes</li>
 * <ul>
 * <li>concrètes</li>
 * <ul>
 * <li>{@link #distanceToCentre(Figure,Figure)}</li>
 * </ul>
 * </ul>
 * </ul>
 * 
 * @author David Roussel
 */
public abstract class AbstractFigure implements Figure {
	/**
	 * Nom de la figure
	 */
	protected String nom;

	/**
	 * Constructeur (protégé) par défaut. Affecte le nom de la classe comme nom de
	 * figure
	 */
	protected AbstractFigure() {
		nom = getClass().getSimpleName();
	}

	/**
	 * Constructeur (protégé) avec un nom on a fait exprès de ne pas mettre de
	 * constructeur sans arguments
	 * 
	 * @param unNom
	 *            Chaine de caractère pour initialiser le nom de la figure
	 */
	protected AbstractFigure(String unNom) {
		nom = unNom;
	}

	/**
	 * @return le nom
	 * @see figures.Figure#getNom()
	 */
	@Override
	public String getNom() {
		return nom;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see figures.Figure#deplace(double, double)
	 */
	@Override
	public abstract Figure deplace(double dx, double dy);

	/*
	 * (non-Javadoc)
	 * 
	 * @see figures.Figure#toString()
	 */
	@Override
	public String toString() {
		return (nom + " : ");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see figures.Figure#contient(points.Point2D)
	 */
	@Override
	public abstract boolean contient(Point2D p);

	/*
	 * (non-Javadoc)
	 * 
	 * @see figures.Figure#getCentre()
	 */
	@Override
	public abstract Point2D getCentre();

	/*
	 * (non-Javadoc)
	 * 
	 * @see figures.Figure#aire()
	 */
	@Override
	public abstract double aire();

	/**
	 * Comparaison de deux figures en termes de contenu
	 * 
	 * @return true si f est du même types que la figure courante et qu'elles ont un
	 *         contenu identique
	 */
	protected abstract boolean equals(Figure f);

	/**
	 * Comparaison de deux figures, on ne peut pas vérifier grand chose pour
	 * l'instant à part la classe et le nom
	 * 
	 * @note implémentation partielle qui ne vérifie que null/this/et l'égalité de
	 *       classe
	 * @see figures.Figure#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj) {
		if (obj == null || !(obj instanceof AbstractFigure)) {
			return (false);
		}
		if (obj == this) {
			return (true);
		}
		return false;
	}

	// /**
	// * Hashcode d'une figure (implémentation partielle basée sur le nom d'une
	// * figure) --> Non utilisé
	// * @see java.lang.Object#hashCode()
	// */
	// @Override
	// public int hashCode()
	// {
	// final int prime = 31;
	// int result = 1;
	// result = (prime * result) + ((nom == null) ? 0 : nom.hashCode());
	// return result;
	// }
}
