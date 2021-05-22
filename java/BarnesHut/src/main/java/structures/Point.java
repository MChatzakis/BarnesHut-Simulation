/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package structures;

import java.text.DecimalFormat;
import lombok.Data;

/**
 *
 * @author manos
 */
@Data
public class Point {

    private double x;
    private double y;

    public Point(double x, double y) {
        this.x = x;
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
