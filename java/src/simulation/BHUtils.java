package simulation;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

import structures.BHTree;
import structures.Entity;
import structures.Point;
import structures.Region;

public class BHUtils {

  public static double parseFile(String path, ArrayList<Entity> entities) {
    int N = 0;
    double R = 0;

    try {
      Scanner scan;
      File file = new File(path);
      try {
        scan = new Scanner(file);

        if (scan.hasNextInt()) {
          N = scan.nextInt();
        }

        if (scan.hasNextDouble()) {
          R = scan.nextDouble();
        }

        for (int i = 0; i < N; i++) {
          double X = 0, Y = 0, Vx = 0, Vy = 0, M = 0;
          String S = "";

          if (scan.hasNextDouble()) {
            X = scan.nextDouble();
          }

          if (scan.hasNextDouble()) {
            Y = scan.nextDouble();
          }

          if (scan.hasNextDouble()) {
            Vx = scan.nextDouble();
          }

          if (scan.hasNextDouble()) {
            Vy = scan.nextDouble();
          }

          if (scan.hasNextDouble()) {
            M = scan.nextDouble();
          }

          if (scan.hasNext()) {
            S = scan.next();
          }

          entities.add(new Entity(new Point(X, Y), S, Vx, Vy, M));
        }
      } catch (FileNotFoundException e1) {
        e1.printStackTrace();
      }
    } catch (Exception ex) {
      Logger.getLogger(BHUtils.class.getName()).log(Level.SEVERE, null, ex);
    }

    return R;
  }

  public static void printEntities(ArrayList<Entity> entities) {
    for (Entity e : entities) {
      System.out.println(e);
    }
  }

  public static BHTree createBHTree(ArrayList<Entity> entities, double dims) {
    BHTree bh = new BHTree(new Region(new Point(0, 0), dims));
    for (Entity e : entities) {
      bh.insertEntity(e);
    }
    return bh;
  }

  public static void newPosition(Entity e, int dt, double dims) {
    double SFy = e.getSFy();
    double SFx = e.getSFx();
    double m = e.getMass();

    double oldVx = e.getVx();
    double oldVy = e.getVy();
    double oldX = e.getPoint().getX();
    double oldY = e.getPoint().getY();

    double Ax = SFx / m;
    double Ay = SFy / m;

    double newVx = oldVx + Ax * dt;
    double newVy = oldVy + Ay * dt;

    double newX = oldX + newVx * dt;
    double newY = oldY + newVy * dt;

    /* Pathing the coordinates in case a body leaves the universe */
    if (newX > dims) {
      newX = dims;
    }

    if (newX < -dims) {
      newX = -dims;
    }

    if (newY > dims) {
      newY = dims;
    }

    if (newY < -dims) {
      newY = -dims;
    }

    e.setPoint(new Point(newX, newY));
    e.setSFx(0.0);
    e.setSFy(0.0);
    e.setVx(newVx);
    e.setVy(newVy);

  }

  public static void netForce(Entity e, BHTree bh) {
    if (e == null) {
      return;
    }

    if (bh.isLeaf()) {
      assert (e.equals(bh.getEntity()));
      return;
    }

    boolean pathFound = false;

    BHTree[] quads = { bh.getQuad1(), bh.getQuad2(), bh.getQuad3(), bh.getQuad4(), };
    BHTree quadToGo = null;

    for (int i = 0; i < 4; i++) {
      if (quads[i] != null) {
        Region reg = quads[i].getRegion();
        Entity curr = quads[i].getEntity();

        if (curr != null) {
          if (!pathFound && reg.containsPoint(e.getPoint())) {

            // System.out.println("Found the region that body " + e.getName() + " belongs.
            // Going to quad[ " + i + " ]");

            quadToGo = quads[i];
            pathFound = true;
          } else {

            // System.out.println("Calculating net force for body " + e.toString() + " by
            // body " + curr.toString());

            double r = Entity.distance(e, curr);
            double f = Entity.F(e, curr, r);
            double fx = Entity.Fx(e, curr, f, r);
            double fy = Entity.Fy(e, curr, f, r);

            e.addToSFx(fx);
            e.addToSFy(fy);
          }
        }
      }
    }

    assert (quadToGo != null);
    netForce(e, quadToGo);
  }

  public static void printEntities(ArrayList<Entity> entities, String filename) {
    try {
      FileWriter wr = new FileWriter(filename);

      for (Entity e : entities) {
        double X = e.getPoint().getX();
        double Y = e.getPoint().getY();
        double Vx = e.getVx();
        double Vy = e.getVy();
        double M = e.getMass();
        String S = e.getName();

        wr.write(X + " " + Y + " " + Vx + " " + Vy + " " + M + " " + S + "\n");
      }

      wr.close();
    } catch (IOException e) {
      System.out.println("An error occurred.");
      e.printStackTrace();
    }
  }

  public static void appendToFile(String filename, double time) {
    try {
      String timeToPrint;
      DecimalFormat numberFormat = new DecimalFormat("#.000");
      FileWriter fw = new FileWriter(filename, true);
      BufferedWriter bw = new BufferedWriter(fw);

      timeToPrint = numberFormat.format(time);
      // bw.write(time + "\n");
      bw.write(timeToPrint + "\n");
      
      bw.close();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
