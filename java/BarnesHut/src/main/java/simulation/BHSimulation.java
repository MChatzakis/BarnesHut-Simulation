/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package simulation;

import java.util.ArrayList;
import structures.Entity;
import utilCommons.Utils;

/**
 *
 * @author manos
 */
public class BHSimulation {

    public static void sequentialSim(String path, int dt, int iterations) {
        new BarnesHutSimulation(dt, iterations, path).runSimulation();
    }

    public static void parallelSim(String path, int threadsNum, int dt, int iterations) throws InterruptedException {
        ArrayList<Entity> entities = new ArrayList<>();

        double dims = Utils.parseFile(path, entities);
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

            threads[i] = new Thread(new BHRunnable(entities, from, to, iterations, dt, dims), "thread" + (i + 1));
            threads[i].start();
        }

        //doing shit....
        for (int i = 0; i < threads.length; i++) {
            threads[i].join();
        }
    }

    public static void main(String[] args) throws InterruptedException {
        String path = "./../../datasets/test1.txt";
        int threads = 4;
        int dt = 1;
        int iters = 1;

        parallelSim(path, threads, dt, iters);

    }

}
