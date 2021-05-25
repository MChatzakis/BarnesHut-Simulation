package simulation;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import structures.*;

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

  public static void newPosition(Entity e, int dt) {
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

    e.setPoint(new Point(newX, newY));

    e.setSFx(0.0);
    e.setSFy(0.0);

    e.setVx(newVx);
    e.setVy(newVy);
    //entities.remove(e);
  }
}
