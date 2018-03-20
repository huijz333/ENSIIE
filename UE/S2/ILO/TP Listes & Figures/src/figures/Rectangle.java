package figures;

import points.Point2D;

public class Rectangle implements Figure {

	private Point2D p1, p2;

	public Rectangle(Point2D p1, Point2D p2) {
		this.p1 = p1;
		this.p2 = p2;
	}

	public Rectangle(Rectangle rect) {
		this.p1 = new Point2D(rect.p1);
		this.p2 = new Point2D(rect.p2);
	}

	@Override
	public String getNom() {
		return ("Rectangle");
	}

	@Override
	public Figure deplace(double dx, double dy) {
		this.p1.deplace(dx, dy);
		this.p2.deplace(dx, dy);
		return (this);
	}

	@Override
	public boolean contient(Point2D p) {
		double mx = this.p1.getX() < this.p2.getX() ? this.p1.getX() : this.p2.getX();
		double my = this.p1.getY() < this.p2.getY() ? this.p1.getY() : this.p2.getY();
		double Mx = this.p1.getX() > this.p2.getX() ? this.p1.getX() : this.p2.getX();
		double My = this.p1.getX() > this.p2.getX() ? this.p1.getX() : this.p2.getX();
		double x = p.getX();
		double y = p.getY();
		return (x >= mx && x <= Mx && y >= my && y <= My);
	}

	@Override
	public Point2D getCentre() {
		double cx = (this.p1.getX() + this.p2.getX()) * 0.5;
		double cy = (this.p1.getY() + this.p2.getY()) * 0.5;
		return (new Point2D(cx, cy));
	}

	@Override
	public double aire() {
		double dx = this.p2.getX() - this.p1.getX();
		double dy = this.p2.getY() - this.p1.getY();
		double a = dx * dy;
		return (a < 0 ? -a : a);
	}

}
