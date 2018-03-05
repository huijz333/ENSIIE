package points;

/**
 * Classe définissant un vecteur du plan
 * @author davidroussel
 */
public class Vecteur2D extends Point2D
{

	/**
	 * Constructeur par défaut d'un vecteur 2D : construit un vecteur nul
	 */
	public Vecteur2D()
	{
		super();
	}

	/**
	 * Constructeur valué d'un vecteur 2D à partir d'un point2D : construit
	 * le vecteur reliant l'origine à ce point
	 * @param pt le point fournissant les coordonnées du vecteur
	 */
	public Vecteur2D(Point2D pt)
	{
		super(pt);
	}

	/**
	 * Constructeur valué d'un vecteur 2D à partir de coordonnées brutes
	 * @param x l'ordonnée du vecteur
	 * @param y l'abcisse du vecteur
	 */
	public Vecteur2D(double x, double y)
	{
		super(x, y);
	}

	/**
	 * Constructeur valué à partir de deux points : construit le vecteur reliant
	 * p1 à p2
	 * @param p1 le premier point du vecteur
	 * @param p2 le second point du vecteur
	 */
	public Vecteur2D(Point2D p1, Point2D p2)
	{
		super(p2.x - p1.x, p2.y - p1.y);
	}

	/**
	 * Calcul du produit scalaire avec un autre vecteur
	 * @param v l'autre vecteur avec lequel calculer le produit scalaire
	 * @return le produit scalaire du vecteur courant avec l'autre vecteur
	 */
	public double dotProduct(Vecteur2D v)
	{
		return (x * v.x) + (y * v.y);
	}

	/**
	 * Calcul de la norme du produit vectoriel avec un autre vecteur
	 * @param v l'autre vecteur avec lequel calculer le produit scalaire
	 * @return le produit scalaire du vecteur courant avec l'autre vecteur
	 */
	public double crossProductN(Vecteur2D v)
	{
		return (x * v.y) - (y * v.x);
	}

	/**
	 * Norme du vecteur
	 * @return la norme du vecteur
	 */
	public double norme()
	{
		return Math.sqrt(dotProduct(this));
	}

	/**
	 * Normalisation d'un vecteur
	 * @return renvoie le vecteur unitaire correspondant au vecteur
	 */
	public Vecteur2D normalize()
	{
		double norme = norme();
		return new Vecteur2D(x / norme, y / norme);
	}
}
