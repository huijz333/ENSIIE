package figures;

import points.Point2D;

public class Cercle implements Figure {
	private Point2D center;
	private double radius;

	public Cercle() {
		this.center = new Point2D();
	}

	public Cercle(Point2D center) {
		this.center = new Point2D(center);
	}

	public Cercle(Point2D center, double radius) {
		this.center = new Point2D(center);
		this.radius = radius;
	}

	public Cercle(Cercle circle) {
		this(circle.center, circle.radius);
	}

	public final double getRadius() {
		return (this.radius);
	}

	public final void setRadius(double radius) {
		this.radius = radius;
	}

	@Override
	public String getNom() {
		return ("Cercle");
	}

	@Override
	public Figure deplace(double dx, double dy) {
		this.center.deplace(dx, dy);
		return (this);
	}

	@Override
	public boolean contient(Point2D p) {
		return (p.distance(this.center) < this.radius);
	}

	@Override
	public Point2D getCentre() {
		return (this.center);
	}

	@Override
	public double aire() {
		return (2.0f * Math.PI * this.radius);
	}

}
