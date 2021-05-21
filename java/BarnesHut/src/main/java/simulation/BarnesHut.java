/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package simulation;

import java.util.ArrayList;
import lombok.Data;
import structures.*;
import utilCommons.Utils;

/**
 *
 * @author manos
 */
@Data
public class BarnesHut {

    private ArrayList<Entity> entities = new ArrayList<>();

    private int iters = 0;
    private double dt = 1;
    private double dims = 0;

    public static void main(String[] args) {
        new BarnesHut(1, 1, "./../../datasets/input1.txt").runSimulation();
    }

    public BarnesHut(int dt, int iters, String dataset) {
        this.dims = Utils.parseFile(dataset, this.entities);
        this.dt = dt;
        this.iters = iters;
    }

    public void runSimulation() {

        System.out.println("======================================== Simulation Started -- Logging ... ========================================");

        Utils.printEntities(entities);

        for (int i = 0; i < iters; i++) {
            BHTree bh = createBHTree();

            for (Entity e : entities) {
                netForce(e, bh);
            }

            // ----------------- //
            for (Entity e : entities) {
                newPosition(e);
            }

            //BHTree.printBHTree(bh);
        }

        System.out.println("======================================== Simulation Ended -- Printing results ... ========================================");
        Utils.printEntities(entities);
        
        //BHTree bh = createBHTree();
        //BHTree.printBHTree(bh);
    }

    private BHTree createBHTree() {
        BHTree bh = new BHTree(new Region(new Point(0, 0), dims));
        for (Entity e : entities) {
            bh.insertEntity(e);
        }
        return bh;
    }

    private void netForce(Entity e, BHTree bh) {

        if (e == null) {
            return;
        }

        if (bh.isLeaf()) {
            assert (e.equals(bh.getEntity()));
            return;
        }

        boolean pathFound = false;

        BHTree[] quads = {bh.getQuad1(), bh.getQuad2(), bh.getQuad3(), bh.getQuad4()};
        BHTree quadToGo = null;

        for (int i = 0; i < 4; i++) {

            if (quads[i] != null) {

                Region reg = quads[i].getRegion();
                Entity curr = quads[i].getEntity();

                if (curr != null) {
                    if (!pathFound && reg.containsPoint(e.getPoint())) {

                        System.out.println("Found the region that body " + e.getName() + " belongs. Going to quad[ " + i + " ]");

                        quadToGo = quads[i];
                        pathFound = true;

                    } else {
                        System.out.println("Calculating net force for body " + e.toString() + " by body " + curr.toString());

                        double f = Entity.F(curr, e);
                        double fx = Entity.Fx(curr, e, f);
                        double fy = Entity.Fy(curr, e, f);

                        e.addToSFx(fx);
                        e.addToSFy(fy);
                    }
                }

            }

        }

        assert (quadToGo != null);
        netForce(e, quadToGo);
    }

    private void newPosition(Entity e) {

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

        e.setPoint(new Point(newX, newY));

        e.setSFx(0);
        e.setSFy(0);

        e.setVx(newVx);
        e.setVy(newVy);

    }
}
