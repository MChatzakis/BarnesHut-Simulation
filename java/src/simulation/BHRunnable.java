package simulation;

import java.util.ArrayList;
import java.util.concurrent.CyclicBarrier;
import java.util.logging.Level;
import java.util.logging.Logger;

import structures.BHTree;
import structures.Entity;

public class BHRunnable implements Runnable {

  private ArrayList<Entity> entities;

  private int iters;
  private int dt;
  private int from;
  private int to;
  private double dims;

  private static Object o = new Object();
  private final CyclicBarrier barrier;

  private static int workDone = 0;
  private static int totalThreads = 0;

  public BHRunnable(ArrayList<Entity> entities, int from, int to, int iters, int dt, double dims,
      CyclicBarrier barrier) {
    this.entities = entities;
    this.from = from;
    this.to = to;
    this.iters = iters;
    this.dt = dt;
    this.dims = dims;
    this.barrier = barrier;
    totalThreads++;
  }

  @Override
  public void run() {
    for (int k = 0; k < iters; k++) {
      try {

        BHTree bh = BHUtils.createBHTree(entities, dims);
        barrier.await();

        for (int i = from; i < to; i++) {
          BHUtils.netForce(entities.get(i), bh);
        }
        barrier.await();
        
        for (int i = from; i < to; i++) {
          BHUtils.newPosition(entities.get(i), dt, dims);
        }
        barrier.await();

      } catch (Exception e) {
        e.printStackTrace();
      }
    }
  }

  void waitForAll() {
    synchronized (o) {
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
    }
  }
}
