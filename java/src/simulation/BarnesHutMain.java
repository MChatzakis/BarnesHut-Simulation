package simulation;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.Callable;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.ForkJoinPool;
import java.util.stream.IntStream;
import structures.*;

public class BarnesHutMain {

  public static void main(String[] args) throws InterruptedException {
    long startTime = System.nanoTime();

    String filename = args[0];
    int iters = Integer.parseInt(args[1]);
    int threads = Integer.parseInt(args[2]);
    
    int dt = 1;
    boolean runAsStream = false;

    if (args.length == 4 && args[4].equals("stream")) {
      runAsStream = true;
    }

    System.out.println("Filename: " + filename);
    System.out.println("Iterations: " + iters);
    System.out.println("Threads: " + threads);

    ArrayList<Entity> entities = new ArrayList<>();
    double dims = BHUtils.parseFile(filename, entities);

    long start = System.currentTimeMillis();

    if (runAsStream) {
      BarnesHutStream(entities, iters, dims, threads, dt);
    }
    else if (threads == 0) {
      BarnesHutSequential(entities, iters, dims, dt);
    } else {
      BarnesHutParallel(entities, iters, dims, threads, dt);
    }

    long estimatedTime = System.nanoTime() - startTime;
    long finish = System.currentTimeMillis();
    long timeElapsed = finish - start;

    // BHUtils.printEntities(entities);

    System.out.println("Execution time 1: " + (double) estimatedTime / 1000000000);
    System.out.println("Execution time 2: " + (double) timeElapsed * 0.001);

    BHUtils.printEntities(entities, "simulationJAVA.txt");
    BHUtils.appendToFile("timesJAVA.txt", (double) timeElapsed * 0.001);
  }

  public static void BarnesHutStream(ArrayList<Entity> entities, int iters, double dims, int threadsNum, int dt) {
    ForkJoinPool customThreadPool = new ForkJoinPool(threadsNum);
    BHTree bh = BHUtils.createBHTree(entities, dims);
    customThreadPool.submit(() -> entities.parallelStream().forEach(e -> {
      // System.out.println(n)
      BHUtils.netForce(e, bh);
    }));

    customThreadPool.submit(() -> entities.parallelStream().forEach(e -> {
      // System.out.println(n)
      BHUtils.newPosition(e, dt);
    }));

    customThreadPool.shutdownNow();
  }

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

    // doing shit....
    for (int i = 0; i < threads.length; i++) {
      threads[i].join();
    }
  }

  public static void BarnesHutSequential(ArrayList<Entity> entities, int iters, double dims, int dt) {
    for (int i = 0; i < iters; i++) {
      BHTree bh = BHUtils.createBHTree(entities, dims);

      for (Entity e : entities) {
        BHUtils.netForce(e, bh);
      }

      for (Entity e : entities) {
        BHUtils.newPosition(e, dt);
      }
      // System.out.println("Iteration: " + (i + 1));
    }
  }
}
