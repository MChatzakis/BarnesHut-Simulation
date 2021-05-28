package structures;

import java.text.DecimalFormat;

public class Point {

  private double x;
  private double y;

  public Point(double x, double y) {
    this.x = x;
    this.y = y;
  }

  public double getX() {
    return x;
  }

  public double getY() {
    return y;
  }

  public void setX(double x) {
    this.x = x;
  }

  public void setY(double y) {
    this.y = y;
  }

  public String toString() {
    DecimalFormat format = new DecimalFormat();
    format.setMaximumFractionDigits(6);
    String xAp = format.format(x);
    String yAp = format.format(y);
    return "(" + xAp + "," + yAp + ")";
  }
}
