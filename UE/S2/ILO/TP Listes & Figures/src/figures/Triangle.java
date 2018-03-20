package figures;

import points.Point2D;
import points.Vecteur2D;

/**
 * Classe triangle héritière de la classe abstraite Figure le triangle est
 * composé de trois points doit donc implémenter les méthodes abstraites
 * suivantes
 * @see AbstractFigure#deplace
 * @see AbstractFigure#contient
 * @see AbstractFigure#getCentre
 * @see AbstractFigure#aire
 */
public class Triangle extends AbstractFigure
{
	/**
	 * tableau de 3 points
	 */
	protected Point2D[] points = new Point2D[3];

	// Constructeurs --------------------------------------------------------
	/**
	 * Constructeur par défaut : construit un triangle isocèle de 1 de base
	 * et de 1 de haut à partir de l'origine
	 */
	public Triangle()
	{
		points[0] = new Point2D(0.0, 0.0);
		points[1] = new Point2D(1.0, 0.0);
		points[2] = new Point2D(0.5, 1.0);
	}

	/**
	 * Constructeur valué : construit un triangle à partir de 3 points
	 * @param p1 premier point
	 * @param p2 second point
	 * @param p3 troisième point
	 */
	public Triangle(Point2D p1, Point2D p2, Point2D p3)
	{
		points[0] = new Point2D(p1);
		points[1] = new Point2D(p2);
		points[2] = new Point2D(p3);
	}

	/**
	 * Constructeur de copie.
	 * @param t le triangle à copier.
	 */
	public Triangle(Triangle t)
	{
		this(t.points[0], t.points[1], t.points[2]);
	}

	// Accessseurs ----------------------------------------------------------
	/**
	 * Accesseur en lecture pour le n<sup><font size=\"-2\">ième</font></sup>
	 * point (avec n dans [0..2])
	 * @param n l'indice du point recherché
	 * @return le n<sup><font size=\"-2\">ième</font></sup> point du triangle
	 */
	public Point2D getPoint(int n)
	{
		if ((n > (points.length - 1)) || (n < 0))
		{
			System.err.println("Triangle getPoint index invalide");
			return null;
		}
		else
		{
			return points[n];
		}
	}

	// Implémentation de Figure ---------------------------------------------
	/**
	 * Implementation Figure,
	 * Déplacement du triangle
	 * @param dx déplacement suivant x
	 * @param dy déplacement suivant y
	 * @return une référence vers la figure déplacée
	 */
	@Override
	public Figure deplace(double dx, double dy)
	{
		for (Point2D p : points)
		{
			p.deplace(dx, dy);
		}
		return this;
	}

	/**
	 * Implementation Figure,
	 * Affichage contenu
	 * @return une chaine représentant l'objet (les trois points)
	 */
	@Override
	public String toString()
	{
		StringBuilder result = new StringBuilder(super.toString());

		for (int i = 0; i < points.length; i++)
		{
			result.append(points[i].toString());

			if (i < (points.length - 1))
			{
				result.append(", ");
			}
		}

		return result.toString();
	}

	/**
	 * Test de contenu : teste si le point passé en argument est contenu à
	 * l'intérieur du triangle.
	 * Pour savoir si un point est contenu dans un polygone convexe
	 * il suffit d'effectuer le produit vectoriel des vecteurs
	 * reliant ce point avec deux points consécutifs le long du
	 * polygone, et ceci le long de chaque paire de points dans le
	 * polygone.
	 * Si on observe un changement de signe du produit vectoriel entre
	 * deux paires de vecteurs cela signifie que le point se trouve à
	 * l'extérieur du polygone.
	 * Contre-exemple : lorsqu'un point se trouve à l'intérieur d'un
	 * polygone convexe la suite des produits vectoriels des paires de
	 * vecteurs ne change jamais de signe !
	 * @param p point à tester
	 * @return une valeur booléenne indiquant si le point est contenu ou pas
	 * à l'intérieur du triangle
	 */
	@Override
	public boolean contient(Point2D p)
	{

		// Résultat initial
		boolean result = true;

		// Vecteurs initiaux
		Vecteur2D v1 = new Vecteur2D(p, points[0]);
		Vecteur2D v2 = new Vecteur2D(p, points[1]);

		// premier produit vectoriel
		double crossp = v1.crossProductN(v2);

		// signe produit vectoriel initial
		double signInit = crossp >= 0 ? 1 : -1;

		// produits vectoriels suivants
		double sign;

		// parcours des points du polygone à la recherche d'un changement
		// de signe du produit vectoriel
		for (int i = 1; i < points.length; i++)
		{
			v1 = v2;
			v2 = new Vecteur2D(p, points[(i + 1) % points.length]);

			crossp = v1.crossProductN(v2);
			sign = crossp >= 0 ? 1 : -1;

			if (sign != signInit)
			{
				result = false;
				break;
			}
		}

		return result;
	}

	/**
	 * Accesseur en lecture du centre de masse du triangle ( = barycentre)
	 * @return renvoie le barycentre du triangle
	 */
	@Override
	public Point2D getCentre()
	{
		double sx = 0.0;
		double sy = 0.0;
		// somme des coordonnées des points
		for (int i = 0; i < points.length; i++)
		{
			sx += points[i].getX();
			sy += points[i].getY();
		}
		// renvoi de la moyenne de chaque coordonnée
		return new Point2D(sx / points.length, sy / points.length);
	}

	/**
	 * Calcul de l'aire d'un triangle
	 * @return l'aire couverte par le triangle
	 */
	@Override
	public double aire()
	{
		// pour calculer l'aire d'un polygone convexe du plan XY, on utilise
		// une nouvelle fois les propriétés du produit vectoriel.
		// La norme du produit vectoriel représente le double de l'aire
		// couverte par les deux vecteurs dont on calcule ce produit.
		// il suffit donc de faire cette somme sur tous les triangle qui
		// composent le polygone en formant des vecteurs constitués par des
		// couples de points consécutifs le long du polygone.
		// Bon tout ca c'est bien mais pour un triangle c'est plus simple :

		Vecteur2D v1 = new Vecteur2D(points[0], points[1]);
		Vecteur2D v2 = new Vecteur2D(points[0], points[2]);

		return (Math.abs(v1.crossProductN(v2)) / 2.0);
	}

	/**
	 * Comparaison de deux triangles. On considère que deux triangles sont
	 * identiques s'ils contiennent les mêmes points (pas forcément dans
	 * le même ordre)
	 * @see Figure#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Figure figure)
	{
		if (getClass().equals(figure.getClass()))
		{
			Triangle other = (Triangle) figure;
			for (int i = 0; i < points.length; i++)
			{
				boolean found = false;
				for (int j = 0; j < points.length; j++)
				{
					if (points[i].equals(other.points[j]))
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
}
