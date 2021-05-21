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
public class Entity {

    private double Vy;
    private double Vx;
    private double mass;
    private double SFx;
    private double SFy;
    private String name;
    private Point point;

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
        return this.name + ": " + point.toString() + " " + Vx + " " + Vy + " " + mass;
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

    public static double F(Entity e1, Entity e2) {
        double G = 6.67 / Math.pow(10, 11); //6.67 × 10−11
        double dist = distance(e1, e2);
        double m1 = e1.getMass();
        double m2 = e2.getMass();

        return G * (m1 * m2) / Math.pow(dist, 2);
    }

    public static double Fx(Entity e1, Entity e2, Double f) {
        double x1 = e1.getPoint().getX(), x2 = e2.getPoint().getX();
        double r = distance(e1, e2);

        return f * (x2 - x1) / r;
    }

    public static double Fy(Entity e1, Entity e2, Double f) {
        double y1 = e1.getPoint().getY(), y2 = e2.getPoint().getY();
        double r = distance(e1, e2);

        return f * (y2 - y1) / r;
    }
    
    
}
