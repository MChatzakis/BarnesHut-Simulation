package simulation;

import java.util.ArrayList;
import structures.*;

public class BarnesHutMain {

  public static void main(String[] args) {
    String filename = args[0];
    int iters = Integer.parseInt(args[1]);
    int threads = Integer.parseInt(args[2]);

    System.out.println("Filename: " + filename);
    System.out.println("Iterations: " + iters);
    System.out.println("Threads: " + threads);

    ArrayList<Entity> entities = new ArrayList<>();
    double dims = BHUtils.parseFile(filename, entities);

    long start = System.currentTimeMillis();

    if (threads == 0) {
      //seq
    } else {
      //par
    }

    long finish = System.currentTimeMillis();
    long timeElapsed = finish - start;

    System.out.println("Execution time: " + timeElapsed);
  }
}
