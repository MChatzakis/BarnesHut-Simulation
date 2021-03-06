package structures;

import java.text.DecimalFormat;

public class Entity {

  private double Vy;
  private double Vx;
  private double mass;
  private double SFx;
  private double SFy;
  private String name;
  private Point point;
  private Region currentRegion;

  public Entity(Point point, String name, double Vx, double Vy, double mass) {
    this.point = point;
    this.name = name;
    this.Vx = Vx;
    this.Vy = Vy;
    this.mass = mass;

    SFx = 0;
    SFy = 0;
  }

  public void addToSFx(double DFx) {
    this.SFx += DFx;
  }

  public void addToSFy(double DFy) {
    this.SFy += DFy;
  }

  public String toString() {
    DecimalFormat format = new DecimalFormat();
    format.setMaximumFractionDigits(6);

    String VxAp = format.format(Vx);
    String VyAp = format.format(Vy);
    String massAp = format.format(mass);

    return (this.name + ": " + point.toString() + " " + VxAp + " " + VyAp + " " + massAp);
  }

  public Region getCurrentRegion() {
    return currentRegion;
  }

  public void setCurrentRegion(Region currentRegion) {
    this.currentRegion = currentRegion;
  }

  public static Entity massCenter(Entity e1, Entity e2) {
    double centerX, centerY, mass = e1.getMass() + e2.getMass();

    centerX = ((e1.getPoint().getX() * e1.getMass() + e2.getPoint().getX() * e2.getMass())) / mass;
    centerY = ((e1.getPoint().getY() * e1.getMass() + e2.getPoint().getY() * e2.getMass())) / mass;

    return new Entity(new Point(centerX, centerY), "MassCenter", 0, 0, mass);
  }

  public static double distance(Entity e1, Entity e2) {
    double x1 = e1.getPoint().getX();
    double x2 = e2.getPoint().getX();

    double y1 = e1.getPoint().getY();
    double y2 = e2.getPoint().getY();

    return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
  }

  public static double F(Entity e1, Entity e2, double dist) {
    double G = 6.67 / Math.pow(10, 11); // 6.67 × 10−11
    // double dist = distance(e1, e2);
    double m1 = e1.getMass();
    double m2 = e2.getMass();

    return G * (m1 * m2) / Math.pow(dist, 2);
  }

  public static double Fx(Entity e1, Entity e2, double f, double r) {
    double x1 = e1.getPoint().getX(), x2 = e2.getPoint().getX();
    // double r = distance(e1, e2);
    // double f = F(e1,e2,r);
    return f * (x2 - x1) / r;
  }

  public static double Fy(Entity e1, Entity e2, double f, double r) {
    double y1 = e1.getPoint().getY(), y2 = e2.getPoint().getY();
    // double r = distance(e1, e2);
    // double f = F(e1, e2, r);
    return f * (y2 - y1) / r;
  }

  public double getVy() {
    return Vy;
  }

  public void setVy(double Vy) {
    this.Vy = Vy;
  }

  public double getVx() {
    return Vx;
  }

  public void setVx(double Vx) {
    this.Vx = Vx;
  }

  public double getMass() {
    return mass;
  }

  public void setMass(double mass) {
    this.mass = mass;
  }

  public double getSFx() {
    return SFx;
  }

  public void setSFx(double SFx) {
    this.SFx = SFx;
  }

  public double getSFy() {
    return SFy;
  }

  public void setSFy(Double SFy) {
    this.SFy = SFy;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public Point getPoint() {
    return point;
  }

  public void setPoint(Point point) {
    this.point = point;
  }
}
