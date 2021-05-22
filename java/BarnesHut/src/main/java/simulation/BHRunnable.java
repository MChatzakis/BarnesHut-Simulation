/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package simulation;

import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import lombok.Data;
import structures.Entity;

/**
 *
 * @author manos
 */
@Data
public class BHRunnable implements Runnable {

    private ArrayList<Entity> entities;

    private int iters;
    private int dt;
    private int from;
    private int to;
    private double dims;

    private static Object o = new Object();

    private static int workDone = 0;
    private static int totalThreads = 0;

    public BHRunnable(ArrayList<Entity> entities, int from, int to, int iters, int dt, double dims) {
        this.entities = entities;
        this.from = from;
        this.to = to;
        this.iters = iters;
        this.dt = dt;
        this.dims = dims;
        totalThreads++;
    }

    @Override
    public void run() {
        //System.out.println("Thread " + Thread.currentThread().getName() + "  Started Running!");
        for (int i = 0; i < iters; i++) {

            waitForAll();
        }
        //System.out.println("Thread " + Thread.currentThread().getName() + "  Got its work done!");

    }

    void waitForAll() {
        synchronized (o) {
            System.out.println("Thread " + Thread.currentThread().getName() + " reached the barrier");
            workDone++;
            if (workDone < totalThreads) {
                try {
                    o.wait();
                } catch (InterruptedException ex) {
                    Logger.getLogger(BHRunnable.class.getName()).log(Level.SEVERE, null, ex);
                }
            } else {
                o.notifyAll();
                workDone = 0;
            }
            System.out.println("Thread " + Thread.currentThread().getName() + " left the barrier");

        }
    }

}
