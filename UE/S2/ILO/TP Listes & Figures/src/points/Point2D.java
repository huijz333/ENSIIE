package points;

/**
 * Classe définissant un point du plan 2D
 * @author David Roussel
 */
public class Point2D
{
	// attributs d'instance --------------------------------------------------
	/**
	 * l'abcisse du point
	 */
	protected double x;
	/**
	 * l'ordonnée du point
	 */
	protected double y;

	// attributs de classe ---------------------------------------------------
	/**
	 * Compteur d'instances : le nombre de points actuellement instanciés
	 */
	protected static int nbPoints = 0;

	/**
	 * Constante servant à comparer deux points entre eux (à {@value} près). On
	 * comparera alors la distance entre deux points.
	 * @see #distance(Point2D)
	 * @see #distance(Point2D, Point2D)
	 */
	protected static final double epsilon = 1e-2;

	/*
	 * Constructeurs
	 */
	/**
	 * Constructeur par défaut. Initialise un point à l'origine du repère [0.0,
	 * 0.0]
	 */
	public Point2D()
	{
		// utilisation du constructeur valué dans le constructeur par défaut
		// Attention si un constructeur est utilisé dans un autre constructeur
		// il doit être la PREMIERE instruction de ce constructeur
		// (Obligatoirement)
		this(0.0, 0.0);
	}

	/**
	 * Constructeur valué
	 * 
	 * @param x
	 *            l'abcisse du point à créer
	 * @param y
	 *            l'ordonnée du point à créer
	 */
	public Point2D(double x, double y) {
		this.x = x;
		this.y = y;
		nbPoints++;
	}

	/**
	 * Constructeur de copie
	 * 
	 * @param p
	 *            le point dont il faut copier les coordonnées Il s'agit ici d'une
	 *            copie profonde de manière à créer une nouvelle instance possédant
	 *            les même caractéristiques que celle dont on copie les coordonnées.
	 */
	public Point2D(Point2D p) {
		// utilisation du constructeur valué dans le constructeur par défaut
		this(p.x, p.y);
	}

	/**
	 * Nettoyeur avant destruction Permet de décrémenter le compteur d'instances
	 */
	@Override
	protected void finalize() {
		nbPoints--;
	}

	/*
	 * Accesseurs
	 */
	/**
	 * Accesseur en lecture de l'abcisse
	 * 
	 * @return l'abcisse du point.
	 */
	public double getX() {
		return x;
	}

	/**
	 * Accesseur en lecture de l'ordonnée
	 * 
	 * @return l'ordonnée du point.
	 */
	public double getY() {
		return y;
	}

	/**
	 * Accesseur en écriture de l'abcisse
	 * 
	 * @param val
	 *            valeur à donner à l'abcisse
	 */
	public void setX(double val) {
		x = val;
	}

	/**
	 * Accesseur en écriture de l'ordonnée
	 * 
	 * @param val
	 *            valeur à donner à l'ordonnée
	 */
	public void setY(double val) {
		y = val;
	}

	/**
	 * Accesseur en lecture d'epsilon
	 * 
	 * @return la valeur d'epsilon choisie pour comparer deux grandeurs à epsilon
	 *         près.
	 * @note Dans la mesure où epsilon est une constante qui ne peut pas changer de
	 *       valeur, il est tout à fait concevable de la rendre publique ce qui
	 *       éviterait cet accesseur
	 */
	public static double getEpsilon() {
		return epsilon;
	}

	/**
	 * Accesseur en lecture du nombre de points actuellement instanciés
	 * 
	 * @return le nombre de points actuellement instanciés
	 */
	public static int getNbPoints() {
		return nbPoints;
	}

	/*
	 * Affichage contenu
	 */
	// toString est une méthode classique en Java, elle est présente
	// dans les objets de type Object, on pourra donc ainsi l'utiliser
	// dans une éventuelle Liste de points.
	/**
	 * Méthode nécessaire pour l'affichage qui permet de placer un point dans un
	 * {@link java.io.PrintStream#println()} comme {@link System#out}.
	 * 
	 * @return une chaîne de caractères représentant un point.
	 */
	@Override
	public String toString() {
		return new String("x = " + x + " y = " + y);
	}

	/**
	 * Opérations sur un point
	 * 
	 * @param dx
	 *            le déplacement en x
	 * @param dy
	 *            le déplacement en y
	 * @return renvoie la référence vers l'instance courante (this) de manière à
	 *         pouvoir enchainer les traitements du style :
	 *         unObjet.uneMéthode(monPoint.deplace(dx,dy))
	 */
	public Point2D deplace(double dx, double dy) {
		x += dx;
		y += dy;
		return this;
	}

	/*
	 * Méthodes de classe : opérations sur les points
	 */
	/**
	 * Calcul de l'écart en abcsisse entre deux points. Cet écart ne concerne pas
	 * plus le premier que le second point c'est pourquoi on en fait une méthode de
	 * classe.
	 * 
	 * @param p1
	 *            le premier point
	 * @param p2
	 *            le second point
	 * @return l'écart en x entre les deux points
	 */
	protected static double dx(Point2D p1, Point2D p2) {
		return (p2.x - p1.x);
	}

	/**
	 * Calcul de l'écart en ordonnée entre deux points. Cet écart ne concerne pas
	 * plus le premier que le second point c'est pourquoi on en fait une méthode de
	 * classe.
	 * 
	 * @param p1
	 *            le premier point
	 * @param p2
	 *            le second point
	 * @return l'écart en y entre les deux points
	 */
	protected static double dy(Point2D p1, Point2D p2) {
		return (p2.y - p1.y);
	}

	/**
	 * Calcul de la distance 2D entre deux points. Cette distance ne concerne pas
	 * plus un point que l'autre c'est pourquoi on en fait une méthode de classe.
	 * Cette méthode utilise les méthodes {@link #dx(Point2D, Point2D)} et
	 * {@link #dy(Point2D, Point2D)} pour calculer la distance entre les points.
	 * 
	 * @param p1
	 *            le premier point
	 * @param p2
	 *            le seconde point
	 * @return la distance entre les points p1 et p2
	 * @see #dx(Point2D, Point2D)
	 * @see #dy(Point2D, Point2D)
	 */
	public static double distance(Point2D p1, Point2D p2) {
		// on remarquera que là aussi on
		// utilise des méthodes statiques
		// de l'objet Math : sqrt ou hypot

		double dx = dx(p1, p2);
		double dy = dy(p1, p2);
		return (Math.sqrt(dx * dx + dy * dy));
		// return (Math.hypot(dx, dy));
	}

	/**
	 * Calcul de distance 2D par rapport au point courant
	 * 
	 * @param p
	 *            l'autre point dont on veut calculer la distance
	 * @return la distance entre le point courant et le point p
	 * @see #distance(Point2D, Point2D)
	 */
	public double distance(Point2D p) {
		return distance(this, p);
	}

	/**
	 * Test d'égalité entre deux points 2D. Deux points sont considérés comme
	 * identiques si leur distance est inférieure à {@link #epsilon}. Cette méthode
	 * n'est utilisée que dans {@link #equals(Object)} donc elle n'est pas publique.
	 * 
	 * @param p
	 *            le point dont on veut tester l'égalité par rapport au point
	 *            courant
	 * @return true si les points sont plus proches que {@link #epsilon}, false
	 *         sinon.
	 */
	protected boolean equals(Point2D p) {
		// version distance
		return (distance(p) < epsilon);
	}

	/**
	 * Test d'égalité générique (hérité de la classe Object)
	 * 
	 * @param o
	 *            le point à tester (si c'est bien un point)
	 * @return true si les points sont plus proches que {@link #epsilon}, false
	 *         sinon ou bien si l'argument n'est pas un point. Il est important
	 *         d'implémenter cette version de la comparaison car lorsque de tels
	 *         points seront contenus dans des conteneurs génériques comme des
	 *         {@link java.util.Vector} ou des {@link listes.Liste} seule cette
	 *         comparaison pourra être utilisée.
	 * @note il est possible que l'on ne puisse pas faire ceci dans le premier TD
	 *       car on aura pas encore vu l'introspection
	 */
	@Override
	public boolean equals(Object o) {
		if (o == null) {
			return false;
		}
		if (o == this) {
			return true;
		}
		// comparaison laxiste (les points 2D et leurs héritiers)
		// if (this.getClass().isInstance(o))
		// comparaison stricte (uniquement les Points 2D)
		if (this.getClass().equals(o.getClass())) {
			return equals((Point2D) o);
		} else {
			return false;
		}
	}
}
