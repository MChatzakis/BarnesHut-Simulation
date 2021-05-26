package simulation;

import java.util.ArrayList;
import structures.*;

public class BarnesHutMain {

  public static void main(String[] args) throws InterruptedException {
    long start = System.currentTimeMillis();
    long startTime = System.nanoTime();

    String filename = args[0];
    int iters = Integer.parseInt(args[1]);
    int threads = Integer.parseInt(args[2]);
    int dt = 1;

    System.out.println("Filename: " + filename);
    System.out.println("Iterations: " + iters);
    System.out.println("Threads: " + threads);

    ArrayList<Entity> entities = new ArrayList<>();
    double dims = BHUtils.parseFile(filename, entities);

    //Instant start = Instant.now();

    if (threads == 0) {
      BarnesHutSequential(entities, iters, dims, dt);
    } else {
      //par
      //BarnesHutParallel(entities,iters,dims,threads,dt);
    }

    //Instant finish = Instant.now();
    //long TE = Duration.between(start, finish).toMillis();

    long estimatedTime = System.nanoTime() - startTime;
    long finish = System.currentTimeMillis();
    long timeElapsed = finish - start;

    //BHUtils.printEntities(entities);

    System.out.println(
      "Execution time 1: " + (double) estimatedTime / 1000000000
    );

    System.out.println("Execution time 2: " + (double) timeElapsed * 0.001);
    //System.out.println("Execution time 3: " + (double) TE * 0.001);

    BHUtils.printEntities(entities, "simulationJAVA.txt");
    BHUtils.appendToFile("timesJAVA.txt", (double) timeElapsed * 0.001);
  }

  public static void BarnesHutParallel(
    ArrayList<Entity> entities,
    int iters,
    double dims,
    int threadsNum,
    int dt
  )
    throws InterruptedException {
    Thread[] threads = new Thread[threadsNum];

    int sets = entities.size() / threadsNum;

    for (int i = 0; i < threads.length; i++) {
      int to;
      int from = i * sets;

      if (i != threadsNum - 1) {
        to = (i + 1) * sets;
      } else {
        to = entities.size();
      }

      threads[i] =
        new Thread(
          new BHRunnable(entities, from, to, iters, dt, dims),
          "thread" + (i + 1)
        );
      threads[i].start();
    }

    //doing shit....
    for (int i = 0; i < threads.length; i++) {
      threads[i].join();
    }
  }

  public static void BarnesHutSequential(
    ArrayList<Entity> entities,
    int iters,
    double dims,
    int dt
  ) {
    for (int i = 0; i < iters; i++) {
      BHTree bh = BHUtils.createBHTree(entities, dims);

      for (Entity e : entities) {
        BHUtils.netForce(e, bh);
      }

      for (Entity e : entities) {
        BHUtils.newPosition(e, dt);
      }

      //System.out.println("Iteration: " + (i + 1));
    }
  }
}
