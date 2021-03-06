package simulation;

import java.util.ArrayList;
import java.util.concurrent.CyclicBarrier;
//import java.util.concurrent.ForkJoinPool;

import structures.BHTree;
import structures.Entity;

public class BarnesHutMain {

  public static void main(String[] args) throws InterruptedException {

    String filename = args[0];
    int iters = Integer.parseInt(args[1]);
    int threads = Integer.parseInt(args[2]);
    int dt = 1;

    System.out.println("Filename: " + filename);
    System.out.println("Iterations: " + iters);
    System.out.println("Threads: " + threads);

    ArrayList<Entity> entities = new ArrayList<>();
    double dims = BHUtils.parseFile(filename, entities);
    long start = System.currentTimeMillis();

    if (threads == 0) {
      BarnesHutSequential(entities, iters, dims, dt);
    } else {
      BarnesHutParallel(entities, iters, dims, threads, dt);
      //BarnesHutStream(entities, iters, dims, threads, dt);
      //BarnesHutParallelv2(entities, iters, dims, threads, dt);
    }

    long finish = System.currentTimeMillis();
    long timeElapsed = finish - start;

    BHUtils.printEntities(entities);

    System.out.println("Execution time: " + (double) timeElapsed * 0.001 + " seconds");
    BHUtils.printEntities(entities, "outputJAVA.txt");
    BHUtils.appendToFile("timesJAVA.txt", (double) timeElapsed * 0.001);
  }

  /*
   * public static void BarnesHutStream(ArrayList<Entity> entities, int iters,
   * double dims, int threadsNum, int dt) {
   * 
   * ForkJoinPool customThreadPool = new ForkJoinPool(threadsNum);
   * 
   * for (int i = 0; i < iters; i++) {
   * 
   * BHTree bh = BHUtils.createBHTree(entities, dims);
   * 
   * customThreadPool.submit(() -> entities.parallelStream().forEach(e -> {
   * BHUtils.netForce(e, bh); }));
   * 
   * customThreadPool.submit(() -> entities.parallelStream().forEach(e -> {
   * BHUtils.newPosition(e, dt, dims); }));
   * 
   * }
   * 
   * customThreadPool.shutdownNow();
   * 
   * }
   */

  public static void BarnesHutParallel(ArrayList<Entity> entities, int iters, double dims, int threadsNum, int dt)
      throws InterruptedException {
    Thread[] threads = new Thread[threadsNum];

    int sets = entities.size() / threadsNum;
    CyclicBarrier barrier = new CyclicBarrier(threadsNum);

    for (int i = 0; i < threads.length; i++) {
      int to;
      int from = i * sets;

      if (i != threadsNum - 1) {
        to = (i + 1) * sets;
      } else {
        to = entities.size();
      }

      threads[i] = new Thread(new BHRunnable(entities, from, to, iters, dt, dims, barrier), "thread" + (i + 1));
      threads[i].start();
    }

    for (int i = 0; i < threads.length; i++) {
      threads[i].join();
    }
  }

  public static void BarnesHutParallelv2(ArrayList<Entity> entities, int iters, double dims, int threadsNum, int dt) throws InterruptedException {
    
    int[] fromArr = new int[threadsNum];
    int[] toArr = new int[threadsNum];
    Thread[] threads = new Thread[threadsNum];
    CyclicBarrier barrier = new CyclicBarrier(threadsNum);

    int sets = entities.size() / threadsNum;
    
    for (int i = 0; i < threads.length; i++) {
      fromArr[i] = i * sets;
      if (i != threadsNum - 1) {
        toArr[i] = (i + 1) * sets;
      } else {
        toArr[i] = entities.size();
      }
    }

    for (int i = 0; i < iters; i++) {

      BHTree bh = BHUtils.createBHTree(entities, dims);

      for (int k = 0; k < threads.length; k++) {
        threads[k] = new Thread(new BHSimpleRunnable(entities, fromArr[k], toArr[k], dt, dims, barrier, bh),
            "thread" + (k + 1));
        threads[k].start();
      }

      for (int k = 0; k < threads.length; k++) {
        threads[k].join();
      }
    }
  }

  public static void BarnesHutSequential(ArrayList<Entity> entities, int iters, double dims, int dt) {
    for (int i = 0; i < iters; i++) {
      BHTree bh = BHUtils.createBHTree(entities, dims);

      for (Entity e : entities) {
        BHUtils.netForce(e, bh);
      }

      for (Entity e : entities) {
        BHUtils.newPosition(e, dt, dims);
      }

      // System.out.println("Iteration: " + (i + 1));
    }
  }
}
