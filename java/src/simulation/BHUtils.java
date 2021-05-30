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

  public static void netForce(Entity e, BHTree bh, boolean nothing) {
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

  public static void netForce(Entity e, BHTree bh) {
    if (bh == null) {
      return;
    }

    /*
     * Reaching a leaf means that the traverse has come to an end. It is clear that
     * this leaf can only be the leaf containing body e, else there has ocurred an
     * error through the iterative traverse of the tree
     */
    if (bh.isLeaf()) {
      Entity body = bh.getEntity();
      if (body != e && body != null) {
        double r = Entity.distance(e, body);
        double f = Entity.F(e, body, r);

        double fx = Entity.Fx(e, body, f, r);
        double fy = Entity.Fy(e, body, f, r);

        e.addToSFx(fx);
        e.addToSFy(fy);
      }

      return;
    }

    BHTree quads[] = { bh.getQuad1(), bh.getQuad2(), bh.getQuad3(), bh.getQuad4() };
    BHTree quadToGo = null;

    /*
     * Through this iteration we have two major aims: 1. Find the subquad that
     * entity e is located, to proceed there recursively 2. Calculate the force
     * acting on body e from all other subquads, with each subquad represented
     * totally as the mass center of all the bodies it contains.
     */
    for (int i = 0; i < 4; i++) {
      /* This operation is done only for quads that contain entities */
      if (quads[i] != null) {
        Entity cent = quads[i].getEntity();
        Region reg = quads[i].getRegion();

        if (cent == null) {
          continue;
        }

        /*
         * Warning: pathFound flag is a patch for the cases that bodies are on the
         * border of different subquads. e.g. If an entity is located at (0,0), we will
         * (by default) assign in to subquad 1, but containgPoint method will return
         * true for every subquad of this region. This problem is solved with the help
         * of this flag, which is set when we find the first matching region.
         */
        if (reg.containsPoint(e.getPoint(), i + 1)) {
          quadToGo = quads[i]; // saving the destination quad
        } else {
          /*
           * In this case, entity cent represents the mass center of the current adjacent
           * subquad
           */
          double s = 2 * quads[i].getRegion().getDimension();
          double r = Entity.distance(e, cent);

          if ((s / r) < 1.00 || quads[i].isLeaf()) // far away, or leaf
          {
            // cout << "Calculating net force for body " << e->getName() << " by body " <<
            // cent->getName() << " .Region: " << quads[i]->getRegion().toString() << "\n";

            double f = Entity.F(e, cent, r);
            double fx = Entity.Fx(e, cent, f, r);
            double fy = Entity.Fy(e, cent, f, r);

            /*
             * As SFx and SFy are only modifies by the thread assigned this chunk, there is
             * no data race
             */
            e.addToSFx(fx);
            e.addToSFy(fy);
          } else // close
          {
            BHTree children[] = { quads[i].getQuad1(), quads[i].getQuad2(), quads[i].getQuad3(), quads[i].getQuad4() };
            for (int k = 0; k < 4; k++) {
              // cout<< "Recursion for body" << quads[i]->getEntity()->toString()<<"\n";
              if (children[k] != null) { // just for optimization
                                         // cout << "Bodies found close to body " << e->getName() << ". Checking for
                                         // bodies at quad region " << children[k]->getRegion().toString() << "\n";
                netForce(e, children[k]);
              }
            }
          }
        }
      }
    }

    if (quadToGo != null) {
      // cout << "Found the region that body " << e->getName() << " belongs. Going to
      // quad on the region " << quadToGo->getRegion().toString() << "\n";
      netForce(e, quadToGo);
    }
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
