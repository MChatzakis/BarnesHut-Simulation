/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package structures;

import lombok.Data;

/**
 *
 * @author manos
 */
@Data
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

        return (currY <= yEnd && currY >= yStart) && (currX <= xEnd && currX >= xStart);
    }
}
