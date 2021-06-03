package simulation;

import java.util.ArrayList;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import structures.BHTree;
import structures.Entity;

public class BHSimpleRunnable implements Runnable {

    private ArrayList<Entity> entities;

    private int dt;
    private int from;
    private int to;
    private BHTree bh = null;
    private double dims;

    private final CyclicBarrier barrier;

    public BHSimpleRunnable(ArrayList<Entity> entities, int from, int to, int dt, double dims, CyclicBarrier barrier,
            BHTree bh) {
        this.barrier = barrier;
        this.bh = bh;
        this.dt = dt;
        this.entities = entities;
        this.dims = dims;
        this.from = from;
        this.to = to;
    }

    @Override
    public void run() {
        try {
            
            for (int i = from; i < to; i++) {
                BHUtils.netForce(entities.get(i), bh);
            }

            barrier.await();

            for (int i = from; i < to; i++) {
                BHUtils.newPosition(entities.get(i), dt, dims);
            }

        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }
    }

}
