package figures;

import java.util.Collection;

import listes.CollectionListe;
import points.Point2D;

public class Polygone implements Figure {

	private Collection<Point2D> points;

	public Polygone() {
		this.points = new CollectionListe<Point2D>();
	}

	public Polygone(Collection<Point2D> points) {
		this();
		for (Point2D p : points) {
			this.points.add(p);
		}
	}

	public Polygone(Point2D... points) {
		this();
		for (Point2D p : points) {
			this.points.add(p);
		}
	}

	public Polygone(Polygone polygone) {
		this();
		this.points.addAll(polygone.points);
	}

	@Override
	public String getNom() {
		return ("Polygone");
	}

	@Override
	public Figure deplace(double dx, double dy) {
		for (Point2D p : this.points) {
			p.deplace(dx, dy);
		}
		return (this);
	}

	@Override
	public boolean contient(Point2D point) {
		// throw new UnsupportedOperationException("Polygone.contient() non définit");

		for (Point2D p : this.points) {
			if (p.equals(point)) {
				return (true);
			}
		}
		return (false);
	}

	@Override
	public Point2D getCentre() {
		double cx = 0.0f;
		double cy = 0.0f;
		for (Point2D p : this.points) {
			cx += p.getX();
			cy += p.getY();
		}

		float f = 1.0f / (float) this.points.size();
		return (new Point2D(cx * f, cy * f));
	}

	@Override
	public double aire() {
		// throw new UnsupportedOperationException("Polygone.contient() non définit");
		return (0.0);
	}

	public void ajouter(Point2D p2) {
		this.points.add(p2);
	}

}
