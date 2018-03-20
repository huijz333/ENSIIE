package figures;

import java.util.Collection;

import listes.CollectionListe;
import points.Point2D;

public class Groupe extends CollectionListe<Figure> implements Figure {

	public Groupe(Groupe groupe) {
		super(groupe);
	}

	public Groupe(Collection<Figure> collection) {
		super(collection);
	}

	@Override
	public String getNom() {
		return ("Groupe");
	}

	@Override
	public Figure deplace(double dx, double dy) {
		for (Figure f : this) {
			f.deplace(dx, dy);
		}
		return (this);
	}

	@Override
	public boolean contient(Point2D p) {
		for (Figure f : this) {
			if (f.contient(p)) {
				return (true);
			}
		}
		return (false);
	}

	/** barycentre des figures en fonction de leur aire */
	@Override
	public Point2D getCentre() {
		double cx = 0.0;
		double cy = 0.0;
		double norm = 0.0;
		for (Figure f : this) {
			double area = f.aire();
			cx += f.getCentre().getX() * area;
			cy += f.getCentre().getY() * area;
			norm += area;
		}
		double invNorm = 1.0 / norm;
		return (new Point2D(cx * invNorm, cy * invNorm));

	}

	/**
	 * N.B : cette implementation ne gere pas 2 figures qui se superposent : l'aire
	 * superposé sera compté 2 fois
	 */
	@Override
	public double aire() {
		double area = 0.0;
		for (Figure f : this) {
			area += f.aire();
		}
		return (area);

	}

}
