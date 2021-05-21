/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package utilCommons;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import structures.Entity;
import structures.Point;

/**
 *
 * @author manos
 */
public class Utils {

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
            Logger.getLogger(Utils.class.getName()).log(Level.SEVERE, null, ex);
        }

        return R;
    }

    public static void printEntities(ArrayList<Entity> entities) {
        for (Entity e : entities) {
            System.out.println(e);
        }
    }

}
