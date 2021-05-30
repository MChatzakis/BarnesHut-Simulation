package structures;

public class Region {

  private double dimension;
  private Point center;

  public Region(Point center, double dimension) {
    this.center = center;
    this.dimension = dimension;
  }

  public String toString() {
    double yEnd = center.getY() + dimension, yStart = center.getY() - dimension;
    double xEnd = center.getX() + dimension, xStart = center.getX() - dimension;

    return "x: [" + xStart + "," + xEnd + "] y: " + yStart + "," + yEnd + "]";
  }

  public boolean containsPoint(Point _center) {
    double yEnd = center.getY() + dimension, yStart = center.getY() - dimension;
    double xEnd = center.getX() + dimension, xStart = center.getX() - dimension;

    double currY = _center.getY();
    double currX = _center.getX();

    return ((currY <= yEnd && currY >= yStart) && (currX <= xEnd && currX >= xStart));
  }

  public boolean containsPoint(Point _center, int quadNo) {
    double yEnd = center.getY() + dimension, yStart = center.getY() - dimension;
    double xEnd = center.getX() + dimension, xStart = center.getX() - dimension;

    double currY = _center.getY();
    double currX = _center.getX();

    switch (quadNo) {
      case 1:
        return (currY <= yEnd && currY >= yStart) && (currX <= xEnd && currX >= xStart);
      case 2:
        return (currY <= yEnd && currY >= yStart) && (currX < xEnd && currX >= xStart);
      case 3:
        return (currY < yEnd && currY >= yStart) && (currX < xEnd && currX >= xStart);
      case 4:
        return (currY < yEnd && currY >= yStart) && (currX <= xEnd && currX > xStart);
      default:
        assert (false);
    }

    assert (false);
    return false;
  }

  public void setDimension(double dimension) {
    this.dimension = dimension;
  }

  public double getDimension() {
    return dimension;
  }

  public Point getCenter() {
    return center;
  }

  public void setCenter(Point center) {
    this.center = center;
  }
}
